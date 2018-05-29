#ifndef ASTEXEC_H
#define ASTEXEC_H
#include "Ast.h"
struct AstElement;

void execCompStat(struct AstElement* ast);


void execStat(struct AstElement* ast);

void execProc(char* name, char* param);

void execAssignment(char* name, struct AstElement* ast);

void execAssignName(char* name, struct AstElement* ast);

void execAssignValue(char* name, struct AstElement* ast);

char* execExpr(struct AstElement* left, struct AstElement* right, char* op);

void execIfElseStat(struct AstElement* cond, struct AstElement* ifstats, struct AstElement* elsestats);

void execIfStat(struct AstElement* cond, struct AstElement* ifstats);

void execWhileStat(char* name, char* from, char* to, struct AstElement* whileStats);

#endif
