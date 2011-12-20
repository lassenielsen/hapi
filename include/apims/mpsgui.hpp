/*! \file
 * This file holds a list of method declarations, which defines the API
 * for UI implementations.  The API is very simple and consists of callback
 * functions for registering sessions, choices, and receiving the decissions.
 * Many UIs exist simultaneously, and a UI is identified the session it is in,
 * and the participant id (pid) in that session.
 * @author Lasse Nielsen <lasse.nielsen.dk@gmail.com>
 */
#ifndef MPSGUI_HPP
#define MPSGUI_HPP

#include <apims/mpsexp.hpp>
#include <string>
#include <vector>

/*!
 * The mpsgui namespace is used to contain the user interfaces, and
 * isolate this from the implementation of the language.
 */
namespace mpsgui
{

/*!
 * Represents the arguments of a choice.
 * The argument has a name, a type and a value.
 * The name and type is given by the SetChoices callback,
 * but it should be possible to update the value using the implemented UI.
 * Before updating the value, it should be checked, that the new value has the correct type.
 * When GetChoices is callsd, the updated values should be used in the result.
 */
class ChoiceArg // {{{
{
  public:
    ChoiceArg(const std::string &name, const apims::MpsMsgType &type, const apims::MpsExp &value);
    ChoiceArg(const ChoiceArg &rhs);
    virtual ~ChoiceArg();
    virtual ChoiceArg &operator=(const ChoiceArg &rhs);

    std::string name;
    apims::MpsMsgType *type;
    apims::MpsExp *value;
}; // }}}

/*!
 * Represents a possible choice.
 * The choice has a name, a list of arguments, and a state storing if the choice is accepted.
 * It should be possible to update the values for each argument, and accept or reject choices using the UI.
 * When GetChoices is called, the accept state should be true only if the choice has been accepted.
 */
struct Choice // {{{
{
  std::string name;
  std::vector<ChoiceArg> args;
  bool accept;
}; // }}}

/*!
 * CreateSession is used to create UIs.
 * A UI is identified by a session and a participant id (pid).
 * This means that Create session creates maxpid new UIs, one for
 * each participant of the session.
 * This means that calls to
 * ClearChoices, SetChoices, GetChoices and SetValue
 * may occur in the future with the given session id.
 * The calls can use pids between 1 and maxpid inclusive.
 * @param session id to specify which session is created.
 * @param maxpid specifies how many participants can use the session.
 */
void CreateSession(const std::string &session, int maxpid);
/*!
 * ClearChoices is used to remove all choices for the UIs of
 * all participants of the given session.
 * @param session the session of the UIs to clear.
 */
void ClearChoices(const std::string &session);
/*!
 * SetChoices is used to overwrite the list of choices for UI.
 * @param session the session of the UI.
 * @param pid the participant of the UI.
 * @param choices The new choices for the specified UI.
 */
void SetChoices(const std::string &session,
                int pid,
                const std::vector<Choice> &choices);
/*!
 * GetChoices is used to send the interpretor a snapshot of the choices mad in a UI.
 * @param session the session of the UI.
 * @param pid the participant of the UI.
 * @result A snapshot of the choices in the UI.
 */
std::vector<Choice> GetChoices(const std::string &session, int pid);
/*!
 * GuiWait is called when the interpretor wants to wait for an event from the
 * UI. The call should not return until a choice has been updated using the UI.
 * @result Returns true if there has been an event, and false if there will
 * never be any events.
 */
bool GuiWait();
/*!
 * @result Returns true if the UIs are enabeled, and false otherwise.
 */
bool GuiEnabeled();
/*!
 * SetValue is used to add information to a UI.
 * The information consists of a name->value binding, and SetValue should
 * update this binding, and the result should be reflected in the UI.
 */
void SetValue(const std::string &session,
              int pid,
              const std::string &name,
              const std::string &value);
/*!
 * Show should display the UIs. If some UI windows has been closed the should be reopened.
 */
void Show();
/*!
 * Hide should hide the UIs. If some UI windows are open the should be closed or minimized.
 */
void Hide();
}

#endif
