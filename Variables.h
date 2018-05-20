#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int level = 0;
std::vector<std::pair<char*, char*> > variables[10];


char* ftoa(double d)
{
	std::ostringstream ss;
	ss << d;
	std::string tmp = ss.str();
	
	return strdup(tmp.c_str());
}
