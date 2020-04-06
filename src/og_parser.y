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
%token              tFOR tIF tINPUT tSIZEOF
%token              tTHEN tDO tPUBLIC tREQUIRE tRETURN tPOINTER tWRITE tWRITELN tBREAK tCONTINUE tPROCEDURE
%token              tINTTAG tREALTAG tSTRINGTAG tAUTOTAG tPOINTERTAG

%nonassoc           tIFX
%nonassoc           tELIF
%nonassoc           tELSE

%nonassoc           tBLOCKNOX

%right              '='
%left               tOR
%left               tAND
%nonassoc           '~'
%left               tEQ tNE
%left               tGE tLE '>' '<'
%left               '+' '-'
%left               '*' '/' '%'
%nonassoc           tUNARY '?'
%nonassoc           '{' '[' '(' ','

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
               | declarations declaration                         { /* TODO */ }
               ;

/* Extra to be easier. */
instrs         :        instr                                     { /* TODO */ }
               | instrs instr                                     { /* TODO */ }
               ;

/* Extra to be easier. */
elif_condit    :             tELIF expr tTHEN instr               { /* TODO */ }
               | elif_condit tELIF expr tTHEN instr               { /* TODO */ }
               ;

declaration    : var ';'                                          { $$ = new cdk::var_declaration_node(LINE, $1);      }
               | function                                         { $$ = new cdk::function_declaration_node(LINE, $1); }
               | procedure                                        { /* TODO */ }
               ;

var            :          type tIDENTIFIER                        { /* TODO */ }
               | tPUBLIC  type tIDENTIFIER                        { /* TODO */ }
               | tREQUIRE type tIDENTIFIER                        { /* TODO */ }
               |          type tIDENTIFIER    '=' expr            { /* TODO */ }
               | tPUBLIC  type tIDENTIFIER    '=' expr            { /* TODO */ }
               | tREQUIRE type tIDENTIFIER    '=' expr            { /* TODO */ }
               |         tAUTOTAG identifiers '=' exps            { /* TODO */ }
               | tPUBLIC tAUTOTAG identifiers '=' exps            { /* TODO */ }
               ;

function       :            type      tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { /* TODO */ }
               |            tAUTOTAG  tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { /* TODO */ }
               | tPUBLIC    tAUTOTAG  tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { /* TODO */ }
               | tPUBLIC    type      tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { /* TODO */ }
               | tREQUIRE   tAUTOTAG  tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { /* TODO */ }
               | tREQUIRE   type      tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { /* TODO */ }
               |            type      tIDENTIFIER '('      ')' block                     { /* TODO */ }
               |            tAUTOTAG  tIDENTIFIER '('      ')' block                     { /* TODO */ }
               | tPUBLIC    tAUTOTAG  tIDENTIFIER '('      ')' block                     { /* TODO */ }
               | tPUBLIC    type      tIDENTIFIER '('      ')' block                     { /* TODO */ }
               | tREQUIRE   tAUTOTAG  tIDENTIFIER '('      ')' block                     { /* TODO */ }
               | tREQUIRE   type      tIDENTIFIER '('      ')' block                     { /* TODO */ }
               |            type      tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { /* TODO */ }
               |            tAUTOTAG  tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { /* TODO */ }
               | tPUBLIC    tAUTOTAG  tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { /* TODO */ }
               | tPUBLIC    type      tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { /* TODO */ }
               | tREQUIRE   tAUTOTAG  tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { /* TODO */ }
               | tREQUIRE   type      tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { /* TODO */ }
               |            type      tIDENTIFIER '(' vars ')' block                     { /* TODO */ }
               |            tAUTOTAG  tIDENTIFIER '(' vars ')' block                     { /* TODO */ }
               | tPUBLIC    tAUTOTAG  tIDENTIFIER '(' vars ')' block                     { /* TODO */ }
               | tPUBLIC    type      tIDENTIFIER '(' vars ')' block                     { /* TODO */ }
               | tREQUIRE   tAUTOTAG  tIDENTIFIER '(' vars ')' block                     { /* TODO */ }
               | tREQUIRE   type      tIDENTIFIER '(' vars ')' block                     { /* TODO */ }
               ;

