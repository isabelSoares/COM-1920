#ifndef __OG_AST_DIMENSION_NODE_H__
#define __OG_AST_DIMENSION_NODE_H__

#include <cdk/ast/expression_node.h>

namespace og {

  /**
   * Class for describing for-cycle nodes.
   */
  class dimension_node: public cdk::expression_node {
    cdk::expression_node *_expression;

  public:
    inline dimension_node(int lineno, cdk::expression_node *expression) :
        cdk::expression_node(lineno), _expression(expression) {}

  public:
    inline cdk::expression_node *expression() {
      return _expression;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_dimension_node(this, level);
    }

  };

} // og

#endif