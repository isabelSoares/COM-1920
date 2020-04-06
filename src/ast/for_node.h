#ifndef __OG_AST_FOR_NODE_H__
#define __OG_AST_FOR_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>

namespace og {

  class for_node: public cdk::basic_node {
    cdk::sequence_node *_init_seq;
    cdk::expression_node *_init_exp;
    cdk::expression_node *_condition;
    cdk::expression_node *_increment;
    cdk::basic_node *_block;

  public:
    inline for_node(int lineno, cdk::sequence_node *init_seq, cdk::expression_node *init_exp, cdk::expression_node *condition, cdk::expression_node *increment, cdk::basic_node *block) :
        basic_node(lineno), _init_seq(init_seq), _init_exp(init_exp), _condition(condition), _increment(increment), _block(block) {
    }

  public:
    inline cdk::sequence_node *init_seq() {
      return _init_seq;
    }
    inline cdk::expression_node *init_exp() {
      return _init_exp;
    }
    inline cdk::expression_node *condition() {
      return _condition;
    }
    inline cdk::expression_node *increment() {
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
