#ifndef __OG_AST_FUNCTION_DECLARATION_NODE_H__
#define __OG_AST_FUNCTION_DECLARATION_NODE_H__

#include <string>
#include <cdk/types/basic_type.h>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <ast/block_node.h>

namespace og {

  class function_declaration_node: public cdk::basic_node {
    std::string _qualifier;
    cdk::basic_type *_type;
    std::string _identifier;
    cdk::sequence_node *_arguments;
    block_node *_block;

  public:
    inline function_declaration_node(int lineno, std::string &qualifier, cdk::basic_type *type, std::string &identifier, cdk::sequence_node *arguments, block_node *block) :
        basic_node(lineno), _qualifier(qualifier), _type(type), _identifier(identifier), _arguments(arguments), _block(block) {
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
    inline block_node *block() {
        return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // og

#endif
