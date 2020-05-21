#include <string>
#include "targets/type_checker.h"
#include "targets/return_verifier.h"
#include "targets/symbol.h"
#include "ast/all.h"

og::return_verifier::~return_verifier() {
  os().flush();
}

void og::return_verifier::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void og::return_verifier::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_nil_node(cdk::nil_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_identity_node(og::identity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_evaluation_node(og::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}
void og::return_verifier::do_write_node(og::write_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_input_node(og::input_node * const node, int lvl) {
  // EMPTY
}
void og::return_verifier::do_sizeof_node(og::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_function_invocation_node(og::function_invocation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_index_node(og::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_nullptr_node(og::nullptr_node * const node, int lvl) {
  // EMPTY
}
void og::return_verifier::do_return_node(og::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (node->return_value()) {
    node->return_value()->accept(this, lvl);

    if (!_return_type) _return_type = node->return_value()->type();
    else if (_return_type != node->return_value()->type()) std::cerr << "ERROR: different return types in same function" << std::endl;
  } else {
    if (!_return_type) _return_type = cdk::make_primitive_type(0, cdk::TYPE_VOID);
    else if (_return_type->name() == cdk::TYPE_VOID) std::cerr << "ERROR: different return types in same function" << std::endl;
  }
}
void og::return_verifier::do_for_node(og::for_node * const node, int lvl) {
  if (node->init_exp()) node->init_exp()->accept(this, lvl);
  if (node->init_seq()) node->init_seq()->accept(this, lvl);

  node->block()->accept(this, lvl);
}
void og::return_verifier::do_tuple_node(og::tuple_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_allocation_node(og::allocation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_break_node(og::break_node *const node, int lvl) {
  // EMPTY
}
void og::return_verifier::do_continue_node(og::continue_node *const node, int lvl) {
  // EMPTY
}
void og::return_verifier::do_index_tuple_node(og::index_tuple_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}
void og::return_verifier::do_position_node(og::position_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}

void og::return_verifier::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

void og::return_verifier::do_block_node(og::block_node * const node, int lvl) {
  if (node->declarations()) node->declarations()->accept(this, lvl);
  if (node->instructions()) node->instructions()->accept(this, lvl);
}

void og::return_verifier::do_if_node(og::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->block()->accept(this, lvl);
}

void og::return_verifier::do_if_else_node(og::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->thenblock()->accept(this, lvl);
  node->elseblock()->accept(this, lvl);
}

void og::return_verifier::do_var_declaration_node(og::var_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
}

void og::return_verifier::do_function_declaration_node(og::function_declaration_node * const node, int lvl) {
  if (node->arguments()) node->arguments()->accept(this, lvl);
  if (node->block()) node->block()->accept(this, lvl);
}
