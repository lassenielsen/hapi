#include <unistd.h>
#include <gmp.h>
using namespace std;

int def(int val)
{
  if (val<=0)
    return 0;
  def(val-1);
  return 0;
}

int main()
{
  int f=fork();
  if (f>0)
  {
    def(100000);
    return 0;
  }
  else if (f==0)
  {
    def(100000);
    return 0;
  }
  else throw "Error during fork!";
  return 0;
}
