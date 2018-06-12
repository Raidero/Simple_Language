#include "Exec.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>

void loadImage(char* filename, int dst);

void execCompStat(struct AstElement* ast)
{
	for(int i = 0; i < ast->data.statements.count; ++i)
	{	
		if(ast->data.statements.statements[i] != NULL)
			execStat(ast->data.statements.statements[i]);
	}
}


void execStat(struct AstElement* ast)
{
	int x = ast->kind;
	switch(x)
	{
		case ekCall:
			execProc(ast->data.call.name, ast->data.call.param1, ast->data.call.param2);
			break;
		case ekAssignment:
			execAssignment(ast->data.assignment.name, ast->data.assignment.right);
			break;
		case ekIfElse:
			execIfElseStat(ast->data.ifElseStmt.cond, ast->data.ifElseStmt.ifstatements, ast->data.ifElseStmt.elsestatements);
			break;
		case ekIf:
			execIfStat(ast->data.ifStmt.cond, ast->data.ifStmt.statements);
			break;
		case ekWhile:
			execWhileStat(ast->data.whileStmt.name, ast->data.whileStmt.from, ast->data.whileStmt.to, ast->data.whileStmt.statements);
			break;
	}
}

void execProc(char* name, char* param1, char* param2)
{
	if(!strcmp("show", name))
	{
		bool found = false;
		for(int i = 0; i < variables[level].size(); ++i)
		{
			if(!strcmp(variables[level][i].first, param1))
			{
				std::cout << variables[level][i].second << '\n';
				found = true;
				break;
			}
		}
		if(!found)
		{
			printf("Didn't find given ident: %s\n", param1);
			exit(0);
		}
	}
	else if(!strcmp("load", name))
	{
		bool found1 = false, found2 = false;
		int index;
		char* filename;
		for(int i = 0; i < variables[level].size(); ++i)
		{
			if(!strcmp(variables[level][i].first, param1))
			{
				filename = variables[level][i].second;
				found1 = true;
			}
			if(!strcmp(variables[level][i].first, param2))
			{
				index = i;
				found2 = true;
			}
		}
		if(!found1 || !found2)
		{
			printf("Didn't find given idents: %s %s\n", param1, param2);
			exit(0);
		}
		loadImage(filename, index);
	}
	else if(!strcmp("getParams", name))
	{
		bool found = false;
		for(int i = 0; i < variables[level].size(); ++i)
		{
			if(!strcmp(variables[level][i].first, param1))
			{
				std::cout <<"File name: "<< reinterpret_cast<Image*>(variables[level][i].second)->filename << '\n';
				std::cout <<"Width: " << reinterpret_cast<Image*>(variables[level][i].second)->width << '\n';
				std::cout <<"Height: " << reinterpret_cast<Image*>(variables[level][i].second)->height << '\n';
				found = true;
				break;
			}
		}
		if(!found)
		{
			printf("Didn't find given ident: %s\n", param1);
			exit(0);
		}
	}
}

void execAssignment(char* name, struct AstElement* ast)
{
	int x = ast->kind;
	switch(x)
	{
		case ekId:
			execAssignName(name, ast);
			break;
		case ekNumber:
			execAssignValue(name, ast);
			break;
		case ekBinExpression:
			char* val = execExpr(ast->data.expression.left, ast->data.expression.right, ast->data.expression.op);
			bool found = false;
			for(int i = 0; i < variables[level].size(); ++i)
			{
				if(!strcmp(variables[level][i].first, name))
				{
					variables[level][i].second = val;
					found = true;
					break;
				}
			}
			if(!found)
			{
				printf("Didn't find given ident: %s\n", name);
				exit(0);
			}
			break;
	}
}

void execAssignName(char* name, struct AstElement* ast)
{
	bool found = false;
	char* val;
	for(int i = 0; i < variables[level].size(); ++i)
	{
		if(!strcmp(variables[level][i].first, ast->data.name))
		{
			val = variables[level][i].second;
			found = true;
			break;
		}
	}
	if(!found)
	{
		printf("Didn't find given ident: %s\n", ast->data.name);
		exit(0);
	}
	found = false;
	for(int i = 0; i < variables[level].size(); ++i)
	{
		if(!strcmp(variables[level][i].first, name))
		{
			variables[level][i].second = val;
			found = true;
			break;
		}
	}
	if(!found)
	{
		printf("Didn't find given ident: %s\n", name);
		exit(0);
	}
}

void execAssignValue(char* name, struct AstElement* ast)
{
	bool found = false;
	for(int i = 0; i < variables[level].size(); ++i)
	{
		if(!strcmp(variables[level][i].first, name))
		{
			variables[level][i].second = ast->data.val;
			found = true;
			break;
		}
	}
	if(!found)
	{
		printf("Didn't find given ident: %s\n", name);
		exit(0);
	}
}

