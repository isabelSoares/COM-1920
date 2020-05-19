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
  int                                   i;	               /* integer value */
  double                                r;
  std::string                           *s;	               /* symbol name or string literal */
  cdk::basic_node                       *node;	           /* node pointer */
  cdk::basic_type                       *type;             /* expression type */
  std::shared_ptr<cdk::basic_type>      *shared_type;
  std::vector<std::string>              *vector;
  og::tuple_node                        *tuple;
  cdk::sequence_node                    *sequence;
  cdk::expression_node                  *expression;       /* expression nodes */
  cdk::lvalue_node                      *lvalue;
  og::block_node                        *block;
};

%token <i>          tINTEGER
%token <r>          tREAL
%token <s>          tIDENTIFIER tSTRING
%token              tFOR tIF tELSE tELIF tINPUT tSIZEOF tNULLPTR
%token              tTHEN tDO tPUBLIC tPRIVATE tREQUIRE tRETURN tPOINTER tWRITE tWRITELN tBREAK tCONTINUE tPROCEDURE
%token <type>       tINTTAG tREALTAG tSTRINGTAG tAUTOTAG tPOINTERTAG

%nonassoc           tIFX
%nonassoc           tELIFX
%nonassoc           tELIF tELSE

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

%type <block>       block
%type <node>        instr file inst_condit inst_iter function procedure declaration elif_condit var
%type <sequence>    vars declarations instrs
%type <vector>      identifiers
%type <expression>  expr
%type <shared_type> type
%type <tuple>       exps
%type <s>           string
%type <lvalue>      lval

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file	         : declarations                                     { compiler->ast($$ = $1); }
	             ;

/* Extra to be easier. */
declarations   :              declaration                         { $$ = new cdk::sequence_node(LINE, $1);     }
               | declarations declaration                         { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

/* Extra to be easier. */
instrs         :        instr                                     { $$ = new cdk::sequence_node(LINE, $1);     }
               | instrs instr                                     { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

/* Extra to be easier. */
elif_condit    : tELSE instr                                      { $$ = $2;                                     }
               | tELIF expr tTHEN instr             %prec tELIFX  { $$ = new og::if_node(LINE, $2, $4);          }
               | tELIF expr tTHEN instr elif_condit               { $$ = new og::if_else_node(LINE, $2, $4, $5); }
               ;

declaration    : var ';'                                          { $$ = $1; }
               | function                                         { $$ = $1; }
               | procedure                                        { $$ = $1; }
               ;

var            :          type tIDENTIFIER                        { $$ = new og::var_declaration_node(LINE, tPRIVATE, $1,                                                                                                $2, nullptr); }
               | tPUBLIC  type tIDENTIFIER                        { $$ = new og::var_declaration_node(LINE, tPUBLIC , $2,                                                                                                $3, nullptr); }
               | tREQUIRE type tIDENTIFIER                        { $$ = new og::var_declaration_node(LINE, tREQUIRE, $2,                                                                                                $3, nullptr); }
               |          type tIDENTIFIER    '=' expr            { $$ = new og::var_declaration_node(LINE, tPRIVATE, $1,                                                                                                $2, $4     ); }
               | tPUBLIC  type tIDENTIFIER    '=' expr            { $$ = new og::var_declaration_node(LINE, tPUBLIC , $2,                                                                                                $3, $5     ); }
               | tREQUIRE type tIDENTIFIER    '=' expr            { $$ = new og::var_declaration_node(LINE, tREQUIRE, $2,                                                                                                $3, $5     ); }
               |         tAUTOTAG identifiers '=' exps            { $$ = new og::var_declaration_node(LINE, tPRIVATE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), $2, $4     ); }
               | tPUBLIC tAUTOTAG identifiers '=' exps            { $$ = new og::var_declaration_node(LINE, tPUBLIC , new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), $3, $5     ); }
               ;

