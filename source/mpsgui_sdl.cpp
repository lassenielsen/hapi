#include <apims/mpsgui_sdl.hpp>
#include <algorithm>
#include <apims/common.hpp>

using namespace SdlWidgets;
using namespace mpsgui;
using namespace apims;
using namespace std;

namespace mpsgui
{

// Static objects
MpsGui maingui;
Lock mainlock("gui_updates");
bool gui_quit=false;
void *LockMain() // {{{
{
  mainlock.LockMe();
  return NULL;
} // }}}
void *_lockmain=LockMain();

// Implementing MPSGUI API
void CreateSession(const std::string &session, int maxpid) // {{{
{
  maingui.CreateSession(session,maxpid);
} // }}}
void ClearChoices(const std::string &session) // {{{
{
  maingui.ClearChoices(session);
} // }}}
void SetChoices(const std::string &session, // {{{
                       int pid,
                       const std::vector<Choice> &choices)
{
  maingui.SetChoices(session,pid,choices);
} // }}}
std::vector<Choice> GetChoices(const std::string &session, int pid) // {{{
{
  return maingui.GetChoices(session,pid);
} // }}}
bool GuiWait() // {{{
{
  mainlock.LockMe();
  return not gui_quit;
} // }}}
bool GuiEnabeled() // {{{
{
  return not gui_quit;
} // }}}
void Show() // {{{
{
  maingui.Show();
} // }}}
void Hide() // {{{
{
  maingui.Hide();
} // }}}

// Argument-Widget
MpsArgGui::MpsArgGui(const ChoiceArg &arg) // {{{
: InputString(arg.value->ToString()),
  myArg(arg)
{
} // }}}
MpsArgGui::~MpsArgGui() // {{{
{
} // }}}
bool MpsArgGui::ValidArg() // {{{
{
  LockMe();
  MpsExp *newValue = MpsExp::Create(GetText());
  MpsGlobalEnv Gamma;
  Gamma.clear();
  MpsLocalEnv Delta;
  Delta.clear();
  MpsMsgEnv Sigma;
  Sigma.clear();
  bool result=false;
  if (newValue->TypeCheck(*myArg.type, Gamma, Delta,Sigma))
  {
    result=true;
    // Update value
    delete myArg.value;
    myArg.value=newValue;
  }
  UnlockMe();
  return result;
} // }}}
ChoiceArg MpsArgGui::GetArg() // {{{
{
  LockMe();
  ChoiceArg arg=myArg;
  UnlockMe();
  return arg;
} // }}}

// Choice-Widget
MpsChoiceGui::MpsChoiceGui(const Choice &rhs) // {{{
: VCol(),
  myChoice(rhs)
{
  //cout << ">MpsChoiceGui::MpsChoiceGui" << endl;
  AddWidget(new CheckBox(new Label(myChoice.name),myChoice.accept),1);
  for (vector<ChoiceArg>::const_iterator it=rhs.args.begin(); it!=rhs.args.end(); ++it)
  {
    //cout << "-MpsChoiceGui::MpsChoiceGui: Adding Arg" << endl;
    AddWidget(new MpsArgGui(*it),1);
  }
  //cout << "<MpsChoiceGui::MpsChoiceGui" << endl;
} // }}}
MpsChoiceGui::~MpsChoiceGui() // {{{
{
} // }}}
Choice MpsChoiceGui::GetChoice() // {{{
{
  LockMe();
  Choice result;
  result.name = myChoice.name;
  result.accept = ((CheckBox*)(*this)[0])->GetValue();
  result.args.clear();
  for (int i=1; i<GetCount(); ++i)
    if (!(((MpsArgGui*)(*this)[i])->ValidArg()))
    {
      result.accept=false;
      ((CheckBox*)(*this)[0])->SetValue(false);
    }
  for (int i=1; i<GetCount(); ++i)
    result.args.push_back(((MpsArgGui*)(*this)[i])->GetArg());
  UnlockMe();
  return result;
} // }}}

// Participant-Widget (Collection of Choices)
MpsParticipantGui::MpsParticipantGui(int pid) // {{{
: VCol()
{
  AddWidget(new Label("Choices:",300,30), 2);
} // }}}
MpsParticipantGui::~MpsParticipantGui() // {{{
{
} // }}}
void MpsParticipantGui::ClearChoices() // {{{
{
  //cout << ">MpsParticipantGui::ClearChoices" << endl;
  while (GetCount()>1)
  {
    RemoveWidget((*this)[1]);
  }
  Update();
  //cout << "<MpsParticipantGui::ClearChoice" << endl;
} // }}}
void MpsParticipantGui::SetChoices(const vector<Choice> &choices) // {{{
{
  //cout << ">MpsParticipantGui::SetChoices" << endl;
  //ClearChoices();
  // FIXME LockMe();
  // Add New Choices
  for (vector<Choice>::const_iterator choice=choices.begin(); choice!=choices.end(); ++choice)
  {
    //cout << "-MpsParticipantGui::SetChoices: Adding Choice" << endl;
    AddWidget(new MpsChoiceGui(*choice),choice->args.size()+1);
  }
  // FIXME UnlockMe();
  //cout << "<MpsParticipantGui::SetChoices" << endl;
} // }}}
vector<Choice> MpsParticipantGui::GetChoices() // {{{
{
  //cout << ">MpsParticipantGui::GetChoices" << endl;
  LockMe();
  vector<Choice> results;
  results.clear();
  for (int i=1; i<GetCount(); ++i)
  {
    MpsChoiceGui *choicegui = (MpsChoiceGui*)(*this)[i];
    results.push_back(choicegui->GetChoice());
  }
  UnlockMe();
  //cout << "<MpsParticipantGui::GetChoices" << endl;
  return results;
} // }}}

// Session-Widget (Collection of Participants)
MpsSessionGui::MpsSessionGui(const string &name, int maxpid) // {{{
: TabCollection(300,500)
{
  for (int i=0; i<maxpid; ++i)
    AddWidget(new MpsParticipantGui(i));
} // }}}
MpsSessionGui::~MpsSessionGui() // {{{
{
} // }}}
void MpsSessionGui::ClearChoices() // {{{
{
  LockMe();
  for (int pid=1; pid<=GetCount(); ++pid)
  {
    MpsParticipantGui *participant=(MpsParticipantGui*)(*this)[pid-1];
    participant->ClearChoices();
  }
  UnlockMe();
} // }}}
void MpsSessionGui::SetChoices(int pid, const vector<Choice> &choices) // {{{
{
  LockMe();
  if (pid <= GetCount() && pid >= 1)
  {
    MpsParticipantGui *participant=(MpsParticipantGui*)(*this)[pid-1];
    participant->SetChoices(choices);
  }
  else
    cerr << "MpsSessionGui::SetChoices: pid out of bounds" << endl;
  UnlockMe();
} // }}}
vector<Choice> MpsSessionGui::GetChoices(int pid) // {{{
{
  vector<Choice> result;
  result.clear();
  LockMe();
  if (pid <= GetCount() && pid >= 1)
  {
    MpsParticipantGui *participant=(MpsParticipantGui*)(*this)[pid-1];
    result=participant->GetChoices();
  }
  else
    cerr << "MpsSessionGui::GetChoices: pid out of bounds" << endl;
  UnlockMe();
  return result;
} // }}}

// Window with list of all sessions
void *choice_update(void *arg) // Click on Continue-Button {{{
{
  mainlock.UnlockMe();
  return NULL;
} // }}}
MpsGui::MpsGui() // {{{
: Window(800,300,new VCol(300,500),"Session List")
{
  VCol *content = (VCol*)GetContent();
  content->AddWidget(new TabCollection(300,500),9);
  Label *lbl = new Label("Continue");
  lbl->AlignCenter();
  content->AddWidget(new Button(lbl,choice_update,NULL),1);
  mySessions.clear();
} // }}}
MpsGui::~MpsGui() // {{{
{
} // }}}
void MpsGui::CreateSession(const string &session, int maxpid) // {{{
{
  LockMe();
  mySessions[session] = new MpsSessionGui(session,maxpid);
  ((TabCollection*)(*(VCol*)GetContent())[0])->AddWidget(mySessions[session]);
  UnlockMe();
} // }}}
void MpsGui::ClearChoices(const string &session) // {{{
{
  LockMe();
  if (mySessions.find(session) != mySessions.end())
    mySessions[session]->ClearChoices();
  UnlockMe();
} // }}}
void MpsGui::SetChoices(const string &session, int pid, const vector<Choice> &choices) // {{{
{
  LockMe();
  mySessions[session]->SetChoices(pid,choices);
  UnlockMe();
} // }}}
vector<Choice> MpsGui::GetChoices(const string &session, int pid) // {{{
{
  vector<Choice> result;
  result.clear();
  LockMe();
  if (mySessions.find(session) == mySessions.end())
    cerr << "Requesting gui-input on closed session: " << session << endl;
  else
    result = mySessions[session]->GetChoices(pid);
  UnlockMe();
  return result;
} // }}}
}