char* execExpr(struct AstElement* left, struct AstElement* right, char* op)
{
	char* leftval, *rightval;
	if(left->kind == ekBinExpression)
	{
		leftval = execExpr(left->data.expression.left, left->data.expression.right, left->data.expression.op);
	}
	else if(left->kind == ekNumber)
	{
		leftval = left->data.val;
	}
	else if(left->kind == ekId)
	{
		bool found = false;
		for(int i = 0; i < variables[level].size(); ++i)
		{
			if(!strcmp(variables[level][i].first, left->data.name))
			{
				leftval = variables[level][i].second;
				found = true;
				break;
			}
		}
		if(!found)
		{
			printf("Didn't find given ident: %s\n", left->data.name);
			exit(0);
		}
	}
	if(right->kind == ekBinExpression)
	{
		rightval = execExpr(right->data.expression.left, right->data.expression.right, right->data.expression.op);
	}
	else if(right->kind == ekNumber)
	{
		rightval = right->data.val;
	}
	else if(right->kind == ekId)
	{
		bool found = false;
		for(int i = 0; i < variables[level].size(); ++i)
		{
			if(!strcmp(variables[level][i].first, right->data.name))
			{
				rightval = variables[level][i].second;
				found = true;
				break;
			}
		}
		if(!found)
		{
			printf("Didn't find given ident: %s\n", right->data.name);
			exit(0);
		}
	}
	double d1 = atof(leftval);
	double d2 = atof(rightval);
	switch((int)op[0])
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
		case '+':
		d1 = d1 + d2;
		break;
		case '-':
		d1 = d1 - d2;
		break;
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
	return ftoa(d1);
}

void execIfElseStat(struct AstElement* cond, struct AstElement* ifstats, struct AstElement* elsestats)
{
	bool con = false;
	int x = cond->kind;
	bool found = false;
	switch(x)
	{
		case ekId:
			for(int i = 0; i < variables[level].size(); ++i)
			{
				if(!strcmp(variables[level][i].first, cond->data.name))
				{
					if(variables[level][i].second[0] != '0' || atof(variables[level][i].second) != 0.0)
						con = true;
					found = true;
					break;
				}
			}
			if(!found)
			{
				printf("Didn't find given ident: %s\n", cond->data.name);
				exit(0);
			}
			break;
		case ekNumber:
			if(cond->data.val[0] != '0' || atof(cond->data.val) != 0.0)
				con = true;
			break;
		case ekBinExpression:
			char* val = execExpr(cond->data.expression.left, cond->data.expression.right, cond->data.expression.op);
			if(atof(val) != 0.0)
				con = true;
			break;
	}
	if(con)
		execCompStat(ifstats);
	else
		execCompStat(elsestats);
}

void execIfStat(struct AstElement* cond, struct AstElement* ifstats)
{
	bool con = false;
	bool found = false;
	int x = cond->kind;
	switch(x)
	{
		case ekId:
			for(int i = 0; i < variables[level].size(); ++i)
			{
				if(!strcmp(variables[level][i].first, cond->data.name))
				{
					if(variables[level][i].second[0] != '0' || atof(variables[level][i].second) != 0.0)
						con = true;
					found = true;
					break;
				}
			}
			if(!found)
			{
				printf("Didn't find given ident: %s\n", cond->data.name);
				exit(0);
			}
			break;
		case ekNumber:
			if(cond->data.val[0] != '0' || atof(cond->data.val) != 0.0)
				con = true;
			break;
		case ekBinExpression:
			char* val = execExpr(cond->data.expression.left, cond->data.expression.right, cond->data.expression.op);
			if(atof(val) != 0.0)
				con = true;
			break;
	}
	if(con)
		execCompStat(ifstats);
}

void execWhileStat(char* name, char* from, char* to, struct AstElement* whileStats)
{
	bool found = false;
	int i;
	for(i = 0; i < variables[level].size(); ++i)
	{
		if(!strcmp(variables[level][i].first, name))
		{
			found = true;
			break;
		}
	}
	if(!found)
	{
		printf("Didn't find given ident: %s\n", name);
		exit(0);
	}
	int fromf = (int)atof(from);
	int tof = (int)atof(to);
	if(fromf != 0.0 && tof != 0.0)
	{
		for(int j = fromf; j <= tof; ++j)
		{
			execCompStat(whileStats);
			int ind = atof(variables[level][i].second);
			++ind;
			variables[level][i].second = ftoa(ind);
		}
	}
		
}

void loadImage(char* filename, int dst)
{
	Image* image = (Image*)malloc(sizeof(Image));
    	FILE* f = NULL;
	f = fopen(filename, "rb");
	if(f == NULL)
	{
		printf("Error file name: %s\n", filename);
		exit(0);
	}
	image->fd = f;
	image->filename = filename;

    	unsigned char info[54];
    	fread(info, sizeof(unsigned char), 54, f);
    	image->width = *(int*)&info[18];
    	image->height = *(int*)&info[22];
	variables[level][dst].second = (char*)image;
}


	
