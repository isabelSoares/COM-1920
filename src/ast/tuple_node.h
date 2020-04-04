#ifndef __OG_AST_TUPLE_NODE_H__
#define __OG_AST_TUPLE_NODE_H__

#include <cdk/ast/expression_node.h>

namespace og {

  class tuple_node: public cdk::expression_node {
      std::vector<cdk::expression_node*> *_expressions;

  public:
    inline tuple_node(int lineno, std::vector<cdk::expression_node*> *expressions) :
        cdk::expression_node(lineno), _expressions(expressions) {
    }

    inline tuple_node(int lineno, std::vector<cdk::expression_node*> *expressions, cdk::expression_node *expression) :
        cdk::expression_node(lineno), _expressions(expressions) {
          _expressions->push_back(expression);
    }

  public:
    inline std::vector<cdk::expression_node*> *expressions() {
      return _expressions;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_tuple_node(this, level);
    }

  };

} // og

#endif
