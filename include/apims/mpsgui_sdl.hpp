#ifndef MPSGUISDL_HPP
#define MPSGUISDL_HPP

#include <apims/mpsgui.hpp>
#include <apims/mpstype.hpp>
#include <apims/mpsexp.hpp>
#include <sdlwidgets/Widgets.hpp>
#include <string>
#include <vector>
#include <set>

namespace mpsgui
{

/* This is an implementation of the GUI functionality
 * using the SdlWidgets library.
 */

class MpsArgGui : public SdlWidgets::InputString // {{{
{
  public:
    MpsArgGui(const ChoiceArg &arg);
    virtual ~MpsArgGui();

    ChoiceArg GetArg();
    bool ValidArg();

  private:
    ChoiceArg myArg;
}; // }}}

class MpsChoiceGui : public SdlWidgets::VCol // {{{
{
  public:
    MpsChoiceGui(const Choice &rhs);
    virtual ~MpsChoiceGui();
    Choice GetChoice();

  private:
    Choice myChoice;
}; // }}}

class MpsParticipantGui : public SdlWidgets::VCol // {{{
{
  public:
    MpsParticipantGui(int pid);
    virtual ~MpsParticipantGui();

    void ClearChoices();
    void SetChoices(const std::vector<Choice> &choices);
    std::vector<Choice> GetChoices();
}; // }}}

class MpsSessionGui : public SdlWidgets::TabCollection // {{{
{
  public:
    MpsSessionGui(const std::string &name, int maxpid);
    virtual ~MpsSessionGui();

    void ClearChoices();
    void SetChoices(int pid, const std::vector<Choice> &choices);
    std::vector<Choice> GetChoices(int pid);
}; // }}}

class MpsGui : public SdlWidgets::Window // {{{
{
  public:
    MpsGui();
    virtual ~MpsGui();

    void CreateSession(const std::string &session, int maxpid);
    void ClearChoices(const std::string &session);
    void SetChoices(const std::string &session,
                    int pid,
                    const std::vector<Choice> &choices);
    std::vector<Choice> GetChoices(const std::string &session, int pid);

  private:
    std::map<std::string,MpsSessionGui*> mySessions;
}; // }}}

}

#endif
