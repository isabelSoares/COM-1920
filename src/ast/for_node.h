#ifndef __OG_AST_FOR_NODE_H__
#define __OG_AST_FOR_NODE_H__

#include <cdk/ast/sequence_node.h>

namespace og {

  /**
   * Class for describing for-cycle nodes.
   */
  class for_node: public cdk::basic_node {
    cdk::sequence_node *_init;
    cdk::sequence_node *_condition;
    cdk::sequence_node *_increment;
    cdk::basic_node *_block;

  public:
    inline for_node(int lineno, cdk::sequence_node *init, cdk::sequence_node *condition, cdk::sequence_node *increment, cdk::basic_node *block) :
        basic_node(lineno), _init(init), _condition(condition), _increment(increment), _block(block) {
    }

  public:
    inline cdk::sequence_node *init() {
      return _init;
    }
    inline cdk::sequence_node *condition() {
      return _condition;
    }
    inline cdk::sequence_node *increment() {
      return _increment;
    }
    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_for_node(this, level);
    }

  };

} // og

#endif
