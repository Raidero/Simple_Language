%option noyywrap

%{
#define YY_DECL int yylex()

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

"int"		|
"float" 	|
"string" 	|
"timer"		|
"image"				return TYPE;

"~"	|
"<"	|
">"				return COM_OP;


"+"	|
"-"				return ADD_OP;

"||"	|
"&&"	|
"*"	|
"/"				return MUL_OP;

"compress"	|
"start"		|
"end"		|
"load"		|
"save"		|
"showTime"	|
"show"		|
"setParams"			return PROCEDURE;

[a-zA-Z]([a-zA-z]|[0-9])*	return IDENT;

[1-9][0-9]*|0			return INT_VALUE;
([1-9][0-9]*|0)"."[0-9]+	return FLOAT_VALUE;
L?\"(\\.|[^\\"])*\"		return STRING_VALUE;


%%
/*#include "Scanner.h"
#include <iostream>
#include <string>

using namespace std;
int main()
{
	yylex();
	Token t;
	Scanner scanner;
	printf("Give me your input:\n");
	while((t = (Token)yylex()) > 0)
	{
		scanner.setToken(t);
		if(t == float_value)
		{
			float *f = new float;
			*f = std::stof(yytext);
			scanner.setValue(f);
			scanner.setSize(sizeof(float));
			std::cout << *f;
		}
		else if(t == int_value)
		{
			int *i = new int;
			*i = std::stoi(yytext);
			scanner.setValue(i);
			scanner.setSize(sizeof(int));
			std::cout << *i;
		}
		else
		{
			string *s = new string;
			*s = yytext;
			scanner.setValue(s);
			scanner.setSize(s->size());	
			std::cout<< *s;
		}
		//TODO, parser should get next token and do something with it
	}
	return 0;
}*/
