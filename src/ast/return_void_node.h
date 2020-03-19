#ifndef __OG_AST_RETURN_VOID_NODE_H__
#define __OG_AST_RETURN_VOID_NODE_H__

#include <cdk/ast/basic_node.h>

namespace og {

  class return_void_node: public cdk::basic_node {

  public:
    inline return_void_node(int lineno) :
        basic_node(lineno) {}

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_void_node(this, level);
    }

  };

} // og

#endif