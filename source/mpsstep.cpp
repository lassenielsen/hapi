#include <apims/mpsstep.hpp>
#include <apims/mpsterm.hpp>
#include <apims/mpsgui.hpp>

using namespace std;
//using namespace apims;

namespace apims
{
MpsStep::MpsStep(const MpsEvent &event, const vector<string> &paths) // {{{
{ myEvent=event;
  myPaths=paths;
} // }}}
MpsStep::MpsStep(const MpsStep &rhs) // {{{
{ myEvent=rhs.myEvent;
  myPaths=rhs.myPaths;
} // }}}
MpsStep::~MpsStep() // {{{
{
} // }}}
const MpsEvent &MpsStep::GetEvent() const // {{{
{ return myEvent;
} // }}}
const vector<string> &MpsStep::GetPaths() const // {{{
{ return myPaths;
} // }}}

MpsTerm *MpsStep::Apply(const MpsTerm &term, MpsEnv &env, vector<MpsFunction> &defs) const // {{{
{ switch (myEvent.myType)
  { case tau:
      switch (myEvent.mySubType)
      { case link:
          return term.ApplyLink(myPaths,MpsTerm::NewName(myEvent.mySession));
          break;
        case sync:
        { 
          MpsTerm *result=term.ApplySync(myPaths,myEvent.myLabel);
          mpsgui::ClearChoices(myEvent.myChannel.ToString()); // Inform GUI
          return result;
        } break;
        case fundef:
          if (myPaths.size()==1)
            return term.ApplyDef(myPaths[0],defs);
          else
            return MpsTerm::Error("Error on Send application");
          break;
        case funcall:
          if (myPaths.size()==1)
            return term.ApplyCall(myPaths[0],defs);
          else
            return MpsTerm::Error("Error on Send application");
          break;
        default:
          if (myPaths.size()==1)
            return term.ApplyOther(myPaths[0]);
          else
            return MpsTerm::Error("Error on Send application");
          break;
      }
      break;
    case snd:
      if (myPaths.size()==1)
      {
        MpsEvent evt;
        evt.myType=snd;
        MpsTerm *result=term.ApplySnd(myPaths[0],&evt.myValue,evt.myChannel);
        env[evt.myChannel].push_back(evt);
        return result;
      }
      else
        return MpsTerm::Error("Error on Send application");
      break;
    case rcv:
      if (myPaths.size()==1 && env[myEvent.myChannel].size()>0 &&
          env[myEvent.myChannel].front().myType==snd)
      {
        MpsExp *val=env[myEvent.myChannel].front().myValue->Copy();
        env[myEvent.myChannel].pop_front();
        MpsTerm *result=term.ApplyRcv(myPaths[0],val);
        delete val;
        return result;
      }
      else
        return MpsTerm::Error("Error on Receive application");
      break;
    case bsnd:
      if (myPaths.size()==1)
      {
        MpsEvent evt;
        evt.myType=bsnd;
        MpsTerm *result=term.ApplyBSnd(myPaths[0],evt.myLabel,evt.myChannel);
        env[evt.myChannel].push_back(evt);
        return result;
      }
      else
        return MpsTerm::Error("Error Select application");
      break;
    case brcv:
      if (myPaths.size()==1 && env[myEvent.myChannel].size()>0 &&
          env[myEvent.myChannel].front().myType==bsnd)
      {
        string label = env[myEvent.myChannel].front().myLabel;
        env[myEvent.myChannel].pop_front();
        return term.ApplyBRcv(myPaths[0],label);
      }
      else
        return MpsTerm::Error("Error on Receive application");
      break;
    default:
      return MpsTerm::Error("_ERROR: Unknown step type");
      break;
  }
} // }}}
}
