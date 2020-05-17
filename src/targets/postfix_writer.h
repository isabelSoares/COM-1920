#ifndef __OG_TARGETS_POSTFIX_WRITER_H__
#define __OG_TARGETS_POSTFIX_WRITER_H__

#include "targets/basic_ast_visitor.h"

#include <set>
#include <stack>
#include <sstream>
#include <cdk/emitters/basic_postfix_emitter.h>

namespace og {

  //!
  //! Traverse syntax tree and generate the corresponding assembly code.
  //!
  class postfix_writer: public basic_ast_visitor {
    bool _inFunction, _inFunctionName, _inFunctionArgs, _inFunctionBody, _inForInit;
    cdk::symbol_table<og::symbol> &_symtab;
    cdk::basic_postfix_emitter &_pf;
    int _lbl;
    std::shared_ptr<og::symbol> _function;

    std::set<std::string> _functions_to_declare;
    std::stack<int> _forIni, _forIncr, _forEnd;

    int _offset;

  public:
    postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<og::symbol> &symtab,
                   cdk::basic_postfix_emitter &pf) :
        basic_ast_visitor(compiler), _inFunction(false), _inFunctionName(false), _inFunctionArgs(
            false), _inFunctionBody(false), _symtab(symtab), _pf(pf), _lbl(0) {
    }

  public:
    ~postfix_writer() {
      os().flush();
    }

  private:
    /** Method used to generate sequential labels. */
    inline std::string mklbl(int lbl) {
      std::ostringstream oss;
      if (lbl < 0)
        oss << ".L" << -lbl;
      else
        oss << "_L" << lbl;
      return oss.str();
    }

  public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end

  };

} // og

#endif
