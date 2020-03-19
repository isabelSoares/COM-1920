#ifndef __OG_AST_WRITE_NODE_H__
#define __OG_AST_WRITE_NODE_H__

#include <cdk/ast/expression_node.h>

namespace og {

  class write_node: public cdk::basic_node {
    cdk::expression_node *_argument;
    bool _new_line;

  public:
    inline write_node(int lineno, cdk::expression_node *argument, bool new_line) :
        cdk::basic_node(lineno), _argument(argument), _new_line(new_line) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    inline bool new_line() {
      return _new_line;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_write_node(this, level);
    }

  };

} // og

#endif
