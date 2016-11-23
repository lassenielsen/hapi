#include <hapi/mpstype.hpp>
#include <dpl/slrparser.hpp>
#include <hapi/mpsterms.hpp>
#include <hapi/mpsgui.hpp>

#include <hapi/common.hpp>

using namespace std;
using namespace hapi;
using namespace dpl;

int MpsTerm::ourNextId = 1;

/* MpsTerm implementation
 */
string MpsTerm::NewName(string base) // {{{
{
  string result = (string)"~"+base;
  result += int2string(ourNextId++);
  return result;
} // }}}
MpsTerm *MpsTerm::Error(const string &msg) // {{{
{
  string name=(string)"_ERROR: " + msg;
  vector<MpsExp*> arg;
  return new MpsCall(name, arg, arg, vector<MpsMsgType*>(), vector<MpsMsgType*>());
} // }}}
void MpsTerm::FreeLink(const string &name) // {{{
{ myFreeLinks.push_back(name);
} // }}}
const vector<string> &MpsTerm::GetFreeLinks() const // {{{
{ return myFreeLinks;
} // }}}

/* Static type-checking of deadlock and communication safety
 */
bool MpsTerm::TypeCheck() // {{{
{
  // Create environments
  MpsBoolVal Theta(true);
  MpsMsgEnv Gamma;
  MpsProcEnv Omega;
  tdc_wrapper wrap=tdc_wrap::check;
  tdc_wraperr wrap_err=tdc_wrap::check_err;
  vector<string> *result=(vector<string>*)TDCompile(wrap,wrap_err,Theta,Gamma,Omega,set<pair<string,int> >(),"",CPS_IMPURE,true);
  bool success=true;

  for (size_t i=0; i<result->size(); ++i)
  { success=false;
    cerr << "TypeCheck Error: " << (*result)[i] << endl;
  }

  delete result;
  return success;
} // }}}

/* Create list of possible steps
 */
