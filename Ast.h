#ifndef ASTGEN_H
#define ASTGEN_H
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum Kind {ekId, ekNumber, ekBinExpression, ekAssignment, ekWhile, ekCall, ekStatements, ekLastElement, ekIf, ekIfElse};
extern int level;
extern std::vector<std::pair<char*, char*> > variables[10];

struct AstElement
{
    Kind kind;
    union
    {
        char* val;
        char* name;
        struct
        {
            struct AstElement *left, *right;
            char* op;
        }expression;
        struct
        {
            char*name;
            struct AstElement* right;
        }assignment;
        struct
        {
            int count;
            struct AstElement** statements;
        }statements;
        struct
        {
            char* name;
	    char* from;
	    char* to;
            struct AstElement* statements;
        } whileStmt;
        struct
        {
            char* name;
            char* param1;
	    char* param2;
        }call;
	struct
	{
	    struct AstElement* cond;
	    struct AstElement* statements;
	}ifStmt;
	struct
	{
	    struct AstElement* statements;
	}elseStmt;
	struct
	{
	    struct AstElement* cond;
	    struct AstElement* ifstatements;
	    struct AstElement* elsestatements;
	}ifElseStmt;
    } data;
};

struct AstElement* makeAss(char* name, struct AstElement* val);
struct AstElement* makeExpByNum(char* val);
struct AstElement* makeExpByName(char* name);
struct AstElement* makeExp(struct AstElement* left, struct AstElement* right, char* op);
struct AstElement* makeStat(struct AstElement* dest, struct AstElement* toAppend);
struct AstElement* makeFor(char* id, char* from, char* to, struct AstElement* exec);
struct AstElement* makeFunc(char* name, char* param1, char* param2);
struct AstElement* makeIf(struct AstElement* cond, struct AstElement* exec);
struct AstElement* makeElse(struct AstElement* exec);
struct AstElement* makeIfElse(struct AstElement* ekif, struct AstElement* ekelse);

char* ftoa(double d);
#endif
