%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                         i;	               /* integer value */
  double                      r;
  std::string                 *s;	               /* symbol name or string literal */
  cdk::basic_node             *node;	          /* node pointer */
  cdk::sequence_node          *sequence;
  cdk::expression_node        *expression;        /* expression nodes */
  cdk::lvalue_node            *lvalue;
};

%token <i>          tINTEGER
%token <r>          tREAL
%token <s>          tIDENTIFIER tSTRING
%token              tFOR tIF tPRINT tINPUT
%token              tTHEN tDO tPUBLIC tREQUIRE tRETURN tPRINT tPOINTER tWRITE tWRITELN tBREAK tCONTINUE tPROCEDURE
%token              tINTTAG tREALTAG tSTRINGTAG tAUTOTAG tPOINTERTAG

%nonassoc           tIFX
%nonassoc           tELIF
%nonassoc           tELSE

%right              '='
%left               tGE tLE tEQ tNE '>' '<'
%left               '+' '-'
%left               '*' '/' '%'
%nonassoc           tUNARY
%nonassoc           ';' '(' ')' '{' '}' ','

%type <node>        instr file inst_condit inst_iter function procedure declaration
%type <sequence>    block exps vars declarations instrs identifiers
%type <expression>  expr
%type <lvalue>      lval var

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file	         : declarations                                     { /* TODO */ }
	             ;

/* Extra to be easier. */
declarations   :              declaration                         { /* TODO */ }
               | declarations declaration                         { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

/* Extra to be easier. */
pos_others     : '('      ')'
               | '('      ')' block
               | '(' vars ')'
               | '(' vars ')' block
               ;

/* Extra to be easier. */
instrs         :        instr                                     { /* TODO */ }
               | instrs instr                                     { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

/* Extra to be easier. */
elif_condit    :             tELIF expr tTHEN instr               { /* TODO */ }
               | elif_condit tELIF expr tTHEN instr               { /* TODO */ }
               ;

/* Extra to be easier. */

declaration    : var ';'                                          { /* TODO */ }
               | function                                         { /* TODO */ }
               | procedure                                        { /* TODO */ }
               ;

var            :          type tIDENTIFIER                        { /* TODO */ }
               | tPUBLIC  type tIDENTIFIER                        { /* TODO */ }
               | tREQUIRE type tIDENTIFIER                        { /* TODO */ }
               | tPUBLIC  type tIDENTIFIER    '=' expr            { /* TODO */ }
               | tREQUIRE type tIDENTIFIER    '=' expr            { /* TODO */ }
               |         tAUTOTAG identifiers '=' exps            { /* TODO */ }
               | tPUBLIC tAUTOTAG identifiers '=' exps            { /* TODO */ }
               ;

function       :            type      tIDENTIFIER pos_others      { /* TODO */ }
               |            tAUTOTAG  tIDENTIFIER pos_others      { /* TODO */ }
               | tPUBLIC    tAUTOTAG  tIDENTIFIER pos_others      { /* TODO */ }
               | tPUBLIC    type      tIDENTIFIER pos_others      { /* TODO */ }
               | tREQUIRE   tAUTOTAG  tIDENTIFIER pos_others      { /* TODO */ }
               | tREQUIRE   type      tIDENTIFIER pos_others      { /* TODO */ }
               ;

procedure      : tPUBLIC  tPROCEDURE tIDENTIFIER pos_others       { /* TODO */ }
               | tREQUIRE tPROCEDURE tIDENTIFIER pos_others       { /* TODO */ }
               ;

identifiers    :                 tIDENTIFIER                      { /* TODO */ }
               | identifiers ',' tIDENTIFIER                      { /* TODO */ }
               ;

exps           :          expr                                   { $$ = new cdk::sequence_node(LINE, $1); }
               | exps ',' expr                                   { $$ = new cdk::sequence_node(LINE, $3, $1); }
               ;

vars           :          var                                    { $$ = new cdk::sequence_node(LINE, $1); }
               | vars ',' var                                    { $$ = new cdk::sequence_node(LINE, $3, $1); }
               ;

type           : tINTTAG                                         { /* TODO */ }
               | tREALTAG                                        { /* TODO */ }
               | tSTRINGTAG                                      { /* TODO */ }
               | tPOINTERTAG '<' tAUTOTAG '>'                    { /* TODO */ }
               | tPOINTERTAG '<' type     '>'                    { /* TODO */ }
               ;

block          : '{'                      '}'                    { /* TODO */ }
               | '{' declarations         '}'                    { /* TODO */ }
               | '{'              instrs  '}'                    { /* TODO */ }
               | '{' declarations instrs  '}'                    { /* TODO */ }
               ;

instr          : expr ';'                                        { $$ = new og::evaluation_node(LINE, $1); }
               | tWRITE exps ';'                                 { /* TODO */ }
               | tWRITELN exps ';'                               { /* TODO */ }
               | tBREAK ';'                                      { /* TODO */ }
               | tCONTINUE ';'                                   { /* TODO */ }
               | tRETURN ';'                                     { /* TODO */ }
               | tBREAK exps ';'                                 { /* TODO */ }
               | tCONTINUE exps ';'                              { /* TODO */ }
               | tRETURN exps ';'                                { /* TODO */ }
               | inst_condit                                     { /* TODO */ }
               | inst_iter                                       { /* TODO */ }
               | block                                           { $$ = $1; }
               ;

inst_condit    : tIF expr tTHEN instr                         %prec tIFX     { $$ = new og::if_node(LINE, $2, $4); }
               | tIF expr tTHEN instr elif_condit             %prec tIFX     { /* TODO */ }
               | tIF expr tTHEN instr elif_condit tELSE instr                { /* TODO */ }
               | tIF expr tTHEN instr             tELSE instr %prec tELSE    { $$ = new og::if_else_node(LINE, $2, $4, $6); }
               ;

inst_iter      : tFOR      ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, nullptr, $5); }
               | tFOR exps ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , $4     , nullptr, $7); }
               | tFOR exps ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, $5     , $7); }
               | tFOR exps ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, nullptr, $6); }
               | tFOR      ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, $3     , $5     , $7); }
               | tFOR      ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, $4     , $6); }
               | tFOR exps ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , $4     , $6     , $8); }
               | tFOR      ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, $3     , nullptr, $6); }
               | tFOR      ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, nullptr, $5); }
               | tFOR vars ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, nullptr, $6); }
               | tFOR vars ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , $4     , nullptr, $7); }
               | tFOR vars ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, $5     , $7); }
               | tFOR      ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, $3     , $5     , $7); }
               | tFOR      ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, $3     , nullptr, $6); }
               | tFOR      ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, $4     , $6); }
               | tFOR vars ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , $4     , $6     , $8); }
               ;

