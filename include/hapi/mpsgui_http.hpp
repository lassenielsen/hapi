#ifndef MPSGUISDL_HPP
#define MPSGUISDL_HPP

#include <hapi/mpsgui.hpp>
#include <hapi/mpstype.hpp>
#include <hapi/mpsexp.hpp>
#include <string>
#include <vector>
#include <set>

namespace mpsgui
{
/* This is an implementation of the GUI methods
 * as an HTTP server using SDL_Net.
 */

class MpsGuiParticipant // {{{
{
  public:
    MpsGuiParticipant(const std::string &session, int pid);
    virtual ~MpsGuiParticipant();

    void ClearChoices();
    void SetChoices(const std::vector<Choice> &choices);
    std::vector<Choice> GetChoices();
    void SetValue(const std::string &name, const std::string &value);
    std::string GetValue(const std::string &name);
    void UpdateChoice(const std::map<std::string,std::string> &data);
    std::string Render();

  private:
    void AcceptChoice(const std::string &choice);
    void RejectChoice(const std::string &choice);
    bool SetChoiceValue(const std::string &choice, const std::string &arg, const std::string &value);
    void ClearBoolChoices(const std::string &choice);

    std::vector<Choice> myChoices;
    std::map<std::string,std::string> myValues;
    int myPid;
    std::string mySession;
}; // }}}

class MpsGuiSession // {{{
{
  public:
    MpsGuiSession(const std::string &name, int maxpid);
    virtual ~MpsGuiSession();

    void ClearChoices();
    void SetChoices(int pid, const std::vector<Choice> &choices);
    std::vector<Choice> GetChoices(int pid);
    MpsGuiParticipant *GetParticipant(int pid);
    void SetValue(int pid,
                  const std::string &name,
                  const std::string &value);
    std::string GetValue(int pid,
                        const std::string &name);
    int GetMaxpid();
    std::string Render();

  private:
    std::string myName;
    int myMaxpid;
    std::vector<MpsGuiParticipant*> myParticipants;
}; // }}}

class MpsGuiServer // {{{
{
  public:
    MpsGuiServer();
    virtual ~MpsGuiServer();

    void RegisterSession(const std::string &session, int maxpid);
    void ClearChoices(const std::string &session);
    void SetChoices(const std::string &session,
                    int pid,
                    const std::vector<Choice> &choices);
    std::vector<Choice> GetChoices(const std::string &session,int pid);
    void SetValue(const std::string &session,
                  int pid,
                  const std::string &name,
                  const std::string &value);
    std::string Render();
    std::string RenderABC();
    std::string RenderABCUser(const std::string &user);
    std::string RenderABCActivity(const std::string &user, const std::string &activity);
    MpsGuiSession *GetSession(const std::string &name);
  private:
    std::map<std::string,MpsGuiSession*> mySessions;
}; // }}}

}

#endif
