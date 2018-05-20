%{
#include "Variables.h"

extern int yylex();
extern int yyparse();
void yyerror(const char* s);
%}

%union {
	int i;
	char* s;
	double d;
}

%token<s> INT_VALUE
%token<s> FLOAT_VALUE
%token<s> STRING_VALUE
%type<s> expr
%token<s> COM_OP
%type<s> constant
%token IF ELSE FOR IN TO VAR IMAGE
%type<s> term
%type<s> sim_expr
%token<s> ADD_OP
%token<s> MUL_OP
%token<s> PROCEDURE
%token<s> IDENT
%type<s> var_value
%type<s> var_name
%type<s> factor

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
stat: 	PROCEDURE var_value ';' { 	if(!strcmp($1, "show"))
					{
						printf("%s\n", $2);
					}
				}
	| assignment ';'
	| var_decl ';'
;
assignment: var_name '=' expr { 	bool found = false;
					for(int i = 0; i < variables[level].size(); ++i)
					{
						if(!strcmp(variables[level][i].first, $1))
						{
							variables[level][i].second = $3;
							found = true;
							break;
						}
					}
					if(!found)
					{
						printf("Didn't find given ident: %s\n", $1);
						exit(0);
					}
				}
;
var_decl: var_decl ',' var_name { variables[level].push_back( { $3, NULL} ); }
	| VAR var_name { variables[level].push_back( { $2, NULL} ); }
;
expr:	expr COM_OP sim_expr	{ 	double d1 = atof($1);
					double d2 = atof($3);
					if(d1 == 0.0 && $3[0] != '0')
					{
						d1 = strcmp($1, $3);
						if(d1 < 0 and $2[0] == '<')
							$$ = ftoa(1);
						else if(d1 > 0 and $2[0] == '>')
							$$ = ftoa(1);
						else if(d1 == 0 and $2[0] == '~')
							$$ = ftoa(1);
						else $$ = ftoa(0);
					}
					else
					{
						switch((int)$2[0])
						{
							case '>':
							d1 = d1 > d2;
							break;
							case '<':
							d1 = d1 < d2;
							break;
							case '~':
							d1 = d1 == d2;
							break;
						}
						$$ = ftoa(d1);
					}
				}	
	| sim_expr { $$ = $1 }
;
sim_expr: sim_expr ADD_OP term { 	if(atof($3) == 0.0 && $3[0] != '0')
					{
						printf("Factor is a string, cannot add or substract\n");
						exit(0);
					}
					double d1 = atof($1);
					double d2 = atof($3);
					switch((int)$2[0])
					{
						case '+':
						d1 = d1 + d2;
						break;
						case '-':
						d1 = d1 - d2;
						break;
					}
					$$ = ftoa(d1);
				}
	| term { $$ = $1 }
;
term: term MUL_OP factor { 	if(atof($3) == 0.0 && $3[0] != '0')
				{
					printf("Factor is a string, cannot multiply, divide or made OR or AND operations\n");
					exit(0);
				}
				double d1 = atof($1);
				double d2 = atof($3);
				switch((int)$2[0])
				{
					case '/':
					d1 = d1 / d2;
					break;
					case '*':
					d1 = d1 * d2;
					break;
					case '&':
					d1 = d1 && d2;
					break;
					case '|':
					d1 = d1 || d2;
					break;
				}
				$$ = ftoa(d1);
			}	
	| factor { $$ = $1 }
;
factor: var_value { $$ = $1 }
	| constant { $$ = $1 }
	| '(' expr ')' { $$ = $2 }
;
constant: INT_VALUE { $$ = $1 } | FLOAT_VALUE { $$ = $1 } | STRING_VALUE { $$ = $1 }
;
var_name: IDENT '[' INT_VALUE ']'
	| IDENT { $$ = $1 }
;
var_value: IDENT '[' INT_VALUE ']'
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


