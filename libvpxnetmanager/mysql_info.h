
#ifndef _MYSQL_INFO_H
#define _MYSQL_INFO_H
// Standard C++ includes
#include <stdio.h>

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

/*
  Public interface of the MySQL Connector/C++.
  You might not use it but directly include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
//#include <driver/mysql_public_iface.h>
/* MySQL Connector/C++ specific headers */
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>
#include <cppconn/warning.h>

#include "json/json.h"

#include "AgentMonitorCommonDef.h"
#include "LoaderUtil.h"


using namespace std;

typedef std::auto_ptr< sql::Statement > PTR_Statement;

#define varName(x) #x


struct device_info_t {
    string deviceSN;
    string deviceCode;
    string deviceName;
    string deviceDesc;
    string deviceVersion;
    uint32_t deviceNetElemMgrIP;
    uint32_t deviceNetMgrIP;
    uint32_t deviceMonitorIP;
    //////////////////////////
    // add 2019/1/18
    uint32_t devid;
    string longitude;
    string latitude;
    ////////////////////////////
    float mainCPUUsage;
    float exchangeCPUUsage;
    float mainMemUsage;
    int mainMemTotal;
    float exchangeMemUsage;
    int exchangeMemTotal;
    float mainHarddiskUsage;
    int mainHarddiskTotal;
    int deviceStatus;
    
    char* dump(char *out);
    char* dumpjson(char *out);
};

struct interface_info_t{
    int Index;
    string Name;
    string Desc;
    int Status;
    int channelbandwidth;
    int channeltype;
    float channelerrorrate;
    uint32_t interfaceIP;
    uint32_t IPMask;
	uint32_t gateway;
	uint32_t dtperiod;
	uint32_t dtquene;
    int OSPFCost;
    float InFlow;
    float OutFlow;
    float PacketLoss;
    float TimeDelay;
    uint32_t jitter;
    float BandwidthUtilization;
    string iprepeat;

    char* dump(char *out);
} ;

struct business_info_t{
    int Index;
    string Name;
    string Desc;
	int Type;
	int Priority;
    uint32_t SourceIP;
    uint32_t SourceIPMask;
    uint32_t TargetIP;
    uint32_t TargetIPMask;
	int sptmin;
	int sptmax;
	int dstmin;
	int dstmax;
	int NetProtocol;
	int mainforward;
	string backforward;
	int backups;
	float nnibwth;
	float nniberth;
	float lostrateth;
	float avgrttth;
	
	string secondpath;
	string thirdpath;
	int pathmode;
	int dtperiod;
	int dtqueue;
	
	float outrate;
	float lostrate;
	int curpath;

	float path1lostrate;
	int path1avgrtt;
	int path1jitter;
	
	float path2lostrate;
	int path2avgrtt;
	int path2jitter;
	
	float path3lostrate;
	int path3avgrtt;
	int path3jitter;
	
	uint32_t pathdstne;

    char* dump(char *out);
    
    void dumpJson(Json::Value& val);
} ;

struct neighbour_info_t{
    uint32_t ethid;
    uint32_t	ipaddr;
    uint32_t	nexthop;
    uint32_t	neighborip;
    uint32_t	cost;
    uint32_t	avgrtt;
};

struct changed_flag_t{
    time_t device_stamp;
    time_t interface_stamp;
    //time_t business_stamp;
    time_t neighbour_stamp;
    char* dump(char *out);
};

// Connection properties

extern string EXAMPLE_DB;
extern string EXAMPLE_HOST;
extern string EXAMPLE_USER;
extern string EXAMPLE_PASS;
extern string EXAMPLE_CHARSET;
extern string DB_JSON;

void get_db_config(string& db, string& host, string& usr, string& pwd, string& charset);

#define CHANGED_FLAG_TABLE 1
#define DEVICE_TABLE 2
#define NEIGHBOUR_TABLE 3
#define INTERFACE_TABLE 4
#define BUSINESS_TABLE 5

#define BUSINESS_UPDATE 100

typedef vector<interface_info_t > VEC_II;
typedef vector<business_info_t > VEC_BI;
typedef vector<neighbour_info_t > VEC_NI;

bool get_mysql_data(void *pData, int type);

bool get_changed_flag_info(PTR_Statement& stmt, changed_flag_t& data);
bool get_device_info(PTR_Statement& stmt, device_info_t& data);
bool get_neighbour_info(PTR_Statement& stmt, vector<neighbour_info_t >& data);
bool get_interface_info(PTR_Statement& stmt, vector<interface_info_t >& data);
bool get_business_info(PTR_Statement& stmt, vector<business_info_t >& data);

string dump_interface(VEC_II& vt);
string dump_business(VEC_BI& vt);
string dump_neighbour(VEC_NI& vt);

bool update_flow(STRINGMAP& smap);

#endif
