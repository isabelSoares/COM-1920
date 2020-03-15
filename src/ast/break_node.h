#ifndef __OG_AST_BREAK_NODE_H__
#define __OG_AST_BREAK_NODE_H__

#include <cdk/ast/basic_node.h>

namespace og {

  /**
   * Class for describing for-cycle nodes.
   */
  class break_node: public cdk::basic_node {

  public:
    inline break_node(int lineno) :
        basic_node(lineno) {}

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_break_node(this, level);
    }

  };

} // og

#endif