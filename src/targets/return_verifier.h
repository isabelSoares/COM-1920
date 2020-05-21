#ifndef __OG_TARGET_RETURN_VERIFIER_H__
#define __OG_TARGET_RETURN_VERIFIER_H__

#include "targets/basic_ast_visitor.h"

#include <string>
#include <iostream>
#include <sstream>
#include <stack>
#include <cdk/symbol_table.h>
#include "targets/symbol.h"

namespace og {

  class return_verifier: public basic_ast_visitor {
    cdk::symbol_table<og::symbol> &_symtab;

    bool _return_everywhere;
    std::shared_ptr<cdk::basic_type> _return_type;

  public:
    return_verifier(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<og::symbol> &symtab) :
        basic_ast_visitor(compiler), _symtab(symtab), _return_everywhere(false) {
    }

  public:
    ~return_verifier();

  public:
    bool return_everywhere() const {
      return _return_everywhere;
    }
    bool return_everywhere(bool value) {
      return _return_everywhere = value;
    }

    std::shared_ptr<cdk::basic_type> return_type() const {
      return _return_type;
    }
    std::shared_ptr<cdk::basic_type> return_type(std::shared_ptr<cdk::basic_type> type) {
      return _return_type = type;
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
