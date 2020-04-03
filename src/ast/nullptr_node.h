#ifndef __OG_AST_NULLPTR_NODE_H__
#define __OG_AST_NULLPTR_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/literal_node.h>

namespace og {

  class nullptr_node: public cdk::literal_node<std::nullptr_t> {
  public:
    inline nullptr_node(int lineno) :
        cdk::literal_node<std::nullptr_t>(lineno, nullptr) {
    }

    void accept(basic_ast_visitor *av, int level) {
      av->do_nullptr_node(this, level);
    }

  };

} // og

#endif
