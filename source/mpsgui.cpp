#include <apims/mpsgui.hpp>
#include <apims/mpstype.hpp>

using namespace mpsgui;
using namespace apims;
using namespace std;

// ChoiceArg metods
ChoiceArg::ChoiceArg(const string &name, const MpsMsgType &type, const MpsExp &value) // {{{
{
  this->name = name;
  this->type = type.Copy();
  this->value = value.Copy();
} // }}}
ChoiceArg::ChoiceArg(const ChoiceArg &rhs) // {{{
{
  this->name = rhs.name;
  this->type = rhs.type->Copy();
  this->value = rhs.value->Copy();
} // }}}
ChoiceArg::~ChoiceArg() // {{{
{
  delete type;
  delete value;
} // }}}
ChoiceArg &ChoiceArg::operator=(const ChoiceArg &rhs) // {{{
{
  name = rhs.name;
  delete type;
  type = rhs.type->Copy();
  delete value; 
  value = rhs.value->Copy();
  return *this;
} // }}}

// Choice methods

