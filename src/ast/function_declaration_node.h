#ifndef __OG_AST_FUNCTION_DECLARATION_NODE_H__
#define __OG_AST_FUNCTION_DECLARATION_NODE_H__

#include <string>
#include <cdk/types/basic_type.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/typed_node.h>
#include <ast/block_node.h>

namespace og {

  class function_declaration_node: public cdk::typed_node {
    int _qualifier;
    std::string _identifier;
    cdk::sequence_node *_arguments;
    block_node *_block;

  public:
    inline function_declaration_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> *type, std::string &identifier, cdk::sequence_node *arguments, block_node *block) :
        typed_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(arguments), _block(block) {
          if (type != nullptr)
            this->type(*type);
    }

  public:
    inline int qualifier() {
        return _qualifier;
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
