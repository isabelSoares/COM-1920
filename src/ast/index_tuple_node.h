#ifndef __OG_AST_INDEX_TUPLE_NODE_H__
#define __OG_AST_INDEX_TUPLE_NODE_H__

#include <cdk/ast/lvalue_node.h>

namespace og {

  class index_tuple_node: public cdk::lvalue_node {
    std::string _identifier;
    int _index;

  public:
    inline index_tuple_node(int lineno, std::string &identifier, int index) :
        cdk::lvalue_node(lineno), _identifier(identifier), _index(index) {}

  public:
    inline std::string &identifier() {
      return _identifier;
    }
    inline int index() {
      return _index;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_tuple_node(this, level);
    }

  };

} // og

#endif