function       :            type      tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tPRIVATE, $1,                                                                                                *$2, nullptr, nullptr); delete $2; }
               |            tAUTOTAG  tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tPRIVATE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$2, nullptr, nullptr); delete $2; }
               | tPUBLIC    tAUTOTAG  tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tPUBLIC , new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$3, nullptr, nullptr); delete $3; }
               | tPUBLIC    type      tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tPUBLIC , $2,                                                                                                *$3, nullptr, nullptr); delete $3; }
               | tREQUIRE   tAUTOTAG  tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tREQUIRE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$3, nullptr, nullptr); delete $3; }
               | tREQUIRE   type      tIDENTIFIER '('      ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tREQUIRE, $2,                                                                                                *$3, nullptr, nullptr); delete $3; }
               |            type      tIDENTIFIER '('      ')' block                     { $$ = new og::function_declaration_node(LINE, tPRIVATE, $1,                                                                                                *$2, nullptr, $5     ); delete $2; }
               |            tAUTOTAG  tIDENTIFIER '('      ')' block                     { $$ = new og::function_declaration_node(LINE, tPRIVATE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$2, nullptr, $5     ); delete $2; }
               | tPUBLIC    tAUTOTAG  tIDENTIFIER '('      ')' block                     { $$ = new og::function_declaration_node(LINE, tPUBLIC , new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$3, nullptr, $6     ); delete $3; }
               | tPUBLIC    type      tIDENTIFIER '('      ')' block                     { $$ = new og::function_declaration_node(LINE, tPUBLIC , $2,                                                                                                *$3, nullptr, $6     ); delete $3; }
               | tREQUIRE   tAUTOTAG  tIDENTIFIER '('      ')' block                     { $$ = new og::function_declaration_node(LINE, tREQUIRE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$3, nullptr, $6     ); delete $3; }
               | tREQUIRE   type      tIDENTIFIER '('      ')' block                     { $$ = new og::function_declaration_node(LINE, tREQUIRE, $2,                                                                                                *$3, nullptr, $6     ); delete $3; }
               |            type      tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tPRIVATE, $1,                                                                                                *$2, $4     , nullptr); delete $2; }
               |            tAUTOTAG  tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tPRIVATE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$2, $4     , nullptr); delete $2; }
               | tPUBLIC    tAUTOTAG  tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tPUBLIC , new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$3, $5     , nullptr); delete $3; }
               | tPUBLIC    type      tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tPUBLIC , $2,                                                                                                *$3, $5     , nullptr); delete $3; }
               | tREQUIRE   tAUTOTAG  tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tREQUIRE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$3, $5     , nullptr); delete $3; }
               | tREQUIRE   type      tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX     { $$ = new og::function_declaration_node(LINE, tREQUIRE, $2,                                                                                                *$3, $5     , nullptr); delete $3; }
               |            type      tIDENTIFIER '(' vars ')' block                     { $$ = new og::function_declaration_node(LINE, tPRIVATE, $1,                                                                                                *$2, $4     , $6     ); delete $2; }
               |            tAUTOTAG  tIDENTIFIER '(' vars ')' block                     { $$ = new og::function_declaration_node(LINE, tPRIVATE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$2, $4     , $6     ); delete $2; }
               | tPUBLIC    tAUTOTAG  tIDENTIFIER '(' vars ')' block                     { $$ = new og::function_declaration_node(LINE, tPUBLIC , new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$3, $5     , $7     ); delete $3; }
               | tPUBLIC    type      tIDENTIFIER '(' vars ')' block                     { $$ = new og::function_declaration_node(LINE, tPUBLIC , $2,                                                                                                *$3, $5     , $7     ); delete $3; }
               | tREQUIRE   tAUTOTAG  tIDENTIFIER '(' vars ')' block                     { $$ = new og::function_declaration_node(LINE, tREQUIRE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_UNSPEC)), *$3, $5     , $7     ); delete $3; }
               | tREQUIRE   type      tIDENTIFIER '(' vars ')' block                     { $$ = new og::function_declaration_node(LINE, tREQUIRE, $2,                                                                                                *$3, $5     , $7     ); delete $3; }
               ;

