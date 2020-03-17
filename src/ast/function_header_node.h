#ifndef __OG_AST_FUNCTION_HEADER_NODE_H__
#define __OG_AST_FUNCTION_HEADER_NODE_H__

#include <string>
#include <cdk/types/basic_type.h>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace og {

  /**
   * Class for describing for-cycle nodes.
   */
  class function_header_node: public cdk::basic_node {
    std::string _qualifier;
    cdk::basic_type *_type;
    std::string _identifier;
    cdk::sequence_node *_arguments;

  public:
    inline function_header_node(int lineno, std::string &qualifier, cdk::basic_type *type, std::string &identifier, cdk::sequence_node *arguments) :
        basic_node(lineno), _qualifier(qualifier), _type(type), _identifier(identifier), _arguments(arguments) {
    }

  public:
    inline std::string &qualifier() {
        return _qualifier;
    }
    inline cdk::basic_type *type() {
        return _type;
    }
    inline std::string &identifier() {
      return _identifier;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_header_node(this, level);
    }

  };

} // og

#endif
