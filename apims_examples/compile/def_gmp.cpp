#include <unistd.h>
#include <gmp.h>
using namespace std;

int def(mpz_t &val)
{
  if (mpz_cmp_si(val,0)<=0)
    return 0;
  mpz_t newval;
  mpz_sub_ui(newval,val,1);
  def(newval);
  return 0;
}

int main()
{
  int f=fork();
  if (f>0)
  {
    mpz_t val;
    mpz_init_set_str(val,"100000",10);
    def(val);
    return 0;
  }
  else if (f==0)
  {
    mpz_t val;
    mpz_init_set_str(val,"100000",10);
    def(val);
    return 0;
  }
  else throw "Error during fork!";
  return 0;
}
