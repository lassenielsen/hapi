#ifndef MPSPARSER_HPP
#define MPSPARSER_HPP

#include <string>
#include <dpl/symparser.hpp>
#include <apims/mpsterm.hpp>

namespace apims
{

class MpsParser // {{{
{
  public:
    static void DefGtype(dpl::Parser &parser);
    static MpsGlobalType *Gtype(const dpl::parsetree *tree);
    static MpsGlobalType *Gtype(const std::string &str);
    static void DefLtype(dpl::Parser &parser);
    static MpsLocalType *Ltype(const dpl::parsetree *tree);
    static MpsLocalType *Ltype(const std::string &str);
    static void DefMtype(dpl::Parser &parser);
    static MpsMsgType *Mtype(const dpl::parsetree *tree);
    static MpsMsgType *Mtype(const std::string &str);
    static void DefExp(dpl::Parser &parser);
    static MpsExp *Exp(const dpl::parsetree *tree);
    static MpsExp *Exp(const std::string &str);

    // DOCUMENTATION: MpsParser::DefPi {{{
    /*!
     * Add token and type definitions to given parser.
     * @param parser The parser that receives the token and type definitions.
     */
    // }}}
    static void DefPi(dpl::Parser &parser);
    // DOCUMENTATION: MpsParser::Pi {{{
    /*!
     * Creates a MpsTerm object from a parsetree
     * @param tree The parsetree describing a process.
     * @result A MpsTerm object representing the described process.
     */
    // }}}
    static MpsTerm *Pi(const dpl::parsetree *tree);
    // DOCUMENTATION: MpsParser::Pi {{{
    /*!
     * Creates a MpsTerm object from a string
     * @param str The input string.
     * @result A MpsTerm object representing the described process.
     */
    // }}}
    static MpsTerm *Pi(const std::string &str);

    static void FindParticipants(const dpl::parsetree *exp, std::vector<MpsParticipant> &dest);
}; // }}}

}
#endif
