#ifndef __OG_AST_VAR_DECLARATION_NODE_H__
#define __OG_AST_VAR_DECLARATION_NODE_H__

#include <string>
#include <vector>
#include <cdk/types/basic_type.h>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <ast/block_node.h>

namespace og {

  class var_declaration_node: public cdk::basic_node {
    std::string _qualifier;
    cdk::basic_type *_type;
    std::vector<std::string> _identifiers;
    std::vector<cdk::expression_node> *_expressions;

  public:
    inline var_declaration_node(int lineno, std::string &qualifier, cdk::basic_type *type, std::vector<std::string> identifiers, std::vector<cdk::expression_node> *expressions) :
        basic_node(lineno), _qualifier(qualifier), _type(type), _identifiers(identifiers), _expressions(expressions) {
    }
    inline var_declaration_node(int lineno, std::string &qualifier, cdk::basic_type *type, std::vector<std::string> identifiers) :
        basic_node(lineno), _qualifier(qualifier), _type(type), _identifiers(identifiers) {
    }

  public:
    inline std::string &qualifier() {
        return _qualifier;
    }
    inline cdk::basic_type *type() {
        return _type;
    }
    inline std::vector<std::string> identifiers() {
      return _identifiers;
    }
    inline std::vector<cdk::expression_node> *expressions() {
      return _expressions;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_declaration_node(this, level);
    }

  };

} // og

#endif
