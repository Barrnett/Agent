
#ifndef LOADERLIB_LOADERUTIL_H_
#define LOADERLIB_LOADERUTIL_H_


#include <iostream>
#include <string>
#include <map>

//using namespace std;

typedef std::map<std::string , std::string > STRINGMAP;

bool ParseParameter(char *pData,int dLen,STRINGMAP &sMap);

bool ParseParameter(const char *pData,int dLen,STRINGMAP &sMap);
int GetParamDataLen(const char *pData);


#endif
