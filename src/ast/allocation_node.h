#ifndef __OG_AST_ALLOCATION_NODE_H__
#define __OG_AST_ALLOCATION_NODE_H__

#include <cdk/ast/expression_node.h>

namespace og {

  class allocation_node: public cdk::expression_node {
    cdk::basic_type *_type;
    int _size;

  public:
    inline allocation_node(int lineno, cdk::basic_type *type, int size) :
        cdk::expression_node(lineno), _type(type), _size(size) {}

  public:
    inline cdk::basic_type *type() {
      return _type;
    }
    inline int size() {
      return _size;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_allocation_node(this, level);
    }

  };

} // og

#endif