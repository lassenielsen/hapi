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
void* MpsTerm::TDCompile(tdc_pre pre, // {{{
                         tdc_post post,
                         tdc_error error,
                         const MpsExp &Theta,
                         const MpsMsgEnv &Gamma,
                         const MpsProcEnv &Omega, 
                         const std::set<std::pair<std::string,int> > &pureStack,
                         const std::string &curPure,
                         PureState pureState,
                         bool checkPure)
{ MpsTerm *tmp=pre(this,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure);
  //try
  { void *result=tmp->TDCompileMain(pre,post,error,Theta,Gamma,Omega,pureStack,curPure,pureState,checkPure);
    if (tmp!=NULL && tmp!=this)
      delete tmp;
    return result;
  }
  //catch (...)
  //{ if (tmp!=NULL && tmp!=this)
  //    delete tmp;
  //  throw;
  //}
} // }}}

/* Static type-checking of deadlock and communication safety
 */
bool MpsTerm::TypeCheck(bool checkPurity) // {{{
{
  // Create environments
  MpsBoolVal Theta(true);
  MpsMsgEnv Gamma;
  MpsProcEnv Omega;
  tdc_pre pre=tdc_wrap::pre_void;
  tdc_post wrap=tdc_wrap::wrap_vector;
  tdc_error wrap_err=tdc_wrap::error_vector;
  vector<string> *result=(vector<string>*)TDCompile(pre,wrap,wrap_err,Theta,Gamma,Omega,set<pair<string,int> >(),"",CPS_IMPURE,checkPurity);
  bool success=true;

  for (size_t i=0; i<result->size(); ++i)
  { success=false;
    cerr << "TypeCheck Error: " << (*result)[i] << endl << endl;
  }

  delete result;
  return success;
} // }}}
MpsTerm *MpsTerm::CloseDefs() // {{{
{
  // Create environments
  MpsBoolVal Theta(true);
  MpsMsgEnv Gamma;
  MpsProcEnv Omega;
  tdc_pre pre=tdc_wrap::pre_closedefs;
  tdc_post wrap=tdc_wrap::wrap_copy;
  tdc_error wrap_err=tdc_wrap::error_throw;
  return (MpsTerm*)TDCompile(pre,wrap,wrap_err,Theta,Gamma,Omega,set<pair<string,int> >(),"",CPS_IMPURE,false);
} // }}}
// TODO: Type Driven Compilation: string MpsTerm::GenerateC() // {{{
// TODO: Type Driven Compilation: {
// TODO: Type Driven Compilation:   // Create environments
// TODO: Type Driven Compilation:   MpsBoolVal Theta(true);
// TODO: Type Driven Compilation:   MpsMsgEnv Gamma;
// TODO: Type Driven Compilation:   MpsProcEnv Omega;
// TODO: Type Driven Compilation:   tdc_pre pre=tdc_wrap::pre_void;
// TODO: Type Driven Compilation:   tdc_post wrap=tdc_wrap::wrap_vector;
// TODO: Type Driven Compilation:   tdc_error wrap_err=tdc_wrap::error_vector;
// TODO: Type Driven Compilation:   vector<string> *result=(vector<string>*)TDCompile(pre,wrap,wrap_err,Theta,Gamma,Omega,set<pair<string,int> >(),"",CPS_IMPURE,checkPurity);
// TODO: Type Driven Compilation:   bool success=true;
// TODO: Type Driven Compilation: 
// TODO: Type Driven Compilation:   for (size_t i=0; i<result->size(); ++i)
// TODO: Type Driven Compilation:   { success=false;
// TODO: Type Driven Compilation:     cerr << "TypeCheck Error: " << (*result)[i] << endl << endl;
// TODO: Type Driven Compilation:   }
// TODO: Type Driven Compilation: 
// TODO: Type Driven Compilation:   delete result;
// TODO: Type Driven Compilation:   return success;
// TODO: Type Driven Compilation: } // }}}

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
  stringstream result;
  result << "/* ==== ORIGINAL ====\n" << ToString() << "\n*/\n";
  MpsTerm *step1=RenameAll();
  result << "/* ==== RENAMED ====\n" << step1->ToString() << "\n*/\n";
  MpsTerm *step2=step1->FlattenFork(false,true,false);
  delete step1;
  result << "/* ==== FLATTENFORKED ====\n" << step2->ToString() << "\n*/\n";
  MpsTerm *step3=step2->CloseDefs();
  delete step2;
  result << "/* ==== CLOSEDEFED ====\n" << step3->ToString() << "\n*/\n";
  MpsFunctionEnv defs;
  // Move definitions to global env
  MpsTerm *main=step3->ExtractDefinitions(defs);
  delete step3;
  // Ready to generate code
  string mainTask="TaskMain";
  result
    << "#include <libpi/value.hpp>\n"
    << "#include <libpi/bool.hpp>\n"
    << "#include <libpi/int.hpp>\n"
    << "#include <libpi/string.hpp>\n"
    << "#include <libpi/session.hpp>\n"
    << "#include <libpi/task/link.hpp>\n"
    << "#include <libpi/task/worker.hpp>\n"
    << "#include <thread>\n"
    << "#include <signal.h>\n"
    << "using namespace std;\n"
    << "// HEADERS {{{\n"
    << main->ToCHeader();
  for (MpsFunctionEnv::const_iterator def=defs.begin(); def!=defs.end(); ++def)
    result << def->GetBody().ToCHeader();
  result
    << "// }}}\n"
    << "// Value declerations {{{\n"
    << "std::vector<char*> _args;\n";
  std::unordered_set<std::string> existing;
  std::vector<std::string> consts;
  // Create const definitions without duplicates
  main->ToCConsts(consts,existing);
  for (MpsFunctionEnv::const_iterator def=defs.begin(); def!=defs.end(); ++def)
    def->GetBody().ToCConsts(consts,existing);
  // Add const defs to result
  for (std::vector<std::string>::const_iterator c=consts.begin(); c!=consts.end(); ++c)
    result << *c;
  // Add framework to result
  result
    << "// }}}\n"
    << "// Task Types {{{\n";
  set<string> ids=main->EV();
  result
  << "class " << mainTask << " : public libpi::task::Task" << endl
  << "{ public:" << endl;
  for (set<string>::const_iterator id=ids.begin(); id!=ids.end(); ++id)
    result << "    shared_ptr<libpi::Value> var_" << ToC_Name(*id) << ";" << endl;
  result
  << "};" << endl;
  for (MpsFunctionEnv::const_iterator def=defs.begin(); def!=defs.end(); ++def)
    result << def->ToCTaskType();
  result
    << "// }}}\n"
    << "// All Methods {{{\n"
    << "inline bool _methods(shared_ptr<libpi::task::Task> &_task)\n"
    << "{ size_t _steps=0;\n"
    << "  void *_label=_task->GetLabel();\n"
    << "  if (_label!=NULL)\n"
    << "    goto *_label;\n"
    << "  method_Main:\n"
    << "  #define _this ((" << mainTask << "*)_task.get())" << endl
    << "  { // Main {{{\n"
    << main->ToC(mainTask)
    << "  } // }}}\n"
    << DefEnvToC(defs)
    << "} // }}}\n"
    << "void libpi::task::Worker_Pool::Work() // {{{\n"
    << "{ try\n"
    << "  { while (true) // Continue until termination\n"
    << "    { // Test if program is complete\n"
    << "      if (ActiveTasks==0)\n"
    << "      { ourIdleWorkersLock.Lock();\n"
    << "        shared_ptr<Task> nullTask;\n"
    << "	while (!ourIdleWorkers.empty())\n"
    << "        { ourIdleWorkers.front()->EmployTask(nullTask);\n"
    << "          ourIdleWorkers.pop();\n"
    << "        }\n"
    << "        ourIdleWorkersSize=0;\n"
    << "        ourIdleWorkersLock.Release();\n"
    << "        break;\n"
    << "      }\n"
    << "      // Find next task\n"
    << "      while (myActiveTasks.empty())\n"
    << "      { ourIdleWorkersLock.Lock();\n"
    << "        ourIdleWorkers.push(this);\n"
    << "        ++ourIdleWorkersSize;\n"
    << "        ourIdleWorkersLock.Release();\n"
    << "        myWaitLock.Lock();\n"
    << "      }\n"
    << "      shared_ptr<Task> task=myActiveTasks.front();\n"
    << "      myActiveTasks.pop();\n"
    << "\n"
    << "      resume_task:\n"
    << "      if (!task)\n"
    << "        break;\n"
    << "      if (_methods(task))\n"
    << "      { if (myActiveTasks.empty())\n"
    << "          goto resume_task;\n"
    << "        else QueueTask(task);\n"
    << "      }\n"
    << "      else\n"
    << "        --ActiveTasks;\n"
    << "    }\n"
    << "  } catch (const string &error)\n"
    << "  { cerr << \"Error: \" << error << endl;\n"
    << "  }\n"
    << "  return;\n"
    << "} // }}}\n"
    << "void *_start_worker(void *arg) // {{{\n"
    << "{ libpi::task::Worker *worker=(libpi::task::Worker*)arg;\n"
    << "  worker->Work();\n"
    << "  delete worker;\n"
    << "} // }}}\n"
    << "inline void _start_workers() // {{{\n"
    << "{ pthread_t x[libpi::task::Worker::TargetTasks];\n"
    << "  pthread_attr_t y;\n"
    << "  pthread_attr_init(&y);\n"
    << "  pthread_attr_setstacksize(&y,16384);\n"
    << "  //pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);\n"
    << "  for (size_t wc=0; wc<libpi::task::Worker::TargetTasks; ++wc)\n"
    << "  { libpi::task::Worker *_worker=new libpi::task::Worker_Pool();\n"
    << "    if (wc==0)\n"
    << "    { // Create main task\n"
    << "      shared_ptr<libpi::task::Task> _main(new " << mainTask << "());\n"
    << "      _main->SetLabel(NULL);\n"
    << "      _main->SetWorker(_worker);\n"
    << "      _worker->AddTask(_main);\n"
    << "    }\n"
    << "    // Bind thread to specific core\n"
    << "    //cpu_set_t cpuset;\n"
    << "    //CPU_ZERO(&cpuset);\n"
    << "    //CPU_SET(wc%cores, &cpuset);\n"
    << "    //pthread_setaffinity_np(x[wc],sizeof(cpu_set_t), &cpuset);\n"
    << "    pthread_create(&(x[wc]),&y,_start_worker,(void*)_worker);\n"
    << "  }\n"
    << "  for (size_t wc=0; wc<libpi::task::Worker::TargetTasks; ++wc)\n"
    << "    pthread_join(x[wc],NULL);\n"
    << "} // }}}\n"
    << "/*Start process, and its continuations */\n"
    << "int main(int argc, char **argv) // {{{\n"
    << "{ // PARSE ARGS!!\n"
    << "  for (int i=0; i<argc; ++i)\n"
    << "  { if (string(argv[i])==\"-pi_tprocs\" && i+1<argc)\n"
    << "    libpi::task::Worker::TargetTasks=atoi(argv[++i]);\n"
    << "    else\n"
    << "      _args.push_back(argv[i]);\n"
    << "  }\n"
    << "  try\n"
    << "  { signal(SIGCHLD, SIG_IGN); // Fork optimization\n"
    << "    // Start workers\n"
    << "    _start_workers();\n"
    << "    //munmap(_aprocs,sizeof(int));\n"
    << "  } catch (const string &error)\n"
    << "  { cerr << \"Error: \" << error << endl;\n"
    << "    return 1;\n"
    << "  }\n"
    << "  return 0;\n"
    << "} // }}}";
  delete main;
  return result.str();
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
MpsTerm *pre_void(MpsTerm *term, // {{{
                  const MpsExp &Theta,
                  const MpsMsgEnv &Gamma,
                  const MpsProcEnv &Omega, 
                  const std::set<std::pair<std::string,int> > &pureStack,
                  const std::string &curPure,
                  MpsTerm::PureState pureState,
                  bool checkPure)
{ return term;
} // }}}
MpsTerm *pre_closedefs(MpsTerm *term, // {{{
                       const MpsExp &Theta,
                       const MpsMsgEnv &Gamma,
                       const MpsProcEnv &Omega, 
                       const std::set<std::pair<std::string,int> > &pureStack,
                       const std::string &curPure,
                       MpsTerm::PureState pureState,
                       bool checkPure)
{ return term->CloseDefsPre(Gamma);
} // }}}
void *wrap_vector(MpsTerm *term, // {{{
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
void *wrap_copy(MpsTerm *term, // {{{
                const MpsExp &Theta,
                const MpsMsgEnv &Gamma,
                const MpsProcEnv &Omega, 
                const std::set<std::pair<std::string,int> > &pureStack,
                const std::string &curPure,
                MpsTerm::PureState pureState,
                bool checkPure,
                std::map<std::string,void*> &children)
{ MpsTerm *result=term->CopyWrapper(children);
  // Cleanup
  for (std::map<std::string,void*>::iterator child=children.begin(); child!=children.end(); ++child)
    delete ((MpsTerm*)child->second);
  return (void*)result;
} // }}}
//void *wrap_consts(MpsTerm *term, // {{{
//                  const MpsExp &Theta,
//                  const MpsMsgEnv &Gamma,
//                  const MpsProcEnv &Omega, 
//                  const std::set<std::pair<std::string,int> > &pureStack,
//                  const std::string &curPure,
//                  MpsTerm::PureState pureState,
//                  bool checkPure,
//                  std::map<std::string,void*> &children)
//{ std::map<string,MpsMsgType> *result=term->GetConsts(Gamma,Omega,children);
//  // Cleanup
//  for (map<string,void*>::iterator child=children.begin(); child!=children.end(); ++child)
//    delete ((map<string,MpsMsgType>*)child->second);
//  return (void*)result;
//} // }}}
//void *wrap_genc(MpsTerm *term, // {{{
//                const MpsExp &Theta,
//                const MpsMsgEnv &Gamma,
//                const MpsProcEnv &Omega, 
//                const std::set<std::pair<std::string,int> > &pureStack,
//                const std::string &curPure,
//                MpsTerm::PureState pureState,
//                bool checkPure,
//                std::map<std::string,void*> &children)
//{ std::string *result=term->GenerateC(Gamma,Omega,children);
//  // Cleanup
//  for (std::map<std::string,void*>::iterator child=children.begin(); child!=children.end(); ++child)
//    delete ((string*)child->second);
//  return (void*)result;
//} // }}}
void *error_vector(MpsTerm *term, // {{{
                   std::string msg,
                   std::map<std::string,void*> &children)
{ std::vector<std::string> *result=new std::vector<std::string>();
  result->push_back(msg);
  // Cleanup
  for (std::map<std::string,void*>::iterator child=children.begin(); child!=children.end(); ++child)
  { result->insert(result->end(),((std::vector<std::string>*)child->second)->begin(),((std::vector<std::string>*)child->second)->end());
    delete ((std::vector<std::string>*)child->second);
  }
  return (void*)result;
} // }}}
void *error_throw(MpsTerm *term, // {{{
                  std::string msg,
                  std::map<std::string,void*> &children)
{ // Cleanup
  for (std::map<std::string,void*>::iterator child=children.begin(); child!=children.end(); ++child)
    delete ((std::vector<std::string>*)child->second);
  throw msg;
} // }}}
}
}
