#include <apims/mpsterm.hpp>
#include <iostream>

using namespace std;
using namespace apims;
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    cerr << "Syntax: ./eval \"<exp>\"" << endl;
    return -1;
  }
  MpsTerm *current = NULL;
  MpsTerm *next = MpsTerm::Create(argv[1]);
  MpsEnv env;
  while (next != NULL)
  {
    if (current != NULL)
      delete current;
    current = next;
    next = NULL;
    cout << "Current Term:\n -- " << EnvToString(env) << "\n -- " << current->ToString(" -- ") << endl;
    vector<MpsTerm*> steps;
    vector<MpsEnv> envs;
    vector<MpsEvent> events;
    steps.clear();
    envs.clear();
    events.clear();
    current->Steps(env,steps,envs,events);
    for (int i=0; i<steps.size(); ++i)
    {
      cout << "Option " << i << ":" << endl;
      cout << "  Result: " << EnvToString(envs[i]) << "\n          " << steps[i]->Simplify()->ToString("          ") << endl;
    }
    cout << "Option " << steps.size() << ": quit." << endl;
    cout << "Select: " << flush;
    int i;
    cin >> i;
    if (i < steps.size())
    {
      next = steps[i];
      env = envs[i];
    }
    // FIXME: clean up
  }
}
