#include "Ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int level = 0;
std::vector<std::pair<char*, char*> > variables[10];
static void* checkAlloc(size_t sz)
{
    void* result = calloc(sz, 1);
    if(!result)
    {
        fprintf(stderr, "alloc failed\n");
        exit(1);
    }
}

struct AstElement* makeAss(char* name, struct AstElement* val)
{
    struct AstElement* result = checkAlloc(sizeof(*result));
    result->kind = ekAssignment;
    result->data.assignment.name = name;
    result->data.assignment.right = val;
    return result;
}

struct AstElement* makeExpByNum(char* val)
{
    struct AstElement* result = checkAlloc(sizeof(*result));
    result->kind = ekNumber;
    result->data.val = val;
    return result;
}

struct AstElement* makeExpByName(char* name)
{
    struct AstElement* result = checkAlloc(sizeof(*result));
    result->kind = ekId;
    result->data.name = name;
    return result;
}

struct AstElement* makeExp(struct AstElement* left, struct AstElement* right, char* op)
{
    struct AstElement* result = checkAlloc(sizeof(*result));
    result->kind = ekBinExpression;
    result->data.expression.left = left;
    result->data.expression.right = right;
    result->data.expression.op = op;
    return result;
}

struct AstElement* makeStat(struct AstElement* result, struct AstElement* toAppend)
{
    if(toAppend != NULL)
    {
	    if(!result)
	    {
		result = checkAlloc(sizeof(*result));
		result->kind = ekStatements;
		result->data.statements.count = 0;
		result->data.statements.statements = 0;
	    }
	    assert(ekStatements == result->kind);
	    result->data.statements.count++;
	    result->data.statements.statements = realloc(result->data.statements.statements, result->data.statements.count*sizeof(*result->data.statements.statements));
	    result->data.statements.statements[result->data.statements.count-1] = toAppend;
	    return result;
    }
    return result;
}

struct AstElement* makeFor(char* id, char* from, char* to, struct AstElement* exec)
{
    struct AstElement* result = checkAlloc(sizeof(*result));
    result->kind = ekWhile;
    variables[level].push_back({id, from});
    result->data.whileStmt.name = id;
    result->data.whileStmt.from = from;
    result->data.whileStmt.to = to;
    result->data.whileStmt.statements = exec;
    return result;
}

struct AstElement* makeFunc(char* name, char* param)
{
    struct AstElement* result = checkAlloc(sizeof(*result));
    result->kind = ekCall;
    result->data.call.name = name;
    result->data.call.param = param;
    return result;
}

struct AstElement* makeIf(struct AstElement* cond, struct AstElement* exec)
{
    struct AstElement* result = checkAlloc(sizeof(*result));
    result->kind = ekIf;
    result->data.ifStmt.cond = cond;
    result->data.ifStmt.statements = exec;
    return result;
}
struct AstElement* makeIfElse(struct AstElement* ekif, struct AstElement* ekelse)
{
    struct AstElement* result = checkAlloc(sizeof(*result));
    result->kind = ekIfElse;
    result->data.ifElseStmt.cond = ekif->data.ifStmt.cond;
    result->data.ifElseStmt.ifstatements = ekif->data.ifStmt.statements;
    result->data.ifElseStmt.elsestatements = ekelse;
    return result;
}

char* ftoa(double d)
{
	std::ostringstream ss;
	ss << d;
	std::string tmp = ss.str();
	
	return strdup(tmp.c_str());
}
