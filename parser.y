%{
#include "Ast.h"
#include "Exec.h"
extern int yylex();
extern int yyparse();
void yyerror(const char* s);

#define YYPARSE_PARAM astDest
%}

%union {
	int i;
	char* s;
	double d;
	struct AstElement* ast;
}

%token<s> INT_VALUE
%token<s> FLOAT_VALUE
%token<s> STRING_VALUE
%type<ast> expr
%token<s> COM_OP
%type<s> constant
%token IF ELSE FOR IN TO VAR IMAGE
%type<ast> term
%type<ast> sim_expr
%token<s> ADD_OP
%token<s> MUL_OP
%token<s> PROCEDURE
%token<s> IDENT
//%type<s> var_value
%type<s> var_name
%type<ast> factor
%type<ast> comp_stat
%type<ast> stat
%type<ast> if_stat
%type<ast> else_stat
%type<ast> for_stat
%type<ast> assignment

%start program

%%

program: comp_stat { (*(struct AstElement**)astDest) = $1 }
;
comp_stat: { $$ = 0 }
	| comp_stat stat { $$ = makeStat($1, $2); }
;
if_stat: IF '(' expr ')' '{' comp_stat '}' { $$ = makeIf($3, $6); }
;
else_stat: ELSE '{' comp_stat '}' { $$ = $3; }
;
for_stat: FOR '(' IDENT IN INT_VALUE TO INT_VALUE ')' '{' comp_stat '}' { $$ = makeFor($3, $5, $7, $10); }
;
stat: 	PROCEDURE var_name ';' { $$ = makeFunc($1, $2); }
	| assignment ';'	{ $$ = $1; }
	| var_decl ';'		{ $$ = NULL; }
	| if_stat else_stat	{ $$ = makeIfElse($1, $2); }
	| if_stat		{ $$ = $1; }
	| for_stat		{ $$ = $1; }
;
assignment: var_name '=' expr { $$ = makeAss($1, $3); }
;
var_decl: var_decl ',' var_name { variables[level].push_back( { $3, NULL} ); }
	| VAR var_name { variables[level].push_back( { $2, NULL} ); }
;
expr:	expr COM_OP sim_expr	{ $$ = makeExp($1, $3, $2); }	
	| sim_expr { $$ = $1 }
;
sim_expr: sim_expr ADD_OP term { $$ = makeExp($1, $3, $2); }
	| term { $$ = $1 }
;
term: term MUL_OP factor { $$ = makeExp($1, $3, $2); }		
	| factor { $$ = $1 }
;
factor: var_name { $$ = makeExpByName($1); }
	| constant { $$ = makeExpByNum($1); }
	| '(' expr ')' { $$ = $2 }
;
constant: INT_VALUE { $$ = $1 } | FLOAT_VALUE { $$ = $1 } | STRING_VALUE { $$ = $1 }
;
var_name: IDENT '[' INT_VALUE ']'
	| IDENT { $$ = $1 }
;
/*var_value: IDENT '[' INT_VALUE ']'
	| IDENT {	bool found = false; 	
			for(int i = 0; i < variables[level].size(); ++i)
			{
				if(!strcmp(variables[level][i].first, $1))
				{
					if(variables[level][i].second == NULL)
					{
						printf("Variable has no value: %s", $1);
						exit(0);
					}
					$$ = variables[level][i].second;
					found = true;
					break;
				}
			}
			if(!found)
			{
				printf("Didn't find given ident: %s", $1);
				exit(0);
			}
		}
;*/

%%

int main()
{
	struct AstElement* element = 0;
	yyparse(&element);
	execCompStat(element);
	
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}


