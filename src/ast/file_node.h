#ifndef __OG_AST_FILE_NODE_H__
#define __OG_AST_FILE_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace og {

  /**
   * Class for describing program nodes.
   */
  class file_node: public cdk::basic_node {
    cdk::sequence_node *_statements;

  public:
    inline file_node(int lineno, cdk::sequence_node *statements) :
        cdk::basic_node(lineno), _statements(statements) {
    }

  public:
    inline cdk::sequence_node *statements() {
      return _statements;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_file_node(this, level);
    }

  };

} // og

#endif
