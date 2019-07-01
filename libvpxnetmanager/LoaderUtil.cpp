#include "LoaderUtil.h"
#include <string.h>

#define AGENT_PARAMETER_SEPARATOR		'='

int GetParamDataLen(const char *pData)
{
	char *p=(char*)pData;
	while(*p!=0)
	{
		p+=strlen(p)+1;

	}
	return (int)(p-pData+1);

}

bool ParseParameter(char *pData,int dLen,STRINGMAP &sMap)
{
	char *pt=pData;
	char *str=pt;
	std::string name,value;
	while(*str)
	{
		if((*str)==AGENT_PARAMETER_SEPARATOR)
		{
			*str='\0';
			name=pt;
			pt=str+1;
			value=pt;
			sMap[name]=value;
			pt+=value.size()+1;
			str=pt;
			if((*str)=='\0')
				break;
		}
		else
			str++;

		//if((*str)=='\0')
		//{
		//	value=pt;
		//	sMap[name]=value;
		//	pt=str+1;
		//	if((*pt)=='\0')
		//	{
		//		break;
		//	}
		//}
	}

	return true;

}
bool ParseParameter(const char *pData,int dLen,STRINGMAP &sMap)
{

	if(pData==NULL)
		return false;
	if(dLen<1)
	{
		dLen=GetParamDataLen(pData);
		if(dLen<1)
			return false;
	}
	char *pd=new char[dLen];
	if(pd==NULL)
		return false;
	memcpy(pd,pData,dLen);

	bool bret=ParseParameter(pd,dLen,sMap);

	delete []pd;
	return bret;
}

