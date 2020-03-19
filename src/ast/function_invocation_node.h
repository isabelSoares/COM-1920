#ifndef __OG_AST_FUNCTION_INVOCATION_NODE_H__
#define __OG_AST_FUNCTION_INVOCATION_NODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace og {

  class function_invocation_node: public cdk::expression_node {
    std::string _identifier;
    cdk::sequence_node *_arguments;

  public:
    inline function_invocation_node(int lineno, std::string &identifier) :
        cdk::expression_node(lineno), _identifier(identifier) {
    }
    inline function_invocation_node(int lineno, std::string &identifier, cdk::sequence_node *arguments) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(arguments) {
    }

  public:
    inline std::string &identifier() {
      return _identifier;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_invocation_node(this, level);
    }

  };

} // og

#endif
