#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/sizeof_calculator.h"
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
  if (_inFunctionBody) _pf.DOUBLE(node->value());
  else _pf.SDOUBLE(node->value());
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
  ASSERT_SAFE_EXPRESSIONS;

  int lbl1;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl1 = ++_lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody) _pf.INT(node->value());
  else _pf.SINT(node->value());
}

void og::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  _pf.RODATA();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  _pf.SSTRING(node->value());

  if (_inFunctionBody) {
    // local variable initializer
    _pf.TEXT();
    _pf.ALIGN();
    _pf.ADDR(mklbl(lbl1));
  } else {
    // global variable initializer
    _pf.DATA();
    _pf.ALIGN();
    _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.NEG();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  node->right()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE)) _pf.DADD();
  else _pf.ADD();
}
void og::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  node->right()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE)) _pf.DSUB();
  else _pf.SUB();
}
void og::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  node->right()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE)) _pf.DMUL();
  else _pf.MUL();
}
void og::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  node->right()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) _pf.I2D();
  else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE)) _pf.DDIV();
  else _pf.DIV();
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
  if (symbol->global()) _pf.ADDR(symbol->name());
  else _pf.LOCAL(symbol->offset());
}

void og::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE)) _pf.LDDOUBLE();
  else if (node->is_typed(cdk::TYPE_STRUCT)) {
    // TUPLES
    auto struct_type = cdk::structured_type_cast(node->type());
    _pf.INT(node->type()->size());
    _pf.ADD();
    for (size_t actual = struct_type->length(); actual > 0; actual--) {
      
      auto component = struct_type->component(actual - 1);
      if (component->name() == cdk::TYPE_DOUBLE) {
        _pf.INT(- component->size());
        _pf.ADD();
        _pf.I2D();
        _pf.DUP64();
        _pf.D2I();
        _pf.LDDOUBLE();
        _pf.SWAP64();
        _pf.D2I();
      } else {
        _pf.INT(- component->size());
        _pf.ADD();
        _pf.DUP32();
        _pf.LDINT();
        _pf.SWAP32();
      }
    }

    _pf.TRASH(4);

  } else {
    // integers, pointers, and strings
    _pf.LDINT();
  }
}

