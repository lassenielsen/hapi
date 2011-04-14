#ifndef MPSGUI_HPP
#define MPSGUI_HPP

#include <apims/mpsexp.hpp>
#include <string>
#include <vector>

namespace mpsgui
{

/* This is the GUI methods, used to communicate with
 * different GUI implementations.
 * The API consists of events and queries that can be
 * given to the GUI implementation.
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

struct Choice // {{{
{
  std::string name;
  std::vector<ChoiceArg> args;
  bool accept;
}; // }}}

void CreateSession(const std::string &session, int maxpid);
void ClearChoices(const std::string &session);
void SetChoices(const std::string &session,
                       int pid,
                       const std::vector<Choice> &choices);
std::vector<Choice> GetChoices(const std::string &session, int pid);
bool GuiWait();
bool GuiEnabeled();
void SetValue(const std::string &session,
              int pid,
              const std::string &name,
              const std::string &value);
void Show();
void Hide();
}

#endif
