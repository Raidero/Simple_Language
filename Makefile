all: language

language: lex.yy.c parser.tab.c parser.tab.h Ast.h Variables.h Exec.h
	g++ -std=c++11 -fpermissive -o language parser.tab.c lex.yy.c Ast.cpp Exec.cpp

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: scanner.l parser.tab.h
	flex scanner.l