void og::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    if (node->rvalue()->is_typed(cdk::TYPE_INT)) _pf.I2D();
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }


  if (new_symbol() != nullptr) {
    _pf.DATA();
    _pf.ALIGN();
    _pf.LABEL(new_symbol()->name());
    reset_new_symbol();
    _pf.SINT(0);
    _pf.TEXT();
  }

  node->lvalue()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE)) _pf.STDOUBLE();
  else _pf.STINT();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_function_declaration_node(og::function_declaration_node * const node, int lvl) {
  if (_inFunctionBody ||  _inFunctionArgs) {
    // No definition of functions inside functions
    return;
  }

  ASSERT_SAFE_EXPRESSIONS;

  if (node->block()){
    if (!new_symbol()) _function = _symtab.find(node->identifier());
    else _function = new_symbol();
    _functions_to_declare.erase(_function->name());
  }
  else {
    std::shared_ptr<og::symbol> function = new_symbol();
    _functions_to_declare.insert(function->name());
  }
  reset_new_symbol();

  if (node->block()){
    _functionHasReturn = false;
    _offset = 8;
    _symtab.push();

    if (node->arguments()) {
      _inFunctionArgs = true;
      std::vector<std::shared_ptr<cdk::basic_type>> args;
      for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
        og::var_declaration_node *arg = (og::var_declaration_node*)node->arguments()->node(ix);
        if (arg == nullptr) break;
        arg->accept(this, 0);
        args.push_back(arg->type());
      }
      _function->arguments(args);
      _inFunctionArgs = false;
    }
    
    _pf.TEXT();
    _pf.ALIGN();
    if (node->qualifier() == tPUBLIC) _pf.GLOBAL(_function->name(), _pf.FUNC());
    _pf.LABEL(_function->name());

    
    frame_size_calculator lsc(_compiler, _symtab);
    node->accept(&lsc, lvl);
    _pf.ENTER(lsc.localsize());
    
    _inFunctionBody = true;
    _offset = -_function->type()->size();
    node->block()->accept(this, lvl + 4);
    if (!_functionHasReturn) {
      _pf.LEAVE();
      _pf.RET();
    }
    _inFunctionBody = false;
    _symtab.pop();

    if (node->identifier() == "og") {
      for (std::string s : _functions_to_declare)
        _pf.EXTERN(s);
    }
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_evaluation_node(og::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.TRASH(node->argument()->type()->size());
}

void og::postfix_writer::do_write_node(og::write_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
     _functions_to_declare.insert("printi");
    _pf.CALL("printi");
    _pf.TRASH(4);
  } else if (node->argument()->is_typed(cdk::TYPE_STRING)) {
    _functions_to_declare.insert("prints");
    _pf.CALL("prints");
    _pf.TRASH(4);
  }  else if (node->argument()->is_typed(cdk::TYPE_DOUBLE)) {
    _functions_to_declare.insert("printd");
    _pf.CALL("printd");
    _pf.TRASH(8);
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN! (write)" << std::endl;
    exit(1);
  }

  if (node->new_line()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println");
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_input_node(og::input_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _functions_to_declare.insert("readd");
    _pf.CALL("readd");
    _pf.LDFVAL64();
    _pf.STDOUBLE();
  } else {
    _functions_to_declare.insert("readi");
  _pf.CALL("readi");
  _pf.LDFVAL32();
  _pf.STINT();
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_for_node(og::for_node * const node, int lvl) {
  _forIni.push(++_lbl);
  _forIncr.push(++_lbl);
  _forEnd.push(++_lbl);

  _inForInit = true;
  if (node->init_seq()) node->init_seq()->accept(this, lvl + 2);  
  if (node->init_exp()) node->init_exp()->accept(this, lvl + 2);  

  // prepare to test
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forIni.top()));
  node->condition()->accept(this, lvl + 2);
  _pf.JZ(mklbl(_forEnd.top()));

  // execute instruction
  node->block()->accept(this, lvl + 2);

  // prepare to increment
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forIncr.top()));
  node->increment()->accept(this, lvl + 2);
  

  _pf.JMP(mklbl(_forIni.top()));

  
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forEnd.top()));

  _inForInit = false;

  _forIni.pop();
  _forIncr.pop();
  _forEnd.pop();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_node(og::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  bool prev = _functionHasReturn;

  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));

  _functionHasReturn = prev;
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_else_node(og::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  bool prev = _functionHasReturn;
  _functionHasReturn = false;

  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  bool ifHasReturn = _functionHasReturn;

  _functionHasReturn = false;
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
  bool elseHasReturn = _functionHasReturn;

  _functionHasReturn = prev || (ifHasReturn && elseHasReturn);
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_block_node(og::block_node *const node, int lvl) {
  _symtab.push(); // block context
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}
void og::postfix_writer::do_break_node(og::break_node *const node, int lvl) {
  if (_forIni.size() != 0) _pf.JMP(mklbl(_forEnd.top()));
  else std::cerr << node->lineno() << "'break' outside 'for'" << std::endl;
}
void og::postfix_writer::do_continue_node(og::continue_node *const node, int lvl) {
  if (_forIni.size() != 0) _pf.JMP(mklbl(_forIncr.top()));
  else std::cerr << node->lineno() << "'restart' outside 'for'" << std::endl;
}
void og::postfix_writer::do_function_invocation_node(og::function_invocation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<og::symbol> symbol = _symtab.find(node->identifier());
  std::vector<std::shared_ptr<cdk::basic_type>> args = symbol->arguments();
  
  size_t argsSize = 0;
  if (node->arguments()) {
    for (int ax = node->arguments()->size(); ax > 0; ax--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ax - 1));
      arg->accept(this, lvl + 2);
      if (args.size() == node->arguments()->size() && arg->is_typed(cdk::TYPE_INT) && args.at(ax - 1)->name() == cdk::TYPE_DOUBLE) _pf.I2D();

      argsSize += arg->type()->size();
    }
  }

  if (symbol->is_typed(cdk::TYPE_STRUCT)) {
    _pf.INT(symbol->type()->size());
    _pf.ALLOC();
    _pf.SP();
    argsSize += 4;
  }

  _pf.CALL(node->identifier());
  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  if (symbol->is_typed(cdk::TYPE_INT) || symbol->is_typed(cdk::TYPE_POINTER) || symbol->is_typed(cdk::TYPE_STRING)) _pf.LDFVAL32();
  else if (symbol->is_typed(cdk::TYPE_DOUBLE)) _pf.LDFVAL64();
  else if (symbol->is_typed(cdk::TYPE_STRUCT)) { 
    /* INVOCATION FUNCTION RETURNING TUPLES */
    int local_offset = symbol->type()->size();
    
    auto struct_type = cdk::structured_type_cast(symbol->type());
    for (auto component : struct_type->components()) {
      local_offset -= component->size();
      //std::cout << "(GET) local_offset: " << local_offset << " ; current_size: " << component->size() << std::endl;
      _pf.LDFVAL32();
      _pf.INT(local_offset);
      _pf.ADD();

      if (component->name() == cdk::TYPE_INT || component->name() == cdk::TYPE_POINTER || component->name() == cdk::TYPE_STRING) _pf.LDINT();
      else if (component->name() == cdk::TYPE_DOUBLE) _pf.LDDOUBLE();

      local_offset -= local_offset;
    }
  }
  else if (symbol->is_typed(cdk::TYPE_VOID)) { /* empty */ }
  else std::cerr << "Should not happen" << std::endl;

}
void og::postfix_writer::do_return_node(og::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _functionHasReturn = true;
  if (!_function->is_typed(cdk::TYPE_VOID)) {
    node->return_value()->accept(this, lvl + 2);

    if (_function->is_typed(cdk::TYPE_INT) || _function->is_typed(cdk::TYPE_STRING) || _function->is_typed(cdk::TYPE_POINTER)) {
      _pf.STFVAL32();
    } else if (_function->is_typed(cdk::TYPE_DOUBLE)) {
      if (node->return_value()->is_typed(cdk::TYPE_INT))
        _pf.I2D();
      _pf.STFVAL64();
    } else if (_function->is_typed(cdk::TYPE_STRUCT)) {
      /* RETURN TUPLES */
      int local_offset = 0;
      auto struct_type = cdk::structured_type_cast(_function->type());
      for (size_t actual = struct_type->length(); actual > 0; actual--) {
        auto component = struct_type->component(actual - 1);
        _pf.LOCAL(8);
        _pf.LDINT();
        _pf.INT(local_offset);
        _pf.ADD();

        if (component->name() == cdk::TYPE_INT || component->name() == cdk::TYPE_POINTER || component->name() == cdk::TYPE_STRING) _pf.STINT();
        else if (component->name() == cdk::TYPE_DOUBLE) _pf.STDOUBLE();

        //std::cout << "(PUSH) local_offset: " << local_offset << " ; current_size: " << component->size() << std::endl;
        local_offset += component->size();
      }

      _pf.LOCAL(8);
      _pf.LDINT();
      _pf.STFVAL32();

    } else {
      std::cerr << node->lineno() << ": should not happen: unknown return type" << std::endl;
    }
  }
  _pf.LEAVE();
  _pf.RET();
}
void og::postfix_writer::do_allocation_node(og::allocation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  _pf.INT(3);
  _pf.SHTL();
  _pf.ALLOC();
  _pf.SP();
}
void og::postfix_writer::do_index_node(og::index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->base()->accept(this, lvl);
  node->index()->accept(this, lvl);
  _pf.INT(3);
  _pf.SHTL();
  _pf.ADD();
}
void og::postfix_writer::do_index_tuple_node(og::index_tuple_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  size_t offset = 0;
  auto symbol = _symtab.find(node->identifier());
  auto structured_type = cdk::structured_type_cast(symbol->type());

  if (symbol->global()) {
    for (int actual = 0; actual < node->index() - 1; actual++) offset += structured_type->component(actual)->size();
    _pf.ADDR(node->identifier());
  } else {
    for (int actual = structured_type->length(); actual > node->index(); actual--) offset += structured_type->component(actual - 1)->size();
    _pf.LOCAL(symbol->offset());
  }

  _pf.INT(offset);
  _pf.ADD();
}
void og::postfix_writer::do_position_node(og::position_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl + 2);
}
void og::postfix_writer::do_var_declaration_node(og::var_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int offset = 0, typesize = node->type()->size();

  if (_inFunctionBody) {
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    offset = _offset;
    _offset += typesize;
  } else {
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
      if (symbol) {
        if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER) || node->is_typed(cdk::TYPE_STRING)) {
          node->expressions()->accept(this, lvl);
          if (node->is_typed(cdk::TYPE_DOUBLE) && node->expressions()->is_typed(cdk::TYPE_INT))
            _pf.I2D();

          if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_POINTER)) {
            _pf.LOCAL(symbol->offset());
            _pf.STINT();
          } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
            _pf.LOCAL(symbol->offset());
            _pf.STDOUBLE();
          } else {
            std::cerr << "cannot initialize" << std::endl;
          }
        } else if (node->is_typed(cdk::TYPE_STRUCT)) {
          auto struct_type = cdk::structured_type_cast(node->expressions()->type());

          // Initiliaze values
          size_t current_offset = 0; // 12
          node->expressions()->accept(this, lvl);
          for (size_t actual = struct_type->length(); actual > 0 ; actual--) {
            _pf.LOCAL(symbol->offset() + current_offset); // - 4 + 4 = 0
            current_offset += struct_type->component(actual - 1)->size(); // 12 - 8 = 4

            if (struct_type->component(actual - 1)->name() == cdk::TYPE_INT || struct_type->component(actual - 1)->name() == cdk::TYPE_STRING || 
              struct_type->component(actual - 1)->name() == cdk::TYPE_POINTER || struct_type->component(actual - 1)->name() == cdk::TYPE_STRUCT) {
                _pf.STINT();
            } else if (struct_type->component(actual - 1)->name() == cdk::TYPE_DOUBLE) {
              _pf.STDOUBLE();
            } else {
              std::cerr << "cannot initialize" << std::endl;
            }
          }

        } else {
          std::cerr << "problem initializing" << std::endl;
        }
      } else {
        auto struct_type = cdk::structured_type_cast(node->expressions()->type());
        node->expressions()->accept(this, lvl);
        for (size_t actual = struct_type->length(); actual > 0; actual--) {

          auto symbol = _symtab.find(node->identifiers()->at(actual - 1));
          symbol->set_offset(offset);

          if (struct_type->component(actual - 1)->name() == cdk::TYPE_INT || struct_type->component(actual - 1)->name() == cdk::TYPE_STRING || 
              struct_type->component(actual - 1)->name() == cdk::TYPE_POINTER || struct_type->component(actual - 1)->name() == cdk::TYPE_STRUCT) {
            _pf.LOCAL(symbol->offset());
            _pf.STINT();
          } else if (struct_type->component(actual - 1)->name() == cdk::TYPE_DOUBLE) {
            _pf.LOCAL(symbol->offset());
            _pf.STDOUBLE();
          } else {
            std::cerr << "cannot initialize" << std::endl;
          }

          offset += struct_type->component(actual - 1)->size();
        }
      }
    }
  } else {
    if (!_function) {
      auto id = node->identifiers()->at(0);
      if (node->expressions() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(id);
        _pf.SALLOC(typesize);
      } else {
        if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER) || node->is_typed(cdk::TYPE_STRING)) {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);

          if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_STRING) || node->is_typed(cdk::TYPE_POINTER)) {
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
            }
          }
        } else if (node->is_typed(cdk::TYPE_STRUCT) && node->identifiers()->size() > 1) {
          _pf.DATA();
          _pf.ALIGN();

          auto struct_type = cdk::structured_type_cast(node->expressions()->type());
          auto tuple_node = (og::tuple_node *) node->expressions();
          for (size_t actual = 0; actual < struct_type->length(); actual++) {
            _pf.LABEL(node->identifiers()->at(actual));
            tuple_node->node(actual)->accept(this, lvl);
          }
        
        } else if (node->is_typed(cdk::TYPE_STRUCT) && node->identifiers()->size() == 1) {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(node->identifiers()->at(0));

          node->expressions()->accept(this, lvl);

        } else {
          std::cerr << node->lineno() << ": '" << id << "' has unexpected initializer\n";
        }

      }

    }
  }
}
void og::postfix_writer::do_tuple_node(og::tuple_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}
void og::postfix_writer::do_identity_node(og::identity_node *const node, int lvl) {   
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}
void og::postfix_writer::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (_inFunctionBody) _pf.INT(0);
  else _pf.SINT(0);
}
void og::postfix_writer::do_sizeof_node(og::sizeof_node *const node, int lvl) {
  sizeof_calculator lsc(_compiler, _symtab);
  node->expression()->accept(&lsc, lvl);
  _pf.INT(lsc.localsize());
}