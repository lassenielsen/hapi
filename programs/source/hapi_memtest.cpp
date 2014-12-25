#include <hapi/piexp.hpp>
#include <iostream>

#define REPEATFACTOR 1

using namespace std;
using namespace hapi;
int main(int argc, char **argv)
{
  PiExp *end = PiExp::Create("0");
  cout << "PiEnd:\n  " << end->ToString("  ") << endl;
  PiExp *snd = PiExp::Create("k!x.0");
  cout << "PiSnd:\n  " << snd->ToString("  ") << endl;
  PiExp *rcv = PiExp::Create("k?x.0");
  cout << "PiRcv:\n  " << rcv->ToString("  ") << endl;
  PiExp *sel = PiExp::Create("k!@1.0");
  cout << "PiSelect:\n  " << sel->ToString("  ") << endl;
  PiExp *bra = PiExp::Create("k?{@1: 0, @2: 0, @3: 0}");
  cout << "PiBranch:\n  " << bra->ToString("  ") << endl;
  PiExp *par = PiExp::Create("k?x.0 | k!x.0");
  cout << "PiPar:\n  " << par->ToString("  ") << endl;
  PiExp *mu  = PiExp::Create("mu X. 0");
  cout << "PiMu:\n  " << mu->ToString("  ") << endl;
  PiExp *var = PiExp::Create("X");
  cout << "PiVar:\n  " << var->ToString("  ") << endl;
  PiExp *nu  = PiExp::Create("nu x. 0");
  cout << "PiNu:\n  " << nu->ToString("  ") << endl;

  cout << "Testing Copy:" << flush;
  cout << " PiEnd" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete end->Copy();
  cout << ", PiSnd" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete snd->Copy();
  cout << ", PiRcv" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete rcv->Copy();
  cout << ", PiSelect" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete sel->Copy();
  cout << ", PiBranch" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete bra->Copy();
  cout << ", PiPar" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete par->Copy();
  cout << ", PiMu" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete mu->Copy();
  cout << ", PiVar" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete var->Copy();
  cout << ", PiNu" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete nu->Copy();
  cout << ". Done." << endl;

  cout << "Testing Simplify:" << flush;
  cout << " PiEnd" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete end->Simplify();
  cout << ", PiSnd" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete snd->Simplify();
  cout << ", PiRcv" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete rcv->Simplify();
  cout << ", PiSelect" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete sel->Simplify();
  cout << ", PiBranch" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete bra->Simplify();
  cout << ", PiPar" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete par->Simplify();
  cout << ", PiMu" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete mu->Simplify();
  cout << ", PiVar" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete var->Simplify();
  cout << ", PiNu" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete nu->Simplify();
  cout << ". Done." << endl;

  cout << "Testing Subst:" << flush;
  cout << " PiEnd" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete end->Subst("X",*end);
  cout << ", PiSnd" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete snd->Subst("X",*end);
  cout << ", PiRcv" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete rcv->Subst("X",*end);
  cout << ", PiSelect" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete sel->Subst("X",*end);
  cout << ", PiBranch" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete bra->Subst("X",*end);
  cout << ", PiPar" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete par->Subst("X",*end);
  cout << ", PiMu" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete mu->Subst("X",*end);
  cout << ", PiVar" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete var->Subst("X",*end);
  cout << ", PiNu" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
    delete nu->Subst("X",*end);
  cout << ". Done." << endl;

  cout << "Testing QuickSteps:" << flush;
  vector< pair<PiExp*,PiEvent> >steps;
  cout << " PiEnd" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
  {
    end->QuickSteps(steps);
    while(steps.size()>0)
    {
      delete steps.back().first;
      steps.pop_back();
    }
  }
  cout << ", PiSnd" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
  {
    snd->QuickSteps(steps);
    while(steps.size()>0)
    {
      delete steps.back().first;
      steps.pop_back();
    }
  }
  cout << ", PiRcv" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
  {
    rcv->QuickSteps(steps);
    while(steps.size()>0)
    {
      delete steps.back().first;
      steps.pop_back();
    }
  }
  cout << ", PiSelect" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
  {
    sel->QuickSteps(steps);
    while(steps.size()>0)
    {
      delete steps.back().first;
      steps.pop_back();
    }
  }
  cout << ", PiBranch" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
  {
    bra->QuickSteps(steps);
    while(steps.size()>0)
    {
      delete steps.back().first;
      steps.pop_back();
    }
  }
  cout << ", PiPar" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
  {
    par->QuickSteps(steps);
    while(steps.size()>0)
    {
      delete steps.back().first;
      steps.pop_back();
    }
  }
  cout << ", PiMu" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
  {
    mu->QuickSteps(steps);
    while(steps.size()>0)
    {
      delete steps.back().first;
      steps.pop_back();
    }
  }
  cout << ", PiVar" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
  {
    var->QuickSteps(steps);
    while(steps.size()>0)
    {
      delete steps.back().first;
      steps.pop_back();
    }
  }
  cout << ", PiNu" << flush;
  for (int i=0; i<10 * REPEATFACTOR; ++i)
  {
    nu->QuickSteps(steps);
    while(steps.size()>0)
    {
      delete steps.back().first;
      steps.pop_back();
    }
  }
  cout << ". Done." << endl;

  delete end;
  delete snd;
  delete rcv;
  delete sel;
  delete bra;
  delete par;
  delete mu;
  delete var;
  delete nu;
  usleep(10000000);
  cout << "Test Completed." << endl;
}
