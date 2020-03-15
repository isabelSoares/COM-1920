#ifndef __OG_AST_RETURN_NODE_H__
#define __OG_AST_RETURN_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace og {

  /**
   * Class for describing for-cycle nodes.
   */
  class return_node: public cdk::basic_node {
    cdk::sequence_node *_return_value;

  public:
    inline return_node(int lineno, cdk::sequence_node *return_value) :
        basic_node(lineno), _return_value(return_value) {}

  public:
    inline cdk::sequence_node *return_value() {
      return _return_value;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // og

#endif