// Remove unused function definitions
void CleanDefinitions(const MpsTerm *body, vector<MpsFunction> &env) // Remove unused functions from env {{{
{
  // Generate list of used functions
  set<string> usedDefs;
  set<string> bufDefs=body->FPV();
  while (bufDefs.size()>0)
  { string funName = *bufDefs.begin();
    bufDefs.erase(funName);
    usedDefs.insert(funName);
    vector<MpsFunction>::const_iterator fun=env.begin(); // Find definition
    while (fun!=env.end() && fun->GetName()!=funName) // FIXME: Linear search!
      ++fun;
    if (fun!=env.end())
    { // Add used definitions recursively (if new)
      set<string> tmp=fun->GetBody().FPV();
      for (set<string>::const_iterator it=tmp.begin(); it!=tmp.end(); ++it)
        if (usedDefs.count(*it)==0)
          bufDefs.insert(*it);
    }
  }
  // Remove unused functions
  vector<MpsFunction>::iterator fun=env.begin();
  while (fun!=env.end())
  { if (usedDefs.count(fun->GetName())==0)
      fun=env.erase(fun);
    else
      ++fun;
  }
} // }}}
// Find possible steps, and make step based on given choice
MpsTerm *MpsTerm::Step(MpsEnv &env, vector<MpsFunction> &defs, int &choice, int &choices) // {{{
{
  vector<MpsStep> steps;
  Steps(env,defs,steps);
  choices = steps.size();
  MpsTerm *result=NULL;
  if (choices>0)
  {
    if (choice<0 || choice>=choices) // Make random choice
      choice = rand()%choices;
    result = steps[choice].Apply(*this,env,defs);
    CleanDefinitions(result,defs);
  }
  else
    choice=-1;
  return result;
} // }}}
// Create all n-combinations of the elements
vector<vector<int> > combine(vector<int>::const_iterator first, vector<int>::const_iterator end, int n) // {{{
{ vector<vector<int> > result;
  if (n==0)
    result.push_back(vector<int>());
  if (first==end || n<=0 || end-first<n)
    return result;

  vector<vector<int> > with=combine(first+1,end,n-1);
  for (vector<vector<int> >::iterator it=with.begin(); it!=with.end(); ++it)
    it->push_back(*first);
  vector<vector<int> > without=combine(first+1,end,n);
  result.insert(result.end(),with.begin(),with.end());
  result.insert(result.end(),without.begin(),without.end());
  return result;
} // }}}
// Get list of partial steps from Substeps, and create list of complete steps
void MpsTerm::Steps(MpsEnv &env, const vector<MpsFunction> &defs, vector<MpsStep> &dest) // {{{
{
  bool first=true;
  bool wait=true;
  // Retry until steps found, or no gui dependance
  while (first || (wait && dest.size()==0 && mpsgui::GuiEnabeled()))
  { first=false;
    vector<MpsStep> substeps;
    substeps.clear();
    wait=SubSteps(substeps);
  
    vector<MpsStep> linksteps;
    vector<MpsStep> syncsteps;
    // Step 0: Traverse list and move complete steps to dest
    while (substeps.size()>0)
    {
      if (substeps.front().GetEvent().myType==link)
        linksteps.push_back(substeps.front());
      else if (substeps.front().GetEvent().myType==sync)
        syncsteps.push_back(substeps.front());
      else if (substeps.front().GetEvent().myType==rcv ||
               substeps.front().GetEvent().myType==brcv)
      { MpsEnv::const_iterator queue = env.find(substeps.front().GetEvent().myChannel);
        if (queue!=env.end() && queue->second.size()>0)
          dest.push_back(substeps.front());
      }
      else // tau or send step
        dest.push_back(substeps.front());
      substeps.erase(substeps.begin());
    }
    // Step 1a: Group link steps by (channel,maxpid)
    map<pair<string,int>,map<int,vector<int> > > links; // (channel,maxpid) -> pid -> index set
    links.clear();
    for (int i=0; i<linksteps.size(); ++i)
      links[pair<string,int>(linksteps[i].GetEvent().myChannel.ToString(),linksteps[i].GetEvent().myMaxpid)]
           [linksteps[i].GetEvent().myPid].push_back(i);
    // Step 1b: Collect complete link steps and add to dest
    for (map<pair<string,int>,map<int,vector<int> > >::iterator group=links.begin(); group!=links.end(); ++group) // foreach (channel,maxpid)
    { if (group->first.second>0) // Check maxpid>0
      { vector<vector<int> > combis; // Initialize groups as the possible 1st participants
        combis.clear();
        for (vector<int>::const_iterator p=group->second[1].begin();p!=group->second[1].end(); ++p) // for each participant with pid=1
        { vector<int> combi;
          combi.clear();
          combi.push_back(*p);
          combis.push_back(combi);
        }
        for (int pid=2; pid<=group->first.second; ++pid) // For each participant id 2..maxpid
        { vector<vector<int> > last_combis=combis;
          combis.clear();
          for (vector<vector<int> >::const_iterator c=last_combis.begin();c!=last_combis.end(); ++c) // For each existing group
          for (vector<int>::const_iterator group_pid=group->second[pid].begin(); group_pid!=group->second[pid].end(); ++group_pid) // for each pid'th participant
          { vector<int> new_combi=*c;          // Create group extended with
            new_combi.push_back(*group_pid);   // new participant
            combis.push_back(new_combi);
          }
        }
        // Add complete links to dest
        for (vector<vector<int> >::const_iterator fullgroup=combis.begin();fullgroup!=combis.end(); ++fullgroup) // For each existing group
        { MpsEvent event;
          event.myType=tau;
          event.mySubType=link;
          event.myChannel = MpsChannel(group->first.first);
          event.mySession = "session";
          event.myMaxpid=group->first.second;
          vector<string> paths;
          for (vector<int>::const_iterator p=fullgroup->begin(); p!=fullgroup->end(); ++p) // for each participant in group
            paths.push_back(linksteps[*p].GetPaths()[0]);
          dest.push_back(MpsStep(event,paths));
        }
      }
    }
    // clean up
    linksteps.clear();
    links.clear();
    // Step 2a: Group sync steps by (session,maxpid)
    map<pair<string,int>,map<string,vector<int> > > syncs; // (session,maxpid) -> label -> index set
    for (int i=0; i<syncsteps.size(); ++i)
      syncs[pair<string,int>(syncsteps[i].GetEvent().mySession,syncsteps[i].GetEvent().myMaxpid)]
           [syncsteps[i].GetEvent().myLabel].push_back(i);
    // Step 2b: Collect complete sync steps and add to dest
    for (map<pair<string,int>,map<string,vector<int> > >::iterator group=syncs.begin(); group!=syncs.end(); ++group) // foreach (channel,maxpid)
      for (map<string,vector<int> >::const_iterator choice=group->second.begin(); choice!=group->second.end(); ++choice) // foreach choice in group
      { vector<vector<int> > combis=combine(choice->second.begin(),choice->second.end(),group->first.second);
        for (vector<vector<int> >::const_iterator combi=combis.begin(); combi!=combis.end(); ++combi)
        { MpsEvent event;
          event.myType=tau;
          event.mySubType=sync;
          event.myChannel = MpsChannel(group->first.first);
          event.mySession=group->first.first;
          event.myMaxpid=group->first.second;
          event.myLabel=choice->first;
          vector<string> paths;
          for (vector<int>::const_iterator p=combi->begin(); p!=combi->end(); ++p) // for each participant in sync
            paths.push_back(syncsteps[*p].GetPaths()[0]);
          dest.push_back(MpsStep(event,paths));
        }
      }
    // clean up
    syncsteps.clear();
    syncs.clear();
  
    if (wait && dest.size()==0) // Wait on GUI decission
    {
      if (mpsgui::GuiEnabeled())
      {
#if HAPI_DEBUG_LEVEL>4
        cout << "*********** Waiting for GUI ***********" << endl;
#endif
        mpsgui::GuiWait();
#if HAPI_DEBUG_LEVEL>4
        cout << "*********** GUI Event *****************" << endl;
#endif
      }
      else
      {
#if HAPI_DEBUG_LEVEL>4
        cout << "******* Stuck: GUI not enabeled *******" << endl;
#endif
      }
    }
  }
} // }}}

