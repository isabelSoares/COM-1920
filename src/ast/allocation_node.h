#ifndef __OG_AST_ALLOCATION_NODE_H__
#define __OG_AST_ALLOCATION_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/unary_operation_node.h>

namespace og {

  class allocation_node: public cdk::unary_operation_node {

  public:
    inline allocation_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_operation_node(lineno, argument) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_allocation_node(this, level);
    }

  };

} // og

#endif