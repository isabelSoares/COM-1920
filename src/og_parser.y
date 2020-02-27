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
  int                   i;	/* integer value */
  double               r;
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
};

%token <i> tINTEGER
%token <r> tREAL
%token <s> tIDENTIFIER tSTRING
%token tFOR tIF tPRINT tINPUT

%nonassoc tIFX
%nonassoc tELSE

%right '='
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> stmt program
%type <sequence> list
%type <expression> expr
%type <lvalue> lval

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

program	: tBEGIN list tEND { compiler->ast(new og::program_node(LINE, $2)); }
	      ;

list : stmt	     { $$ = new cdk::sequence_node(LINE, $1); }
	| list stmt { $$ = new cdk::sequence_node(LINE, $2, $1); }
	;

stmt : expr ';'                                       { $$ = new og::evaluation_node(LINE, $1); }
 	| tWRITE exps ';'                                { $$ = new og::write_node(LINE, $2); }
 	| tWRITELN exps ';'                              { $$ = new og::writeln_node(LINE, $2); }
 	| tBREAK exps ';'                                { $$ = new og::break_node(LINE, $2); }
 	| tCONTINUE exps ';'                             { $$ = new og::continue_node(LINE, $2); }
 	| tRETURN exps ';'                               { $$ = new og::return_node(LINE, $2); }
     | tIF expr tTHEN stmt                            { $$ = new og::if_node(LINE, $3, $5); }
     | tIF expr stmt mid_conditional tELSE stmt       { /* TODO */ }
     | tIF expr stmt tELSE stmt                       { $$ = new og::if_else_node(LINE, $3, $5, $7); }
     | tFOR '(' exps ';' exps ';' exps ')' tDO stmt   { $$ = new og::for_node(LINE, $3, $5, $7, $9); }
     | tFOR '(' vars ';' exps ';' exps ')' tDO stmt   { $$ = new og::for_node(LINE, $3, $5, $7, $9); }
     | '{' list '}'                                   { $$ = $2; }
     ;

mid_conditional     : tELIF expr tTHEN stmt                           { /* TODO */ }
                    | mid_conditional tELIF expr tTHEN stmt           { /* TODO */ }
                    ;

ids  : id                                              { $$ = new cdk::sequence_node(LINE, $1)}
     | id ',' id                                       { $$ = new cdk::sequence_node(LINE, $1, $3)}

vars : var                                             { $$ = new cdk::sequence_node(LINE, $1)}
     | var ',' var                                     { $$ = new cdk::sequence_node(LINE, $1, $3)}
     
exps : expr                                            { $$ = new cdk::sequence_node(LINE, $1)}
     | expr ',' expr                                   { $$ = new cdk::sequence_node(LINE, $1, $3)}

expr : tINTEGER                { $$ = new cdk::integer_node(LINE, $1); }
	   | tSTRING                 { $$ = new cdk::string_node(LINE, $1); }
     | '-' expr %prec tUNARY   { $$ = new cdk::neg_node(LINE, $2); }
     | expr '+' expr	         { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	         { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	         { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	         { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	         { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr	         { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	         { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	         { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr           { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	         { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	         { $$ = new cdk::eq_node(LINE, $1, $3); }
     | '(' expr ')'            { $$ = $2; }
     | lval                    { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
     | lval '=' expr           { $$ = new cdk::assignment_node(LINE, $1, $3); }
     ;

lval : tIDENTIFIER             { $$ = new cdk::variable_node(LINE, $1); }
     ;

%%
