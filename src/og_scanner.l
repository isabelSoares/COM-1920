%option c++ prefix="og_scanner_" outfile="og_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "og_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT
%%

"&&"                   return tAND; 
"||"                   return tOR; 

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE; 

"for"                  return tFOR; 
"if"                   return tIF; 
"else"                 return tELSE;
"input"                return tINPUT; 
"sizeof"               return tSIZEOF;

"do"                   return tDO;
"then"                 return tTHEN;
"elif"                 return tELIF;
"write"                return tWRITE;
"writeln"              return tWRITELN;
"break"                return tBREAK;
"continue"             return tCONTINUE;
"return"               return tRETURN;
"public"               return tPUBLIC;
"require"              return tREQUIRE;
"procedure"            return tPROCEDURE;

"int"                  return tINTTAG;
"real"                 return tREALTAG;
"string"               return tSTRINGTAG;
"auto"                 return tAUTOTAG;
"ptr"                  return tPOINTERTAG;

[A-Za-z][A-Za-z0-9_]*                   yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                                      yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"                            yy_pop_state(); return tSTRING;
<X_STRING>\\\"                          *yylval.s += yytext + 1;
<X_STRING>.                             *yylval.s += yytext;
<X_STRING>\n                            yyerror("newline in string");

[0-9]+                                  {
                                            try {
                                                yylval.i = std::stoi(yytext, nullptr, 10);
                                                return tINTEGER;
                                            } catch (const std::invalid_argument& e) {
                                                yyerror("not an integer");
                                            } catch (const std::out_of_range& e) {
                                                yyerror("integer out of range");
                                            }
                                        }
"0x"[A-Fa-f0-9]+                        {
                                            try {
                                                yylval.i = std::stoi(yytext, nullptr, 0);
                                                return tINTEGER;
                                            } catch (const std::invalid_argument& e) {
                                                yyerror("not an integer");
                                            } catch (const std::out_of_range& e) {
                                                yyerror("integer out of range");
                                            }
                                        }

[0-9]*\.[0-9]+([Ee][-+]?[0-9]+)?        {
                                            try {
                                                yylval.r = std::stod(yytext);
                                                return tREAL;
                                            } catch (const std::invalid_argument& e) {
                                                yyerror("not an double");
                                            } catch (const std::out_of_range& e) {
                                                yyerror("double out of range");
                                            }
                                        }
[0-9]+\.[0-9]*([Ee][-+]?[0-9]+)?        {
                                            try {
                                                yylval.r = std::stod(yytext);
                                                return tREAL;
                                            } catch (const std::invalid_argument& e) {
                                                yyerror("not an double");
                                            } catch (const std::out_of_range& e) {
                                                yyerror("double out of range");
                                            }
                                        }
[0-9]+([Ee][-+]?[0-9]+)                 {
                                            try {
                                                yylval.r = std::stod(yytext);
                                                return tREAL;
                                            } catch (const std::invalid_argument& e) {
                                                yyerror("not an double");
                                            } catch (const std::out_of_range& e) {
                                                yyerror("double out of range");
                                            }
                                        }

"/*"                                    yy_push_state(X_COMMENT);
<X_COMMENT>"/*"                         yy_push_state(X_COMMENT);
<X_COMMENT>"*/"                         yy_pop_state();
<X_COMMENT>.|\n                         ;

"//".*$                                 ;

[-()<>=+*/%;{}\[\]?@,.]                 return *yytext;

[ \t\n]+                                ; /* ignore whitespace */

.                                       yyerror("Unknown character");

%%