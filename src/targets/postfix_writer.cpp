#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/frame_size_calculator.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

#include "og_parser.tab.h"

//---------------------------------------------------------------------------

void og::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.DOUBLE(node->value()); // load number to the stack
  } else {
    _pf.SDOUBLE(node->value());    // double is on the DATA segment
  }
}
void og::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl1;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
}
void og::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.INT(node->value()); // integer literal is on the stack: push an integer
  } else {
    _pf.SINT(node->value()); // integer literal is on the DATA segment
  }
}

void og::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  if (_function) {
    // local variable initializer
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  } else {
    // global variable initializer
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}
void og::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void og::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void og::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void og::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void og::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void og::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void og::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void og::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void og::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void og::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  const std::string &id = node->name();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);
  if (symbol->global()) {
    os() << "        ;; accessing global variable " << std::endl;
    _pf.ADDR(symbol->name());
  }
  else {
    os() << "        ;; accessing local variable " << std::endl;
    _pf.LOCAL(symbol->offset());
  }
}

void og::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  os() << "        ;; ldint " << std::endl;
  _pf.LDINT(); // depends on type size
  os() << "        ;; ldint close" << std::endl;
}

void og::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // determine the new value
  _pf.DUP32();
  if (new_symbol() == nullptr) {
    node->lvalue()->accept(this, lvl); // where to store the value
  } else {
    _pf.DATA(); // variables are all global and live in DATA
    _pf.ALIGN(); // make sure we are aligned
    _pf.LABEL(new_symbol()->name()); // name variable location
    reset_new_symbol();
    _pf.SINT(0); // initialize it to 0 (zero)
    _pf.TEXT(); // return to the TEXT segment
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }
  _pf.STINT(); // store the value at address
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_function_declaration_node(og::function_declaration_node * const node, int lvl) {
  // Note that Simple doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.

  // generate the main function (RTS mandates that its name be "_main")
  if (_inFunctionBody || _inFunctionArgs) {
    //error(node->lineno(), "cannot define function in body or in arguments");
    return;
  }

  ASSERT_SAFE_EXPRESSIONS;

  if (!new_symbol()) return;

  if (node->block()){
    _function = new_symbol();
    _functions_to_declare.erase(_function->name());
  }
  else {
    std::shared_ptr<og::symbol> function = new_symbol();
    _functions_to_declare.insert(function->name());
  }
  reset_new_symbol();

  if (node->block()){

    _offset = 8; // prepare for arguments (4: remember to account for return address)
    _symtab.push(); // scope of args
    if (node->arguments()) {
      _inFunctionArgs = true; //FIXME really needed?
      for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
        cdk::basic_node *arg = node->arguments()->node(ix);
        if (arg == nullptr) break; // this means an empty sequence of arguments
        arg->accept(this, 0); // the function symbol is at the top of the stack
      }
      _inFunctionArgs = false; //FIXME really needed?
    }

    _pf.TEXT();
    _pf.ALIGN();
    if (node->qualifier() == tPUBLIC) _pf.GLOBAL(_function->name(), _pf.FUNC());
    _pf.LABEL(_function->name());

    // compute stack size to be reserved for local variables
    frame_size_calculator lsc(_compiler, _symtab);
    node->accept(&lsc, lvl);
    _pf.ENTER(lsc.localsize()); // total stack size reserved for local variables

    // the following flag is a slight hack: it won't work with nested functions
    _inFunctionBody = true;
    // prepare for local variables but remember the return value (first below fp)
    _offset = -_function->type()->size();
    os() << "        ;; before body " << std::endl;
    node->block()->accept(this, lvl + 4); // block has its own scope
    os() << "        ;; after body " << std::endl;
    _inFunctionBody = false;
    _symtab.pop(); // scope of arguments

    if (node->identifier() == "og") {
      // declare external functions
      for (std::string s : _functions_to_declare)
        _pf.EXTERN(s);
    }
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_evaluation_node(og::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->is_typed(cdk::TYPE_STRING)) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void og::postfix_writer::do_write_node(og::write_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
     _functions_to_declare.insert("printi");
    os() << "               ; call printi " << std::endl;
    _pf.CALL("printi");
    os() << "               ; call trash " << std::endl;
    _pf.TRASH(4); // delete the printed value
    os() << "               ; finish write int " << std::endl;
  } else if (node->argument()->is_typed(cdk::TYPE_STRING)) {
    _functions_to_declare.insert("prints");
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }  else if (node->argument()->is_typed(cdk::TYPE_DOUBLE)) {
    _functions_to_declare.insert("printd");
    _pf.CALL("printd");
    _pf.TRASH(8); // delete the printed double value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }

  if (node->new_line()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println"); // print a newline
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_input_node(og::input_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.LDFVAL32();
  _pf.STINT();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_for_node(og::for_node * const node, int lvl) {
  _forIni.push(++_lbl); // after init, before body
  _forIncr.push(++_lbl);// after intruction
  _forEnd.push(++_lbl);// after for

  os() << "        ;; FOR initializer" << std::endl;
  // initialize: be careful with variable declarations:
  // they are done here, but the space is occupied in the function
  _inForInit = true;// remember this for local declarations

  // initialize
  if (node->init_seq()) node->init_seq()->accept(this, lvl + 2);  
  if (node->init_exp()) node->init_exp()->accept(this, lvl + 2);  


  os() << "        ;; FOR test" << std::endl;
  // prepare to test
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forIni.top()));
  node->condition()->accept(this, lvl + 2);
  _pf.JZ(mklbl(_forEnd.top()));

  os() << "        ;; FOR instruction" << std::endl;
  // execute instruction
  node->block()->accept(this, lvl + 2);

  os() << "        ;; FOR increment" << std::endl;
  // prepare to increment
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forIncr.top()));
  node->increment()->accept(this, lvl + 2);
  
  os() << "        ;; FOR jump to test" << std::endl;
  _pf.JMP(mklbl(_forIni.top()));

  os() << "        ;; FOR end" << std::endl;
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forEnd.top()));

  _inForInit = false;// remember this for local declarations

  _forIni.pop();
  _forIncr.pop();
  _forEnd.pop();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_node(og::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_else_node(og::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_block_node(og::block_node *const node, int lvl) {
  _symtab.push(); // for block-local vars
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}
void og::postfix_writer::do_break_node(og::break_node *const node, int lvl) {
  if (_forIni.size() != 0) {
    _pf.JMP(mklbl(_forEnd.top())); // jump to for end
  } else {}
    //error(node->lineno(), "'break' outside 'for'");
}
void og::postfix_writer::do_continue_node(og::continue_node *const node, int lvl) {
  if (_forIni.size() != 0) {
    _pf.JMP(mklbl(_forIncr.top())); // jump to next cycle
  } else {}
    //error(node->lineno(), "'restart' outside 'for'");
}
void og::postfix_writer::do_function_invocation_node(og::function_invocation_node *const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_return_node(og::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  // should not reach here without returning a value (if not void)
  if (!_function->is_typed(cdk::TYPE_VOID)) {
    node->return_value()->accept(this, lvl + 2);

    if (_function->is_typed(cdk::TYPE_INT) || _function->is_typed(cdk::TYPE_STRING) || _function->is_typed(cdk::TYPE_POINTER)) {
      _pf.STFVAL32();
    } else if (_function->is_typed(cdk::TYPE_DOUBLE)) {
      if (node->return_value()->is_typed(cdk::TYPE_INT))
        _pf.I2D();
      _pf.STFVAL64();
    } else {
      std::cerr << node->lineno() << ": should not happen: unknown return type" << std::endl;
    }
  }
  _pf.LEAVE();
  _pf.RET();
}
void og::postfix_writer::do_allocation_node(og::allocation_node *const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_index_node(og::index_node *const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_index_tuple_node(og::index_tuple_node *const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_position_node(og::position_node *const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_var_declaration_node(og::var_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto id = node->identifiers()->at(0);
  int offset = 0, typesize = node->type()->size();

  std::cout << "ARG: " << id << ", " << typesize << std::endl;
  if (_inFunctionBody) {
    std::cout << "Function Body" << std::endl;
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    std::cout << "Function Args" << std::endl;
    offset = _offset;
    _offset += typesize;
  } else {
    std::cout << "Global" << std::endl;
    offset = 0; // global variable
  }

  std::shared_ptr<og::symbol> symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (_inFunctionBody) {
    // if we are dealing with local variables, then no action is needed
    // unless an initializer exists
    if (node->expressions()) {
      node->expressions()->accept(this, lvl);
      if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_POINTER)) {
        _pf.LOCAL(symbol->offset());
        _pf.STINT();
      } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
        _pf.LOCAL(symbol->offset());
        _pf.STDOUBLE();
      } else {
        std::cerr << "cannot initialize" << std::endl;
      }
    }
  } else {
    if (!_function) {
      if (node->expressions() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(id);
        _pf.SALLOC(typesize);
      } else {
        if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER)) {
          os() << "        ;; variable data " << std::endl;
          _pf.DATA();
          os() << "        ;; variable align " << std::endl;
          _pf.ALIGN();
          os() << "        ;; variable label " << std::endl;
          _pf.LABEL(id);

          if (node->is_typed(cdk::TYPE_INT)) {
            os() << "        ;; type int " << std::endl;
            node->expressions()->accept(this, lvl);
          } else if (node->is_typed(cdk::TYPE_POINTER)) {
            node->expressions()->accept(this, lvl);
          } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
            if (node->expressions()->is_typed(cdk::TYPE_DOUBLE)) {
              node->expressions()->accept(this, lvl);
            } else if (node->expressions()->is_typed(cdk::TYPE_INT)) {
              cdk::integer_node *dclini = dynamic_cast<cdk::integer_node *>(node->expressions());
              cdk::double_node ddi(dclini->lineno(), dclini->value());
              ddi.accept(this, lvl);
            } else {
              std::cerr << node->lineno() << ": '" << id << "' has bad initializer for real value\n";
              //_errors = true;
            }
          }
        } else if (node->is_typed(cdk::TYPE_STRING)) {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);
          node->expressions()->accept(this, lvl); 
        } else {
          std::cerr << node->lineno() << ": '" << id << "' has unexpected initializer\n";
          //_errors = true;
        }

      }

    }
  }
}
void og::postfix_writer::do_tuple_node(og::tuple_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    os() << "        ;; accessing tuple " << std::endl;
    node->node(i)->accept(this, lvl);
  }
}
void og::postfix_writer::do_identity_node(og::identity_node *const node, int lvl) {   
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}
void og::postfix_writer::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_sizeof_node(og::sizeof_node *const node, int lvl) {
  // EMPTY
}