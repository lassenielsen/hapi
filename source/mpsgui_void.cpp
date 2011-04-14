#include <apims/mpsgui.hpp>

using namespace std;

namespace mpsgui
{

void CreateSession(const std::string &session, int maxpid)
{}
void ClearChoices(const std::string &session)
{}
void SetChoices(const std::string &session,
                       int pid,
                       const std::vector<Choice> &choices)
{}
std::vector<Choice> GetChoices(const std::string &session, int pid)
{ return vector<Choice>();
}
bool GuiWait()
{ return false; }
bool GuiEnabeled()
{ return false; }
void SetValue(const std::string &session,
              int pid,
              const std::string &name,
              const std::string &value)
{}
void Show()
{}
void Hide()
{}
}
