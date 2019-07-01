#include <sys/vfs.h>
#include <sys/statfs.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "monitor.h"
#include "mysql_info.h"
//#include "sigar_info.h"
#include "json/json.h"

#define MAX_PARAM_LEN	2048
using namespace std;

extern "C"
bool vpxChanged(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;
	
	cout << "# vpxChanged func~" << endl;
	changed_flag_t data;
	//if(!get_mysql_changed_flag(data))
	if(!get_mysql_data(&data, CHANGED_FLAG_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Read data from mysql error !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	
	pt = data.dump(pt);
	nRetSize = (int)(pt-retBuf);
		
	return true;
}

extern "C"
bool vpxDevice(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;
	STRINGMAP smap;
	if(!ParseParameter(strParam, 0, smap))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Parse parameter failed");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	
	cout << "# vpxDevice func~ " << endl;
	device_info_t dev;
	//if(!get_mysql_device(dev))
	if(!get_mysql_data(&dev, DEVICE_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Read data from mysql error !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	/*
	sigar_info_t info;
	get_sigar_info(info);
	dev.mainCPUUsage = info.cpu_used_percent;
	dev.mainMemUsage = info.mem_used_percent;
	dev.mainMemTotal = info.mem_total;
	dev.mainHarddiskUsage = info.hard_disk_used_percent;
	dev.mainHarddiskTotal = info.hard_disk_total;
	*/
	pt = dev.dump(pt);
	nRetSize = (int)(pt-retBuf);
	return true;
}

extern "C"
bool vpxDeviceJson(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;
	STRINGMAP smap;
	if(!ParseParameter(strParam, 0, smap))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Parse parameter failed");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	
	cout << "# vpxDeviceJson func~ " << endl;
	device_info_t dev;
	//if(!get_mysql_device(dev))
	if(!get_mysql_data(&dev, DEVICE_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Read data from mysql error !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	pt = dev.dumpjson(pt);
	nRetSize = (int)(pt-retBuf);
	return true;
}

extern "C"
bool vpxInterfaceList(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;

	cout << "# vpxInterfaceList func~ " << endl;
	vector<interface_info_t > data;
	//if(!get_mysql_interface(data))
	if(!get_mysql_data(&data, INTERFACE_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Read data from mysql error !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	char szOut[128];
	for(unsigned int i=0; i<data.size(); i++)
	{
		cout << "# vpxInterfaceList index: " <<data[i].Index  << endl;
		sprintf(szOut, "%d", data[i].Index);
		M_MAKERETURN_ITEM_S(pt, szOut, data[i].Name.c_str());
	}
	cout << "# vpxInterfaceList finished func " << endl;
	pt[0] ='\0';
	pt++;

	nRetSize=(int)(pt-retBuf);
	return true;
}

extern "C"
bool vpxInterfaceStatus(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;
	STRINGMAP smap;
	if(!ParseParameter(strParam, 0, smap))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Parse parameter failed");
		nRetSize=(int)(pt-retBuf);
		return false;
	}

	cout << "# vpxInterfaceStatus func~ " << endl;

	STRINGMAP::iterator it;
	it = smap.find(INTERFACE_INDEX);
	if(it == smap.end())
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Parse parameter failed");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	int interface_index = atoi(it->second.c_str());

	vector<interface_info_t > data;
	//if(!get_mysql_interface(data))
	if(!get_mysql_data(&data, INTERFACE_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Read data from mysql error !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	bool found = false;
	interface_info_t inter_row;
	for(unsigned int i=0; i<data.size(); i++)
	{
		if (interface_index == data[i].Index)
		{
			found = true;
			inter_row = data[i];
			break;
		}
	}
	if(!found)
	{
		cout << "No found interface_index: " << interface_index << endl;
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"No found index !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	pt = inter_row.dump(pt);
	nRetSize = (int)(pt-retBuf);
	return true;
}

extern "C"
bool vpxInterfaceJson(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;

	cout << "# vpxInterfaceJson func~ " << endl;
	vector<interface_info_t > data;
	//if(!get_mysql_interface(data))
	if(!get_mysql_data(&data, INTERFACE_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Read data from mysql error !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}

   	string strJson = dump_interface(data);
	cout << "# vpxInterfaceJson: " << strJson << endl;
 	sprintf(pt, "%s", strJson.c_str());
	pt+=strlen(pt);
	pt[0]='\0';
	pt++;
	nRetSize=(int)(pt-retBuf);
	return true;
}

extern "C"
bool vpxBusinessList(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;

	cout << "# vpxBusinessList func~ " << endl;
	vector<business_info_t > data;
	//if(!get_mysql_business(data))
	if(!get_mysql_data(&data, BUSINESS_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Read data from mysql error !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	char szOut[128];
	for(unsigned int i=0; i<data.size(); i++)
	{
		sprintf(szOut, "%d", data[i].Index);
		M_MAKERETURN_ITEM_S(pt, szOut, data[i].Name.c_str());
	}
	pt[0] ='\0';
	pt++;

	nRetSize=(int)(pt-retBuf);
	return true;
}

extern "C"
bool vpxBusinessStatus(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;
	STRINGMAP smap;
	if(!ParseParameter(strParam, 0, smap))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Parse parameter failed");
		nRetSize=(int)(pt-retBuf);
		return false;
	}

	cout << "# vpxBusinessStatus func~ " << endl;
	STRINGMAP::iterator it;
	it = smap.find(BUSINESS_INDEX);
	if(it == smap.end())
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Parse parameter failed");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	int business_index = atoi(it->second.c_str());

	vector<business_info_t > data;
	//if(!get_mysql_business(data))
	if(!get_mysql_data(&data, BUSINESS_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Read data from mysql error !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	bool found = false;
	business_info_t row;
	for(unsigned int i=0; i<data.size(); i++)
	{
		if (business_index == data[i].Index)
		{
			found = true;
			row = data[i];
			break;
		}
	}
	if(!found)
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"No found index !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	pt = row.dump(pt);
	nRetSize = (int)(pt-retBuf);
	return true;
}

extern "C"
bool vpxBusinessJson(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;

	cout << "# vpxBusinessJson func~ " << endl;
	VEC_BI data;
	//if(!get_mysql_business(data))
	if(!get_mysql_data(&data, BUSINESS_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Read data from mysql error !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
   	string strJson = dump_business(data);
	cout << "# vpxBusinessJson~: " << strJson << endl;
 	sprintf(pt, "%s", strJson.c_str());
	pt+=strlen(pt);
	pt[0]='\0';
	pt++;
	nRetSize=(int)(pt-retBuf);
	return true;
}

extern "C"
bool vpxGetNeighbourJson(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;

	cout << "# vpxGetNeighbourJson func~ " << endl;
	vector<neighbour_info_t > data;
	//if(!get_mysql_neighbour(data))
	if(!get_mysql_data(&data, NEIGHBOUR_TABLE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"No found index !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
    
   	string strJson = dump_neighbour(data);
	cout << "# vpxGetNeighbourJson: " << strJson << endl;
 	sprintf(pt, "%s", strJson.c_str());
	pt+=strlen(pt);
	pt[0]='\0';
	pt++;
	nRetSize=(int)(pt-retBuf);
	return true;
}

extern "C"
bool vpxSetBusinessPathIP(const char *strParam, int paramLen, char *retBuf, int &nRetSize, const char *addBuf, int addbufLen, int sock)
{
	char *pt = retBuf;
	STRINGMAP smap;
	if(!ParseParameter(strParam, 0, smap))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"Parse parameter failed");
		nRetSize=(int)(pt-retBuf);
		return false;
	}

	cout << "# vpxSetBusinessPathIP func~ " << endl;
	if(!get_mysql_data(&smap, BUSINESS_UPDATE))
	{
		M_MAKERETURN_ITEM_S_END(pt,AM_RETURN_ERROR_FLAG,"No found index !!!!");
		nRetSize=(int)(pt-retBuf);
		return false;
	}
	nRetSize = 0;
	return true;
}

/**
* Usage example for Driver, Connection, (simple) Statement, ResultSet
*/
	/*
int main(int argc, const char **argv)
{
    string deviceSN = "SN";
    float mainCPUUsage = 1.0;
    int mainMemTotal = 0;
    cout << "deviceSN:" << varName(deviceSN)<< endl;
    cout << "mainCPUUsage:" << varName(mainCPUUsage)<< endl;
    cout << "mainMemTotal:" << varName(mainMemTotal)<< endl;

    changed_flag_t row;
    row.device_stamp=1537340761;
    row.neighbour_stamp=1537340761;
    row.interface_stamp =1537340761;
    char buff[10240];
    row.dump(buff);

	sigar_info_t info;
	get_sigar_info(info);
	cout << "sigar data:" << endl;
	cout << "cpu_used_percent:"  << info.cpu_used_percent << endl;
	cout << "mem_used_percent:" << info.mem_used_percent << endl;
	cout << "mem_total:" << info.mem_total << endl;
	cout << "hard_disk_used_percent:" << info.hard_disk_used_percent << endl;
	cout << "hard_disk_total:" << info.hard_disk_total << endl;
	
	return EXIT_SUCCESS;
}

	*/