expr           : tINTEGER                                        { $$ = new cdk::integer_node(LINE, $1); }
               | tSTRING                                         { $$ = new cdk::string_node(LINE, $1); }
               | '-' expr %prec tUNARY                           { $$ = new cdk::neg_node(LINE, $2); }
               | '+' expr %prec tUNARY                           { /* TODO */ }
               | expr '+' expr	                                 { $$ = new cdk::add_node(LINE, $1, $3); }
               | expr '-' expr	                                 { $$ = new cdk::sub_node(LINE, $1, $3); }
               | expr '*' expr	                                 { $$ = new cdk::mul_node(LINE, $1, $3); }
               | expr '/' expr	                                 { $$ = new cdk::div_node(LINE, $1, $3); }
               | expr '%' expr	                                 { $$ = new cdk::mod_node(LINE, $1, $3); }
               | expr '<' expr	                                 { $$ = new cdk::lt_node(LINE, $1, $3); }
               | expr '>' expr	                                 { $$ = new cdk::gt_node(LINE, $1, $3); }
               | expr tGE expr	                                 { $$ = new cdk::ge_node(LINE, $1, $3); }
               | expr tLE expr                                   { $$ = new cdk::le_node(LINE, $1, $3); }
               | expr tNE expr	                                 { $$ = new cdk::ne_node(LINE, $1, $3); }
               | expr tEQ expr	                                 { $$ = new cdk::eq_node(LINE, $1, $3); }
               | '(' expr ')'                                    { $$ = $2; }
               | lval                                            { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
               | lval '=' expr                                   { $$ = new cdk::assignment_node(LINE, $1, $3); }
               ;

lval           : tIDENTIFIER                                     { $$ = new cdk::variable_node(LINE, $1); }
               ;



%%
