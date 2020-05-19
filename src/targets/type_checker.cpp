#include <string>
#include <vector>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/basic_type.h>
#include <cdk/types/primitive_type.h>

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void og::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
}
void og::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void og::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void og::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in argument of unary expression");

  // in Simple, expressions are always int
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void og::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_INT)) {
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
  } else if ((node->left()->is_typed(cdk::TYPE_DOUBLE) || node->right()->is_typed(cdk::TYPE_DOUBLE)) && 
      (node->left()->is_typed(cdk::TYPE_INT) || node->right()->is_typed(cdk::TYPE_INT))) {
    node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
  } else if (node->left()->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_DOUBLE)) {
    node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
  } else {
   throw std::string("wrong type in left argument of binary expression");
  }
}

void og::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void og::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void og::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void og::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    auto symbol = std::make_shared<og::symbol>(-1, cdk::make_primitive_type(4, cdk::TYPE_INT), id, false, false);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }

  if (!node->lvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (!node->rvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of assignment expression");

  // in Simple, expressions are always int
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void og::type_checker::do_function_declaration_node(og::function_declaration_node *const node, int lvl) {
  std::string label_name;
  if (!node->identifier().compare("og"))
    label_name = "_main";
  else if (!node->identifier().compare("_main"))
    label_name = "og";
  else label_name = node->identifier();

  std::shared_ptr<og::symbol> function = std::make_shared<og::symbol
      > (node->qualifier(), node->type(), label_name, false, true);

  std::shared_ptr<og::symbol> previous = _symtab.find(function->name());
  if (previous) {
    if (false /*DAVID: FIXME: should verify fields*/) {
      throw std::string("conflicting declaration for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}

void og::type_checker::do_evaluation_node(og::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void og::type_checker::do_write_node(og::write_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void og::type_checker::do_input_node(og::input_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::type_checker::do_for_node(og::for_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::type_checker::do_if_node(og::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void og::type_checker::do_if_else_node(og::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void og::type_checker::do_block_node(og::block_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_break_node(og::break_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_continue_node(og::continue_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_function_invocation_node(og::function_invocation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->identifier();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);

  if (symbol == nullptr) throw std::string("symbol '" + id + "' is undeclared.");

  if (!symbol->isFunction()) throw std::string("symbol '" + id + "' is not a function.");

  node->type(symbol->type());

  //DAVID: FIXME: should also validate args against symbol
  if (node->arguments()) {
    node->arguments()->accept(this, lvl + 4);
  }
}
void og::type_checker::do_return_node(og::return_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_allocation_node(og::allocation_node *const node, int lvl) {  
  // EMPTY
}
void og::type_checker::do_index_node(og::index_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_index_tuple_node(og::index_tuple_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_position_node(og::position_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_var_declaration_node(og::var_declaration_node *const node, int lvl) {
  if (node->expressions() != nullptr) {
    node->expressions()->accept(this, lvl + 2);

    if (node->is_typed(cdk::TYPE_INT)) {
      if (!node->expressions()->is_typed(cdk::TYPE_INT)) throw std::string(
          "wrong type for initializer (integer expected).");
    } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
      if (!node->expressions()->is_typed(cdk::TYPE_INT)
          && !node->expressions()->is_typed(cdk::TYPE_DOUBLE)) throw std::string(
          "wrong type for initializer (integer or double expected).");
    } else if (node->is_typed(cdk::TYPE_STRING)) {
      if (!node->expressions()->is_typed(cdk::TYPE_STRING)) throw std::string(
          "wrong type for initializer (string expected).");
    } else if (node->is_typed(cdk::TYPE_POINTER)) {
      //DAVID: FIXME: trouble!!!
      if (!node->expressions()->is_typed(cdk::TYPE_POINTER)) throw std::string(
          "wrong type for initializer (pointer expected).");
    } else {
      node->type(node->expressions()->type());
      //throw std::string("unknown type for initializer.");
    }
  }

  const std::string &id = node->identifiers()->at(0);
  std::shared_ptr<og::symbol> symbol = std::make_shared<og::symbol> (node->qualifier(), node->type(), id, (bool)node->expressions(), false);
  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
  } else {
    throw std::string("variable '" + id + "' redeclared");
  }
}
void og::type_checker::do_tuple_node(og::tuple_node *const node, int lvl) {
  ASSERT_UNSPEC;

  if (node->size() == 1) {
    node->node(0)->accept(this, lvl);
    node->type(node->node(0)->type());
  } else if (node->size() > 1) {

    std::vector<std::shared_ptr<cdk::basic_type>> vector;
    for (size_t ix = 0; ix < node->size(); ix++) {
      cdk::expression_node *exp = node->node(ix);
      if (exp == nullptr) break;
      exp->accept(this, 0);

      vector.push_back(exp->type());
    }

    node->type(cdk::make_structured_type(vector));
  }
}
void og::type_checker::do_identity_node(og::identity_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}
void og::type_checker::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_sizeof_node(og::sizeof_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}