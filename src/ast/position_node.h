#ifndef __OG_AST_POSITION_NODE_H__
#define __OG_AST_POSITION_NODE_H__

#include <cdk/ast/lvalue_node.h>
#include <cdk/ast/expression_node.h>

namespace og {

  class position_node: public cdk::expression_node {
    cdk::lvalue_node *_lvalue;

  public:
    inline position_node(int lineno, cdk::lvalue_node *lvalue) :
        cdk::expression_node(lineno), _lvalue(lvalue) {}

  public:
    inline cdk::lvalue_node *lvalue() {
      return _lvalue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_position_node(this, level);
    }

  };

} // og

#endif