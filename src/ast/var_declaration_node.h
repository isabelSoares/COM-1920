#ifndef __OG_AST_VAR_DECLARATION_NODE_H__
#define __OG_AST_VAR_DECLARATION_NODE_H__

#include <string>
#include <vector>
#include <cdk/types/basic_type.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/typed_node.h>

namespace og {

  class var_declaration_node: public cdk::typed_node {
    int _qualifier;
    std::vector<std::string> *_identifiers;
    cdk::expression_node *_expressions;

  public:
    inline var_declaration_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> *type, std::vector<std::string> *identifiers, cdk::expression_node *expressions = nullptr) :
        typed_node(lineno), _qualifier(qualifier), _identifiers(identifiers), _expressions(expressions) {
          this->type(*type);
    }

    inline var_declaration_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> *type, std::string *identifier, cdk::expression_node *expressions = nullptr) :
        typed_node(lineno), _qualifier(qualifier), _expressions(expressions) {
          this->type(*type);
          _identifiers = new std::vector<std::string>();
          _identifiers->push_back(*identifier);
          delete identifier;

    }

  public:
    inline int qualifier() {
        return _qualifier;
    }
    inline std::vector<std::string> *identifiers() {
      return _identifiers;
    }
    inline cdk::expression_node *expressions() {
      return _expressions;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_declaration_node(this, level);
    }

  };

} // og

#endif