/* Applying Steps
 */
MpsTerm *MpsTerm::ApplyRcv(const std::string &path, const MpsExp *val) const // {{{
{ return Error((string)"Applying Rcv on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplySnd(const std::string &path, MpsExp **val, MpsChannel &ch) const // {{{
{ return Error((string)"Applying Snd on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyBRcv(const std::string &path, const std::string &label) const // {{{
{ return Error((string)"Applying BRcv on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyBSnd(const std::string &path, std::string &label, MpsChannel &ch) const // {{{
{ return Error((string)"Applying BSnd on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyLink(const std::vector<std::string> &paths, const std::string &session) const // {{{
{ if (paths.size()>0)
    return Error((string)"Applying Link on "+ToString());
  else
    return Copy();
} // }}}
MpsTerm *MpsTerm::ApplySync(const std::vector<std::string> &paths, const std::string &label) const // {{{
{ if (paths.size()>0)
    return Error((string)"Applying Sync on "+ToString());
  else
    return Copy();
} // }}}
MpsTerm *MpsTerm::ApplyDef(const std::string &path, std::vector<MpsFunction> &dest) const // {{{
{ return Error((string)"Applying Def on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyCall(const std::string &path, const std::vector<MpsFunction> &funs) const // {{{
{ return Error((string)"Applying Call on "+ToString());
} // }}}
MpsTerm *MpsTerm::ApplyOther(const std::string &path) const // {{{
{ return Error((string)"Applying Other on "+ToString());
} // }}}

int _compile_id=0;
/* Make executable C++ code for processes
 */
string MpsTerm::MakeC() const // {{{
{ _compile_id=1;
  MpsTerm *step1=RenameAll();
  MpsTerm *step2=step1->FlattenFork(false,true,false);
  delete step1;
  MpsTerm *step3=step2->CloseDefinitions(MpsMsgEnv());
  delete step2;
  MpsFunctionEnv defs;
  // Move definitions to global env
  MpsTerm *main=step3->ExtractDefinitions(defs);
  delete step3;
  string result = (string)
       + "#include <iostream>\n"
       + "#include <cstdlib>\n"
       + "#include <pthread.h>\n"
       + "#include <libpi/value.hpp>\n"
       + "#include <libpi/thread/channel.hpp>\n"
       + "#include <vector>\n"
       + "#include <queue>\n"
       + "#include <sstream>\n"
       + "#include <atomic>\n"
       + "#include <memory>\n"
       + main->ToCHeader()
       + "using namespace std;\n"
       + "using namespace libpi;\n\n"
       + "inline atomic<int> *_new_shared_int()\n"
       + "{ return (std::atomic<int>*)mmap(NULL, sizeof(atomic<int>), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // Actual number of active processes\n"
       + "}\n"
       + "std::atomic<int> *_aprocs=_new_shared_int(); // Actual number of active processes\n"
       + "inline void _inc_aprocs() // {{{\n"
       + "{ ++(*_system_aprocs);\n"
       + "} // }}}\n"
       + "inline void _dec_aprocs() // {{{\n"
       + "{ --(*_system_aprocs);\n"
       + "} // }}}\n"
       + "int _system_tprocs=std::thread::hardware_concurrency(); // Target number of active processes\n"
       + "std::vector<char*> _system_args;   // Store args for use in processes\n"
       + "\n"
       + "// Declare implementation\n"
       + "void *_methods(void *arg);\n"
       + "\n"
       + "struct State\n"
       + "{ void *label;\n"
       + "  vector<std::shared_ptr<libpi::Value> > values;\n"
       + "};\n"
       + "\n"
       + "inline void _spawn_thread(State *state)\n"
       + "{ pthread_t x;\n"
       + "  pthread_attr_t y;\n"
       + "  pthread_attr_init(&y);\n"
       + "  pthread_attr_setstacksize(&y,16384);\n"
       + "  pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);\n"
       + "  pthread_create(&x,&y,_methods,(void*)state);\n"
       + "}\n"
       + "\n\n/* All methods */\n"
       + "#define _state ((State*)(arg))\n"
       + "void *_methods(void *arg)\n"
       + "{\n"
       + "  if (state==NULL) // Error\n"
       + "  { std::cerr << \"Error in call to all_methods: null state provided\" << endl;\n"
       + "    return NULL;\n"
       + "  }  \n"
       + "  if (state->label!=NULL)\n"
       + "    goto *state->label;\n"
       + "  // Main\n"
       + main->ToC()
       + DefEnvToC(defs)
       + "}\n"
       + "\n\n/*Start process, and its continuations */\n"
       + "int main(int argc, char **argv) // {{{\n"
       + "{ // PARSE ARGS!!\n"
       + "  for (int i=0; i<argc; ++i)\n"
       + "  { if (string(argv[i])==\"-pi_tprocs\" && i+1<argc)\n"
       + "      _system_tprocs=atoi(argv[++i]);\n"
       + "    else\n"
       + "      _system_args.push_back(argv[i]);\n"
       + "  }\n"
       + "  (*__system_aprocs)=1;\n"
       + "  try\n"
       + "  { signal(SIGCHLD, SIG_IGN); // Fork optimization\n"
       + "    State *s0=new State();\n"
       + "    s0->label=NULL;\n"
       + "    _methods(s0);\n"
       + "    //munmap(__system_aprocs,sizeof(int));\n"
       + "  } catch (const string &error)\n"
       + "  { cerr << \"Error: \" << error << endl;\n"
       + "    return 1;\n"
       + "  }\n"
       + "  return 0;\n"
       + "} // }}}";
  delete main;
  return result;
} // }}}
MpsTerm *MpsTerm::Parallelize() const // {{{
{ MpsTerm *seqTerm;
  MpsTerm *parTerm;
  bool opt=Parallelize(MpsEnd(),seqTerm,parTerm);
  if (!opt)
  { delete parTerm;
    return seqTerm;
  }
  MpsTerm *result = new MpsCond(MpsBinOpExp("<=",MpsSystemExp("tprocs"),MpsSystemExp("aprocs"),MpsMsgNoType(),MpsMsgNoType()),*seqTerm,*parTerm);

  // Clean up
  delete seqTerm;
  delete parTerm;

  return result;
} // }}}
void MpsTerm::Split(const set<string> &fv, MpsTerm* &pre, MpsTerm* &post) const // {{{
{ pre=Copy();
  post=new MpsEnd();
} // }}}

namespace hapi
{
namespace tdc_wrap
{
void *check(MpsTerm *term, // {{{
            const MpsExp &Theta,
            const MpsMsgEnv &Gamma,
            const MpsProcEnv &Omega, 
            const std::set<std::pair<std::string,int> > &pureStack,
            const std::string &curPure,
            MpsTerm::PureState pureState,
	    bool checkPure,
            std::map<std::string,void*> &children)
{ std::vector<std::string> *result=new std::vector<std::string>();
  // Cleanup
  for (std::map<std::string,void*>::iterator child=children.begin(); child!=children.end(); ++child)
  { result->insert(result->end(),((std::vector<std::string>*)child->second)->begin(),((std::vector<std::string>*)child->second)->end());
    delete ((std::vector<std::string>*)child->second);
  }
  return result;
} // }}}
void *check_err(MpsTerm *term, // {{{
                std::string msg)
{ std::vector<std::string> *result=new std::vector<std::string>();
  result->push_back(msg);
  return (void*)result;
} // }}}
}
}