procedure      :          tPROCEDURE tIDENTIFIER '('      ')'       %prec tBLOCKNOX      { $$ = new og::function_declaration_node(LINE, tPRIVATE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$2, nullptr, nullptr); delete $2; }
               | tPUBLIC  tPROCEDURE tIDENTIFIER '('      ')'       %prec tBLOCKNOX      { $$ = new og::function_declaration_node(LINE, tPUBLIC , new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$3, nullptr, nullptr); delete $3; }
               | tREQUIRE tPROCEDURE tIDENTIFIER '('      ')'       %prec tBLOCKNOX      { $$ = new og::function_declaration_node(LINE, tREQUIRE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$3, nullptr, nullptr); delete $3; }
               |          tPROCEDURE tIDENTIFIER '('      ')' block                      { $$ = new og::function_declaration_node(LINE, tPRIVATE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$2, nullptr, $5     ); delete $2; }
               | tPUBLIC  tPROCEDURE tIDENTIFIER '('      ')' block                      { $$ = new og::function_declaration_node(LINE, tPUBLIC , new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$3, nullptr, $6     ); delete $3; }
               | tREQUIRE tPROCEDURE tIDENTIFIER '('      ')' block                      { $$ = new og::function_declaration_node(LINE, tREQUIRE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$3, nullptr, $6     ); delete $3; }
               |          tPROCEDURE tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX      { $$ = new og::function_declaration_node(LINE, tPRIVATE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$2, $4     , nullptr); delete $2; }
               | tPUBLIC  tPROCEDURE tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX      { $$ = new og::function_declaration_node(LINE, tPUBLIC , new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$3, $5     , nullptr); delete $3; }
               | tREQUIRE tPROCEDURE tIDENTIFIER '(' vars ')'       %prec tBLOCKNOX      { $$ = new og::function_declaration_node(LINE, tREQUIRE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$3, $5     , nullptr); delete $3; }
               |          tPROCEDURE tIDENTIFIER '(' vars ')' block                      { $$ = new og::function_declaration_node(LINE, tPRIVATE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$2, $4     , $6     ); delete $2; }
               | tPUBLIC  tPROCEDURE tIDENTIFIER '(' vars ')' block                      { $$ = new og::function_declaration_node(LINE, tPUBLIC , new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$3, $5     , $7     ); delete $3; }
               | tREQUIRE tPROCEDURE tIDENTIFIER '(' vars ')' block                      { $$ = new og::function_declaration_node(LINE, tREQUIRE, new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(0, cdk::typename_type::TYPE_VOID)), *$3, $5     , $7     ); delete $3; }
               ;

identifiers    :                 tIDENTIFIER                     { $$ = new std::vector<std::string>(); $$->push_back(*$1); delete $1; }
               | identifiers ',' tIDENTIFIER                     { $$ = $1;                             $$->push_back(*$3); delete $3; }
               ;

exps           :          expr                                   { $$ = new og::tuple_node(LINE, $1);     }
               | exps ',' expr                                   { $$ = new og::tuple_node(LINE, $3, $1); }
               ;

vars           :          var                                    { $$ = new cdk::sequence_node(LINE, $1);     }
               | vars ',' var                                    { $$ = new cdk::sequence_node(LINE, $3, $1); }
               ;

type           : tINTTAG                                         { $$ = new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(4, cdk::typename_type::TYPE_INT));    }
               | tREALTAG                                        { $$ = new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(8, cdk::typename_type::TYPE_DOUBLE)); }
               | tSTRINGTAG                                      { $$ = new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(4, cdk::typename_type::TYPE_STRING)); }
               | tPOINTERTAG '<' tAUTOTAG '>'                    { $$ = new std::shared_ptr<cdk::basic_type>(new cdk::reference_type(4, * new std::shared_ptr<cdk::basic_type>(new cdk::primitive_type(4, cdk::typename_type::TYPE_STRUCT)))); }
               | tPOINTERTAG '<' type     '>'                    { $$ = new std::shared_ptr<cdk::basic_type>(new cdk::reference_type(4, *$3)); }
               ;

block          : '{'                      '}'                    { $$ = new og::block_node(LINE, nullptr, nullptr); }
               | '{' declarations         '}'                    { $$ = new og::block_node(LINE, $2     , nullptr); }
               | '{'              instrs  '}'                    { $$ = new og::block_node(LINE, nullptr, $2     ); }
               | '{' declarations instrs  '}'                    { $$ = new og::block_node(LINE, $2     , $3     ); }
               ;

instr          : expr ';'                                        { $$ = new og::evaluation_node(LINE, $1);   }
               | tWRITE exps ';'                                 { $$ = new og::write_node(LINE, $2, false); }
               | tWRITELN exps ';'                               { $$ = new og::write_node(LINE, $2, true);  }
               | tBREAK                                          { $$ = new og::break_node(LINE);            }
               | tCONTINUE                                       { $$ = new og::continue_node(LINE);         }
               | tRETURN ';'                                     { $$ = new og::return_node(LINE, nullptr);  }
               | tRETURN exps ';'                                { $$ = new og::return_node(LINE, $2);       }
               | inst_condit                                     { $$ = $1; }
               | inst_iter                                       { $$ = $1; }
               | block                                           { $$ = $1; }
               ;

inst_condit    : tIF expr tTHEN instr             %prec tIFX     { $$ = new og::if_node(LINE, $2, $4);          }
               | tIF expr tTHEN instr elif_condit                { $$ = new og::if_else_node(LINE, $2, $4, $5); }
               ;

