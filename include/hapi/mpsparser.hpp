#ifndef MPSPARSER_HPP
#define MPSPARSER_HPP

#include <dpl/parsetree.hpp>
#include <hapi/mpsterms.hpp>
#include <hapi/mpstype.hpp>
#include <string>
#include <map>
#include <vector>

namespace hapi
{

  class MpsParser
  {
    public:
      // DOCUMENTATION: Pi {{{
      /*!
       * Creates a MpsTerm object from a parsetree
       * @param tree The parsetree describing a process.
       * @result A MpsTerm object representing the described process.
       */
      // }}}
      static MpsTerm *Pi(const dpl::parsetree *tree);
      static MpsTerm *PiActs(const dpl::parsetree *tree);
      static MpsTerm *PiEActs(const dpl::parsetree *tree);
      static MpsTerm *PiAct(const dpl::parsetree *tree, MpsTerm *succ);
      static MpsTerm *PiEAct(const dpl::parsetree *tree);
      static MpsTerm *PiTerm(const dpl::parsetree *tree);

      static MpsGlobalType *Gtype(const dpl::parsetree *tree);
      static MpsGlobalType *Gact(const dpl::parsetree *tree, MpsGlobalType *succ);
      static MpsGlobalType *Gterm(const dpl::parsetree *tree);
      static void Gbranches(const dpl::parsetree *tree, std::map<std::string,MpsGlobalType*> &dest, std::map<std::string,MpsExp*> &assertions);

      static MpsLocalType *Ltype(const dpl::parsetree *tree);
      static MpsLocalType *Lact(const dpl::parsetree *tree, MpsLocalType *succ);
      static MpsLocalType *Lterm(const dpl::parsetree *tree);
      static void Lbranches(const dpl::parsetree *tree, std::map<std::string,MpsLocalType*> &dest, std::map<std::string,MpsExp*> &assertions);

      static MpsMsgType *Mtype(const dpl::parsetree *tree);
      static MpsMsgType *Stype(const dpl::parsetree *tree);
      static void Stypes(const dpl::parsetree *tree, std::vector<MpsMsgType*> &dest);

      static MpsExp *Exp(const dpl::parsetree *tree);
      static void Exps(const dpl::parsetree *tree, std::vector<MpsExp*> &dest);

      static void Ids(const dpl::parsetree *tree, std::vector<std::string> &dest);

      static MpsChannel Channel(const dpl::parsetree *tree);
      static MpsParticipant Participant(const dpl::parsetree *tree);
      static void Participants(const dpl::parsetree *tree, std::vector<MpsParticipant> &dest);
      static MpsExp *Assertion(const dpl::parsetree *tree);
      static MpsExp *NamedAssertion(const dpl::parsetree *tree, bool &used, std::string &name);
      static void Branch(const dpl::parsetree *tree, std::map<std::string,MpsTerm*> &terms,  std::map<std::string,MpsExp*> &assertions);
      static void Branches(const dpl::parsetree *tree, std::map<std::string,MpsTerm*> &terms, std::map<std::string,MpsExp*> &assertions);
      static void InputBranch(const dpl::parsetree *tree, std::map<std::string,inputbranch> &dest);
      static void InputBranches(const dpl::parsetree *tree, std::map<std::string,inputbranch> &dest);
      static void Args(const dpl::parsetree *tree, std::vector<std::string> &args, std::vector<MpsMsgType*> &types);
      static void DArgs(const dpl::parsetree *tree, std::vector<std::string> &args, std::vector<MpsMsgType*> &types);
      static void DExps(const dpl::parsetree *tree, std::vector<MpsExp*> &exps);
      static bool Mode(const dpl::parsetree *tree, bool d);
      static MpsTerm *Send(const dpl::parsetree *tree, MpsChannel dest, MpsTerm *succ);
      static MpsTerm *Sends(const dpl::parsetree *tree, MpsChannel dest, MpsTerm *succ);
      static MpsTerm *Recv(const dpl::parsetree *tree, MpsChannel dest, MpsTerm *succ);
      static MpsTerm *Recvs(const dpl::parsetree *tree, MpsChannel dest, MpsTerm *succ);
      static void Tvals(const dpl::parsetree *tree, std::vector<MpsExp*> &dest);
      static TypeArg Targ(const dpl::parsetree *tree);
      static void Targ(const dpl::parsetree *tree, std::vector<std::string> &names, std::vector<MpsMsgType*> &types, std::vector<MpsExp*> &values);
      static void Targs(const dpl::parsetree *tree, std::vector<TypeArg> &dest);
      static void Targs(const dpl::parsetree *tree, std::vector<std::string> &names, std::vector<MpsMsgType*> &types, std::vector<MpsExp*> &values);
      static bool Modal(const dpl::parsetree *tree);

      // DOCUMENTATION: Pi {{{
      /*!
       * Creates a MpsTerm object from a string
       * @param str The input string.
       * @result A MpsTerm object representing the described process.
       */
      // }}}
      static MpsTerm *Pi(const std::string &str);
      static MpsGlobalType *Gtype(const std::string &str);
      static MpsLocalType *Ltype(const std::string &str);
      static MpsMsgType *Mtype(const std::string &str);
      static MpsExp *Exp(const std::string &str);
  };

}
#endif