procedure      :          tPROCEDURE tIDENTIFIER '('      ')'       %prec tBLOCKNOX      { /* TODO */ }
               | tPUBLIC  tPROCEDURE tIDENTIFIER '('      ')'       %prec tBLOCKNOX      { /* TODO */ }
               | tREQUIRE tPROCEDURE tIDENTIFIER '('      ')'       %prec tBLOCKNOX      { /* TODO */ }
               |          tPROCEDURE tIDENTIFIER '('      ')' block                      { /* TODO */ }
               | tPUBLIC  tPROCEDURE tIDENTIFIER '('      ')' block                      { /* TODO */ }
               | tREQUIRE tPROCEDURE tIDENTIFIER '('      ')' block                      { /* TODO */ }
               |          tPROCEDURE tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX      { /* TODO */ }
               | tPUBLIC  tPROCEDURE tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX      { /* TODO */ }
               | tREQUIRE tPROCEDURE tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX      { /* TODO */ }
               |          tPROCEDURE tIDENTIFIER '(' vars ')' block                      { /* TODO */ }
               | tPUBLIC  tPROCEDURE tIDENTIFIER '(' vars ')' block                      { /* TODO */ }
               | tREQUIRE tPROCEDURE tIDENTIFIER '(' vars ')' block                      { /* TODO */ }
               ;

identifiers    :                 tIDENTIFIER                     { /* TODO */ }
               | identifiers ',' tIDENTIFIER                     { /* TODO */ }
               ;

exps           :          expr                                   { $$ = new cdk::expression_node(LINE,$1);    }
               | exps ',' expr                                   { $$ = new cdk::expression_node(LINE,$1,$3); }
               ;

vars           :          var                                    { $$ = new cdk::variable_node(LINE,$1);    }
               | vars ',' var                                    { $$ = new cdk::variable_node(LINE,$1,$3); }
               ;

type           : tINTTAG                                         { /* TODO */ }
               | tREALTAG                                        { /* TODO */ }
               | tSTRINGTAG                                      { /* TODO */ }
               | tPOINTERTAG '<' tAUTOTAG '>'                    { /* TODO */ }
               | tPOINTERTAG '<' type     '>'                    { /* TODO */ }
               ;

block          : '{'                      '}'                    { $$ = new og::block_node(LINE, $1,$3);   }
               | '{' declarations         '}'                    { /* TODO */ }
               | '{'              instrs  '}'                    { /* TODO */ }
               | '{' declarations instrs  '}'                    { /* TODO */ }
               ;

instr          : expr ';'                                        { $$ = new og::evaluation_node(LINE, $1); }
               | tWRITE exps ';'                                 { $$ = new og::write_node(LINE, $1);      }
               | tWRITELN exps ';'                               { $$ = new og::write_node(LINE, $1);      }
               | tBREAK                                          { $$ = new og::break_node(LINE, $1);      }
               | tCONTINUE                                       { $$ = new og::continue_node(LINE, $1);   }
               | tRETURN ';'                                     { $$ = new og::return_node(LINE, $1);     }
               | tRETURN exps ';'                                { $$ = new og::return_node(LINE, $1);     }
               | inst_condit                                     { $$ = $1; }
               | inst_iter                                       { $$ = $1; }
               | block                                           { $$ = $1; }
               ;

inst_condit    : tIF expr tTHEN instr                         %prec tIFX     { $$ = new og::if_node(LINE, $2, $4); }
               | tIF expr tTHEN instr elif_condit             %prec tIFX     { /* TODO */ }
               | tIF expr tTHEN instr elif_condit tELSE instr                { /* TODO */ }
               | tIF expr tTHEN instr             tELSE instr                { $$ = new og::if_else_node(LINE, $2, $4, $6); }
               ;