inst_iter      : tFOR      ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, nullptr, nullptr, $5); }
               | tFOR      ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, nullptr, $4     , $6); }
               | tFOR      ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, $3     , nullptr, $6); }
               | tFOR      ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, nullptr, $3     , $5     , $7); }
               | tFOR exps ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, $2     , nullptr, nullptr, $6); }
               | tFOR exps ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, $2     , nullptr, $5     , $7); }
               | tFOR exps ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, nullptr, $2     , $4     , nullptr, $7); }
               | tFOR exps ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, nullptr, $2     , $4     , $6     , $8); }
               | tFOR vars ';'      ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, nullptr, nullptr, $6); }
               | tFOR vars ';'      ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, nullptr, $5     , $7); }
               | tFOR vars ';' exps ';'      tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, $4     , nullptr, $7); }
               | tFOR vars ';' exps ';' exps tDO instr           { $$ = new og::for_node(LINE, $2     , nullptr, $4     , $6     , $8); }
               ;

expr           : tINTEGER                                        { $$ = new cdk::integer_node(LINE, $1);                                   }
               | tREAL                                           { $$ = new cdk::double_node(LINE, $1);                                    }
               | string                                          { $$ = new cdk::string_node(LINE, $1);                                    }
               | tNULLPTR                                        { $$ = new og::nullptr_node(LINE);                                        }
               | '-' expr  %prec tUNARY                          { $$ = new cdk::neg_node(LINE, $2);                                       }
               | '+' expr  %prec tUNARY                          { $$ = new og::identity_node(LINE, $2);                                   }
               | '~' expr  %prec tUNARY                          { $$ = new cdk::not_node(LINE, $2);                                       }
               | expr '+'  expr	                                 { $$ = new cdk::add_node(LINE, $1, $3);                                   }
               | expr '-'  expr	                                 { $$ = new cdk::sub_node(LINE, $1, $3);                                   }
               | expr '*'  expr	                                 { $$ = new cdk::mul_node(LINE, $1, $3);                                   }
               | expr '/'  expr	                                 { $$ = new cdk::div_node(LINE, $1, $3);                                   }
               | expr tAND expr	                                 { $$ = new cdk::and_node(LINE, $1, $3);                                   }
               | expr tOR expr	                                 { $$ = new cdk::or_node(LINE, $1, $3);                                    }
               | expr '%'  expr	                                 { $$ = new cdk::mod_node(LINE, $1, $3);                                   }
               | expr '<'  expr	                                 { $$ = new cdk::lt_node(LINE, $1, $3);                                    }
               | expr '>'  expr	                                 { $$ = new cdk::gt_node(LINE, $1, $3);                                    }
               | expr tGE  expr	                                 { $$ = new cdk::ge_node(LINE, $1, $3);                                    }
               | expr tLE  expr                                  { $$ = new cdk::le_node(LINE, $1, $3);                                    }
               | expr tNE  expr	                                 { $$ = new cdk::ne_node(LINE, $1, $3);                                    }
               | expr tEQ  expr	                                 { $$ = new cdk::eq_node(LINE, $1, $3);                                    }
               | '(' expr ')'                                    { $$ = $2;                                                                }
               | '[' expr ']'                                    { $$ = new og::allocation_node(LINE, $2);                                 }
               | tIDENTIFIER '('      ')'                        { $$ = new og::function_invocation_node(LINE, *$1, nullptr); delete $1;   }
               | tIDENTIFIER '(' exps ')'                        { $$ = new og::function_invocation_node(LINE, *$1, $3     ); delete $1;   }
               | tSIZEOF     '(' exps ')'                        { $$ = new og::sizeof_node(LINE, $3);                                     }
               | lval                                            { $$ = new cdk::rvalue_node(LINE, $1);                                    }
               | lval '?'                                        { $$ = new og::position_node(LINE, $1);                                   }
               | lval '=' expr                                   { $$ = new cdk::assignment_node(LINE, $1, $3);                            }
               ;

string         :        tSTRING                                  { $$ = $1;                                               }
               | string tSTRING                                  { $$ = new std::string(*$1 + *$2); delete $1; delete $2; }
               ;

lval           : tIDENTIFIER                                     { $$ = new cdk::variable_node(LINE, $1);        }
               | expr '[' expr ']'                               { $$ = new og::index_node(LINE, $1, $3);        }
               | tIDENTIFIER '@' tINTEGER                        { $$ = new og::index_tuple_node(LINE, *$1, $3); }
               ;



%%
