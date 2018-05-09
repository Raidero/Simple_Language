%{
#include <stdio.h>
#include <stdlib.h>
//#include "parser.h"

extern int yylex();
extern int yyparse();
//extern FILE* yyin;
struct
{
	union {
		double d;
		int i;
		char* s;
	} val;
	int type;
} symbol_table[100];
void yyerror(const char* s);
%}

%union {
	int i;
	char* s;
	double d;
	struct
	{
		double d;
		int i;
		char* s;
	} value;
}

%token<i> INT_VALUE
%token<d> FLOAT_VALUE
%token<s> STRING_VALUE
%type<d> expr
%token<s> COM_OP
%type<value> constant;
%token IF ELSE FOR PROCEDURE ADD_OP MUL_OP IDENT IN TO TYPE

%start comp_stat

%%
comp_stat: 
	| comp_stat stat
	| comp_stat struct_stat
;
struct_stat: if_stat else_stat
	| if_stat
	| for_stat
;
if_stat: IF '(' expr ')' '{' comp_stat '}'
;
else_stat: ELSE '{' comp_stat '}'
;
for_stat: FOR '(' IDENT IN INT_VALUE TO INT_VALUE ')' '{' comp_stat '}'
;
stat: 	PROCEDURE var_name ';'
	| assignment ';'
	| var_decl ';'
;
assignment: IDENT '=' expr
;
var_decl: var_decl ',' var_name
	| TYPE var_name
;
expr:	expr COM_OP sim_expr	/*{ switch($2)
					{
						case '~':
						if((int)$1 == (int)$2)
						 	$$ = 1;
						else
							$$ = 0;
						printf("lel");
						break;
					}
				}*/	
	| sim_expr
;
sim_expr: sim_expr ADD_OP term
	| term
;
term: term MUL_OP factor
	| factor
;
factor: var_name
	| constant { printf("XD"); }
	| '(' expr ')'
;
constant: INT_VALUE { $$.i = $1 } | FLOAT_VALUE { $$.d = $1 } | STRING_VALUE { $$.s = $1 }
;
var_name: IDENT '[' INT_VALUE ']'
	| IDENT
;

%%

int main()
{
	yyparse();
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}


