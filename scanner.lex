%{
#include "Scanner.h"
%}

%%
"if"				return if_token;
"for"				return for_token;
"in"				return in;
"to"				return to;
"("				return bracket_open;
")"				return bracket_close;
"{"				return brace_open;
"}"				return brace_close;
"["				return square_bracket_open;
"]"				return square_bracket_close;
";"				return semicolon;
","				return comma;
"int"				return int_token;
"float"				return float_token;
"string"			return string_token;
"timer"				return timer;
"image"				return image;
"=="				return equal_token;
"="				return assignment;
"<"				return less_token;
">"				return more_token;
"+"				return plus_token;
"-"				return minus_token;
"||"				return or_token;
"&&"				return and_token;
"*"				return multiply;
"/"				return divide;
"compress"			return compressf;
"start"				return startf;
"end"				return endf;
"load"				return loadf;
"save"				return savef;
"showTime"			return showTimef;
"show"				return showf;
"setParams"			return setParamsf;
[1-9][0-9]*|0			return int_value;
([1-9][0-9]*|0)"."[0-9]+	return float_value;
L?\"(\\.|[^\\"])*\"		return string_value;
[a-zA-Z]([a-zA-z]|[0-9])*	return ident;

%%
#include "Scanner.h"
#include <iostream>
#include <string>

using namespace std;
int main()
{
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
}