inst_iter      : tFOR      ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, nullptr, $5); }
               | tFOR      ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, $4     , $6); }
               | tFOR      ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, $3     , nullptr, $6); }
               | tFOR      ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, $3     , $5     , $7); }
               | tFOR exps ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, nullptr, $6); }
               | tFOR exps ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, $5     , $7); }
               | tFOR exps ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , $4     , nullptr, $7); }
               | tFOR exps ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , $4     , $6     , $8); }
               | tFOR vars ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, nullptr, $6); }
               | tFOR vars ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, $5     , $7); }
               | tFOR vars ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , $4     , nullptr, $7); }
               | tFOR vars ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , $4     , $6     , $8); }
               ;

expr           : tINTEGER                                        { $$ = new cdk::integer_node(LINE, $1);  }
               | tREAL                                           { $$ = new cdk::double_node(LINE, $1);   }
               | string                                          {    }
               | '-' expr  %prec tUNARY                          { $$ = new cdk::neg_node(LINE, $2);      }
               | '+' expr  %prec tUNARY                          { $$ = new og::identity_node(LINE, $2);  }
               | '~' expr  %prec tUNARY                          { $$ = new cdk::not_node(LINE, $2);      }
               | expr '+'  expr	                                 { $$ = new cdk::add_node(LINE, $1, $3);  }
               | expr '-'  expr	                                 { $$ = new cdk::sub_node(LINE, $1, $3);  }
               | expr '*'  expr	                                 { $$ = new cdk::mul_node(LINE, $1, $3);  }
               | expr '/'  expr	                                 { $$ = new cdk::div_node(LINE, $1, $3);  }
               | expr tAND expr	                                 { $$ = new cdk::and_node(LINE, $1, $3);  }
               | expr tOR expr	                                 { $$ = new cdk::or_node(LINE, $1, $3);   }
               | expr '%'  expr	                                 { $$ = new cdk::mod_node(LINE, $1, $3);  }
               | expr '<'  expr	                                 { $$ = new cdk::lt_node(LINE, $1, $3);   }
               | expr '>'  expr	                                 { $$ = new cdk::gt_node(LINE, $1, $3);   }
               | expr tGE  expr	                                 { $$ = new cdk::ge_node(LINE, $1, $3);   }
               | expr tLE  expr                                  { $$ = new cdk::le_node(LINE, $1, $3);   }
               | expr tNE  expr	                                 { $$ = new cdk::ne_node(LINE, $1, $3);   }
               | expr tEQ  expr	                                 { $$ = new cdk::eq_node(LINE, $1, $3);   }
               | '(' expr ')'                                    { $$ = $2;                               }
               | '[' expr ']'                                    { $$ = $2;                               }
               | tIDENTIFIER '('      ')'                        { /* TODO */ }
               | tIDENTIFIER '(' exps ')'                        { /* TODO */ }
               | tSIZEOF     '(' exps ')'                        { /* TODO */ }
               | lval                                            { $$ = new cdk::rvalue_node(LINE, $1); }
               | lval '?'                                        { $$ = new og::position_node(LINE, $1); }
               | lval '=' expr                                   { $$ = new cdk::assignment_node(LINE, $1, $3); }
               ;

string         :        tSTRING                                  { $$ = new cdk::string_node(LINE, $1); }
               | string tSTRING                                  { /* TODO */ }
               ;

lval           : tIDENTIFIER                                     { $$ = new cdk::variable_node(LINE, $1);    }
               | expr '[' expr ']'                               { $$ = new og::index_node(LINE, $1, $3);    }
               | tIDENTIFIER '@' tINTEGER                        { $$ = new cdk::index_tuple_node(LINE, $2); }
               ;



%%
