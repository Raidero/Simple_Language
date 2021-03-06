%option noyywrap

%{
#define YY_DECL int yylex()
#include <string.h>
#include "parser.tab.h"
%}

%%

"(" |
")" |
"=" |
";" |
"{" |
"}" |
"[" |
"]" |
","				return yytext[0];


"if"				return IF;
"for"				return FOR;
"in"				return IN;
"to"				return TO;
"else"				return ELSE;

"var"				return VAR;

"~"	|
"<"	|
">"				yylval.s = strdup(yytext); return COM_OP;


"+"	|
"-"				yylval.s = strdup(yytext); return ADD_OP;

"|"	|
"&"	|
"*"	|
"/"				yylval.s = strdup(yytext); return MUL_OP;

"compress"	|
"start"		|
"end"		|
"load"		|
"save"		|
"showTime"	|
"show"		|
"getParams"			yylval.s = strdup(yytext); return PROCEDURE;

[1-9][0-9]*|0			yylval.s = strdup(yytext); return INT_VALUE;
([1-9][0-9]*|0)"."[0-9]+	yylval.s = strdup(yytext); return FLOAT_VALUE;
\"(\\.|[^\\"])*\"		{  yylval.s = strdup(yytext + 1);
				   yylval.s[yyleng-2] = 0;
				  return STRING_VALUE;
				}
[a-zA-Z]([a-zA-z]|[0-9])*	yylval.s = strdup(yytext); return IDENT;

.				{ }
%%
