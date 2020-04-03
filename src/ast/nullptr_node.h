#ifndef __OG_AST_NULLPTR_NODE_H__
#define __OG_AST_NULLPTR_NODE_H__

#include <cdk/ast/expression_node.h>

namespace og {

  class nullptr_node: public virtual literal_node<std::shared_ptr> {
  public:
    nullptr_node(int lineno) :
        literal_node<std::shared_ptr>(lineno, nullptr) {
    }

    void accept(basic_ast_visitor *av, int level) {
      av->do_nullptr_node(this, level);
    }

  };

} // og

#endif
