#include <stdio.h>
#include <string.h>
#include "mysql_info.h"
#include "monitor.h"
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



string EXAMPLE_DB   = "tckr";
string EXAMPLE_HOST = "tcp://127.0.0.1:3306";
string EXAMPLE_USER = "root";
string EXAMPLE_PASS = "newroot";
string EXAMPLE_CHARSET = "gbk";
string DB_JSON = "/usr/local/mxagent/bmc/module/libvpxnetmanager/db.json";

/* Connection parameter and sample data */
bool get_interface_info(PTR_Statement& stmt, vector<interface_info_t >& data)
{
	int nrow = 0, i=0;
	/*
	Run a query which returns exactly one result set like SELECT
	Stored procedures (CALL) may return more than one result set
	*/
	string strTmp;
	stringstream sql;
	sql.str("");
	/*
	sql << "SELECT `Index`,`Name`,`Desc`,`Status`,channelbandwidth,channeltype,channelerrorrate,";
	sql << "interfaceIP,IPMask,OSPFCost,InFlow,OutFlow,PacketLoss,TimeDelay,BandwidthUtilization FROM t_interface;";
	*/
	sql << "SELECT t1.ethid,t3.`name`,t1.description,t2.ifstat,t1.nnibw,t1.nnitype,t1.nniber,t1.ipaddr,";
	sql << "t1.mask,t1.gateway,t1.dtperiod,t1.dtquene,t2.cost,t2.inrate,t2.outrate,t2.lostrate,t2.avgrtt,t2.bwutility,t2.jitter,t2.iprepeat ";
	sql << "FROM extranet t1 JOIN switcheth t3 ON t1.ethid = t3.identity ";
	sql << "LEFT JOIN extranetstat t2 ON t3.identity = t2.ethid";
	sql << " UNION ";
	sql << "SELECT t1.ethid,t3.`name`,t1.description,t2.ifstat,t1.nnibw,t1.nnitype,t1.nniber,t1.ipaddr,";
	sql << "t1.mask,t1.gateway,t1.dtperiod,t1.dtquene,t2.cost,t2.inrate,t2.outrate,t2.lostrate,t2.avgrtt,t2.bwutility,t2.jitter,t2.iprepeat ";
	sql << "FROM backnet t1 JOIN maineth t3 ON t1.ethid = t3.identity ";
	sql << "LEFT JOIN backnetstat t2 ON t3.identity = t2.ethid ";
	sql << " UNION ";
	sql << "SELECT t1.ethid,t3.`name`,t1.description,t2.ifstat,0 AS nnibw,0 AS nnitype,0 AS nniber,t1.ipaddr,";
	sql << "t1.netmask,t1.gateway,0 AS dtperiod, 0 AS dtquene,0 AS cost,t2.inrate,t2.outrate,0 AS lostrate,0 AS avgrtt,	0 AS bwutility, 0 AS jitter,'' AS iprepeat ";
	sql << "FROM localnet t1 JOIN maineth t3 ON t1.ethid = t3.identity ";
	sql << "LEFT JOIN localnetstat t2 ON t3.identity = t2.ethid ;";
 
	std::auto_ptr< sql::ResultSet > res(stmt->executeQuery(sql.str()));
	
	data.clear();
	nrow = 0;
	while (res->next()) {
		interface_info_t row;
		i=1;
		row.Index = res->getInt(i++);
	    row.Name = res->getString(i++);
	    row.Desc = res->getString(i++);
	    row.Status = res->getInt(i++);
	    row.channelbandwidth = res->getInt(i++);
	    row.channeltype = res->getInt(i++);
	    row.channelerrorrate = res->getDouble(i++);
	    
	    row.interfaceIP = htonl(res->getUInt(i++));
	    row.IPMask = htonl(res->getUInt(i++));
	    row.gateway = htonl(res->getUInt(i++));
	    row.dtperiod = res->getUInt(i++);
	    row.dtquene = res->getUInt(i++);

	    row.OSPFCost = res->getInt(i++);
	    row.InFlow = res->getInt(i++)/1000.0;
	    row.OutFlow = res->getInt(i++)/1000.0;
	    row.PacketLoss = (float)res->getDouble(i++);
	    row.TimeDelay = res->getInt(i++)/1000.0;
	    row.BandwidthUtilization = (float)res->getDouble(i++);
	    row.jitter = res->getInt(i++);
	    //row.iprepeat = res->getString(i++);
	    strTmp = res->getString(i++);

	    if(strTmp.size() > 1)
	    {
	    	string strAppend = "ºÍ " + strTmp;
	    	row.iprepeat =  strAppend + " ³åÍ»";
	    }else
	    {
	    	row.iprepeat = strTmp;
	    }

		nrow++;
		data.push_back(row);
	}
	return (nrow > 0);
	
}

bool get_business_info(PTR_Statement& stmt, vector<business_info_t >& data)
{
	int nrow = 0, i=0;
	/*
	Run a query which returns exactly one result set like SELECT
	Stored procedures (CALL) may return more than one result set
	*/
	stringstream sql;
	sql.str("");
	/*
	sql << "SELECT `Index`,`Name`,`Desc`,Type,Priority,SourceIP,SourceIPMask,TargetIP,TargetIPMask,";
	sql << "SourcePort,TargetPort,NetProtocol,TransmissionChannel,BackupTransmissionChannel,";
	sql << "IsCreateBackupPath,OutFlow,OutPacketLoss,TransTimeDelay,TransmissionPath  FROM t_business;";
	*/

	sql << "select t1.flowid,t1.`name`,t1.description,t1.`type`,t1.priority,";
	sql << "t1.srcip,t1.srcmask,t1.dstip,t1.dstmask,t1.sptmin,t1.sptmax,t1.dstmin,t1.dstmax,";
	sql << "t1.proto,t1.mainforward,t1.backforward,t1.backups,t1.nnibwth,t1.nniberth,t1.lostrateth,";
	sql << "t1.avgrttth,t1.secondpath,t1.thirdpath,t1.pathmode,t1.dtperiod,t1.dtqueue, ";
	sql << "t2.outrate, t2.lostrate, t2.curpath, ";
	sql << "t2.path1lostrate,t2.path1avgrtt,t2.path1jitter,t2.path2lostrate,t2.path2avgrtt,t2.path2jitter,t2.path3lostrate,t2.path3avgrtt,t2.path3jitter,t2.pathdstne ";
	sql << "from flowpolicy t1 left join flowstat t2 on t1.flowid=t2.flowid;";
	std::auto_ptr< sql::ResultSet > res(stmt->executeQuery(sql.str()));

	data.clear();
	nrow = 0;
	while (res->next()) {
		business_info_t row;
		i=1;
		row.Index = res->getInt(i++);
	    row.Name = res->getString(i++);
	    row.Desc = res->getString(i++);
	    row.Type = res->getInt(i++);
	    row.Priority = res->getInt(i++);
	    row.SourceIP = htonl(res->getUInt(i++));
	    row.SourceIPMask = htonl(res->getUInt(i++));
	    row.TargetIP = htonl(res->getUInt(i++));
	    row.TargetIPMask = htonl(res->getUInt(i++));
	    row.sptmin = res->getInt(i++);
	    row.sptmax = res->getInt(i++);
	    row.dstmin = res->getInt(i++);
	    row.dstmax = res->getInt(i++);
	    row.NetProtocol = res->getInt(i++);
	    row.mainforward = res->getInt(i++);
	    row.backforward = res->getString(i++);
	    row.backups = res->getInt(i++);
	    row.nnibwth = res->getUInt64(i++)/1000.0;
	    row.nniberth = (float)res->getDouble(i++);
	    row.lostrateth = (float)res->getDouble(i++);
	    row.avgrttth = res->getInt(i++)/1000.0; 
	    row.secondpath = res->getString(i++);
	    row.thirdpath = res->getString(i++);
	    row.pathmode = res->getInt(i++);
	    row.dtperiod = res->getInt(i++);
	    row.dtqueue = res->getInt(i++);	    
	    row.outrate = res->getUInt64(i++)/1000.0;
	    row.lostrate = (float)res->getDouble(i++);	    
	    row.curpath = res->getInt(i++);
	    row.path1lostrate = (float)res->getDouble(i++);
	    row.path1avgrtt = res->getInt(i++);
	    row.path1jitter = res->getInt(i++);
	    row.path2lostrate = (float)res->getDouble(i++);
	    row.path2avgrtt = res->getInt(i++);
	    row.path2jitter = res->getInt(i++);
	    row.path3lostrate = (float)res->getDouble(i++);
	    row.path3avgrtt = res->getInt(i++);
	    row.path3jitter = res->getInt(i++);
	    row.pathdstne = htonl(res->getUInt(i++));

		data.push_back(row);
		nrow++;
	}
	return (nrow > 0);

}


string get_longitude_latitude(uint32_t latitude)
{
	char buf[128];
	uint8_t u1,u2,u3,u4;
	u4 = latitude & 0xff;
	u3 = (latitude >> 8) & 0xff;
	u2 = (latitude >> 16) & 0xff;
	u1 = (latitude >> 24) & 0xff;
	sprintf(buf, "%C %u¡ã%u'%u\"", u1, u2, u3,u4);
	return string(buf);
}

bool get_device_info(PTR_Statement& stmt, device_info_t& data)
{
	int row = 0, i=0;
	/*
	Run a query which returns exactly one result set like SELECT
	Stored procedures (CALL) may return more than one result set
	*/
	stringstream sql;
	sql.str("");
	/*
	sql << "SELECT sn,dname,ddesc,version,NetElemMgrIP,NetMgrIP,MonitorIP,";
	sql << "exchangeCPUUsage,exchangeMemUsage,exchangeMemTotal,deviceStatus FROM t_device;";
	*/
	sql << "SELECT t1.sn,t1.`code`,t1.`name`,t1.description,t1.edition,t1.neteip,t1.netmip,t1.controlip,t1.devid,t1.longitude,t1.latitude,";
	sql << "t2.maincpurate,t2.switchcpurate,t2.mainmemrate,t2.switchmemrate,t2.mainmemtotal,t2.switchmemtotal,t2.maindiskrate,t2.maindisktotal,t2.state ";
	sql << "from eqinfo t1 left join eqstat t2 on t1.identity=t2.identity;";
	std::auto_ptr< sql::ResultSet > res(stmt->executeQuery(sql.str()));

	uint32_t longitude, latitude;
	/* Fetching data */
	row = 0;
	while (res->next()) {
		i=1;
		data.deviceSN = res->getString(i++);
		data.deviceCode = res->getString(i++);
	    data.deviceName = res->getString(i++);
	    data.deviceDesc = res->getString(i++);
	    data.deviceVersion = res->getString(i++);
	    data.deviceNetElemMgrIP = htonl(res->getUInt(i++));
	    data.deviceNetMgrIP = htonl(res->getUInt(i++));
	    data.deviceMonitorIP = htonl(res->getUInt(i++));
	    /////////////////////
		//2019/1/18
		data.devid = res->getUInt(i++);
		//data.longitude = res->getUInt(i++);
		//data.latitude = res->getUInt(i++);
		longitude = res->getUInt(i++);
		latitude = res->getUInt(i++);
		data.longitude = get_longitude_latitude(longitude);
		data.latitude = get_longitude_latitude(latitude);
	    ////////////////////////
	    data.mainCPUUsage = (float)res->getDouble(i++);
	    data.exchangeCPUUsage = (float)res->getDouble(i++);
	    data.mainMemUsage = (float)res->getDouble(i++);
	    data.exchangeMemUsage = (float)res->getDouble(i++);
	    data.mainMemTotal = res->getUInt(i++);
	    data.exchangeMemTotal = res->getUInt(i++);
	    data.mainHarddiskUsage = (float)res->getDouble(i++);
	    data.mainHarddiskTotal = res->getUInt(i++);
	    data.deviceStatus = res->getInt(i++);

		row++;
	}
	return (row > 0);
}

bool get_changed_flag_info(PTR_Statement& stmt, changed_flag_t& data)
{
	//string strSQL = "SELECT device_stamp,interface_stamp,business_stamp, neighbour_stamp FROM t_changedflag limit 1;";
	string strSQL = "SELECT eqinfotime,nettime,neighbortime FROM eqchange limit 1;";
	std::auto_ptr< sql::ResultSet > res(stmt->executeQuery(strSQL));
	/* Fetching data */
	int row = 0, i=0;
	while (res->next()) {
		i=1;
		data.device_stamp = res->getUInt(i++);
		data.interface_stamp = res->getUInt(i++);
		//data.business_stamp = res->getInt64(i++);
		data.neighbour_stamp = res->getUInt(i++);
		row++;
	}
	return (row > 0);
}

bool get_neighbour_info(PTR_Statement& stmt, vector<neighbour_info_t >& data)
{
	int nrow = 0, i=0;
	/*
	Run a query which returns exactly one result set like SELECT
	Stored procedures (CALL) may return more than one result set
	*/
	
	stringstream sql;
	sql.str("");
	//sql << "SELECT NetElemMgrIP,LocalIndex,LocalIP,LocalMask,Cost FROM t_neighbour;";
	sql << "SELECT ethid,ipaddr,nexthop,neighborip,cost,avgrtt FROM extraneighbor;";

	std::auto_ptr< sql::ResultSet > res(stmt->executeQuery(sql.str()));

	data.clear();
	nrow = 0;
	while (res->next()) {
		neighbour_info_t row;
		i=1;
		row.ethid = res->getUInt(i++);
	    row.ipaddr = htonl(res->getUInt(i++));
	    row.nexthop = htonl(res->getUInt(i++));
	    row.neighborip = htonl(res->getUInt(i++));
	    row.cost = res->getUInt(i++);
	    row.avgrtt = res->getUInt(i++);

	    data.push_back(row);
		nrow++;
	}
	return (nrow > 0);

}

bool update_flow(PTR_Statement& stmt, STRINGMAP& smap)
{
	string strSQL;
	STRINGMAP::iterator it;
	for(it=smap.begin(); it!=smap.end(); it++)
	{
		strSQL = it->second;
		cout << "exec: " << strSQL << endl;
		stmt->execute(strSQL);
	}
	return true;
}

bool get_mysql_data(void *pData, int type)
{
	/*
	string url( EXAMPLE_HOST);
	const string user(EXAMPLE_USER);
	const string pass(EXAMPLE_PASS);
	const string database(EXAMPLE_DB);
	const string characterSetResults(EXAMPLE_CHARSET);
	*/
	string database, url , user, pass, characterSetResults;
	get_db_config(database, url , user, pass, characterSetResults);

	bool ret = false;
	//cout << "# get_mysql_changed_flag" << endl;
	try {
		sql::Driver * driver = get_driver_instance();
		/* Using the Driver to create a connection */
		//std::auto_ptr< sql::Connection > con(driver->connect(url, user, pass));
		std::map<std::string, sql::ConnectPropertyVal>  options;
		{
			sql::ConnectPropertyVal tmp;
			tmp.str.val = url.c_str();
			tmp.str.len = url.length();
			options[std::string("hostName")] = tmp;
		}
		{
			sql::ConnectPropertyVal tmp;
			tmp.str.val = user.c_str();
			tmp.str.len = user.length();
			options[std::string("userName")] = tmp;
		}
		{
			sql::ConnectPropertyVal tmp;
			tmp.str.val = pass.c_str();
			tmp.str.len = pass.length();
			options[std::string("password")] = tmp;
		}
		{
			sql::ConnectPropertyVal tmp;
			tmp.str.val = characterSetResults.c_str();
			tmp.str.len = characterSetResults.length();
			options[std::string("characterSetResults")] = tmp;
		}
		std::auto_ptr< sql::Connection > con(driver->connect(options));
		/* Creating a "simple" statement - "simple" = not a prepared statement */
		std::auto_ptr< sql::Statement > stmt(con->createStatement());

		/* Create a test table demonstrating the use of sql::Statement.execute() */
		stmt->execute("USE " + database);
		
		switch(type)
		{
			case CHANGED_FLAG_TABLE:
				{
				cout<<"debug stephen1"<<endl;
				changed_flag_t *ptr_data = (changed_flag_t *)pData;
				ret = get_changed_flag_info(stmt, *ptr_data);
				}
				break;	
			case DEVICE_TABLE:
				{
				device_info_t *ptr_data = (device_info_t *)pData;
				ret = get_device_info(stmt, *ptr_data);
				}
				break;	
			case NEIGHBOUR_TABLE:
				{
				vector<neighbour_info_t > *ptr_data = (vector<neighbour_info_t > *)pData;
				ret = get_neighbour_info(stmt, *ptr_data);
				}
				break;	
			case INTERFACE_TABLE:
				{
				vector<interface_info_t > *ptr_data =(vector<interface_info_t > *)pData;
				ret = get_interface_info(stmt, *ptr_data);
				}
				break;	
			case BUSINESS_TABLE:
				{
				vector<business_info_t > *ptr_data =(vector<business_info_t > *)pData;
				ret = get_business_info(stmt, *ptr_data);
				}
				break;
			case BUSINESS_UPDATE:
				{
				STRINGMAP* psmap =(STRINGMAP *)pData;
				ret = update_flow(stmt, *psmap);
				}
				break;
			default:
				break;
		}
		

		cout << "# done!" << endl;

	} catch (sql::SQLException &e) {
		/*
		The MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		cout << "not ok 1 - examples/connect.php" << endl;

		return false;
	} catch (std::runtime_error &e) {

		cout << "# ERR: runtime_error in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what() << endl;
		cout << "not ok 1 - examples/connect.php" << endl;

		return false;
	}
	return ret;
}



string ip_ntostr(const unsigned int ip)
{
	struct in_addr addr;
	addr.s_addr = ip;
	return string(inet_ntoa(addr));
}

char* device_info_t::dump(char *out)
{
	string strTmp;
	char *pt = out;
	M_MAKERETURN_ITEM_S(pt, varName(deviceSN), deviceSN.c_str());
	M_MAKERETURN_ITEM_S(pt, varName(deviceCode), deviceCode.c_str());
	M_MAKERETURN_ITEM_S(pt, varName(deviceName), deviceName.c_str());
	M_MAKERETURN_ITEM_S(pt, varName(deviceDesc), deviceDesc.c_str());
	M_MAKERETURN_ITEM_S(pt, varName(deviceVersion), deviceVersion.c_str());
	strTmp = ip_ntostr(deviceNetElemMgrIP);
	M_MAKERETURN_ITEM_S(pt, varName(deviceNetElemMgrIP), strTmp.c_str());
	strTmp = ip_ntostr(deviceNetMgrIP);
	M_MAKERETURN_ITEM_S(pt, varName(deviceNetMgrIP), strTmp.c_str());
	strTmp = ip_ntostr(deviceMonitorIP);
	M_MAKERETURN_ITEM_S(pt, varName(deviceMonitorIP), strTmp.c_str());
	//////////////////
	char buf[128];
	M_MAKERETURN_ITEM_D(pt, varName(devid), devid);
	/*
	sprintf(buf, "%f", longitude);
	M_MAKERETURN_ITEM_S(pt, varName(longitude), buf);
	sprintf(buf, "%f", latitude);
	M_MAKERETURN_ITEM_S(pt, varName(latitude), buf);
	*/
	M_MAKERETURN_ITEM_S(pt, varName(longitude), longitude.c_str());
	M_MAKERETURN_ITEM_S(pt, varName(latitude), latitude.c_str());
	
	/////////////////
	M_MAKERETURN_ITEM_F(pt, varName(mainCPUUsage), mainCPUUsage);
	M_MAKERETURN_ITEM_F(pt, varName(exchangeCPUUsage), exchangeCPUUsage);
	M_MAKERETURN_ITEM_F(pt, varName(mainMemUsage), mainMemUsage);
	M_MAKERETURN_ITEM_D(pt, varName(mainMemTotal), mainMemTotal);
	M_MAKERETURN_ITEM_F(pt, varName(exchangeMemUsage), exchangeMemUsage);
	M_MAKERETURN_ITEM_D(pt, varName(exchangeMemTotal), exchangeMemTotal);
	M_MAKERETURN_ITEM_F(pt, varName(mainHarddiskUsage), mainHarddiskUsage);
	M_MAKERETURN_ITEM_F(pt, varName(mainCPUUsage), mainCPUUsage);
	M_MAKERETURN_ITEM_D(pt, varName(mainHarddiskTotal), mainHarddiskTotal);
	M_MAKERETURN_ITEM_D_END(pt, varName(deviceStatus), deviceStatus);
	return pt;
}

char* device_info_t::dumpjson(char *out)
{
	char *pt = out;
    Json::Value writevalueinfo;
   	writevalueinfo["NetDeviceSN"] = deviceSN;
   	writevalueinfo["NetDeviceCode"] = deviceCode;
   	writevalueinfo["NetDeviceName"] = deviceName;
   	writevalueinfo["NetDeviceDesc"] = deviceDesc;
   	writevalueinfo["SoftVersion"] = deviceVersion;
   	writevalueinfo["NetElementIp"] = deviceNetElemMgrIP;
   	writevalueinfo["JianKongIp"] = deviceNetMgrIP;
   	writevalueinfo["ChassisMonitorIp"] = deviceMonitorIP;
   	writevalueinfo["NetDeviceState"] = deviceStatus;

   	writevalueinfo["devid"] = devid;
   	writevalueinfo["longitude"] = longitude;
   	writevalueinfo["latitude"] = latitude;
   	
   	writevalueinfo["maincpurate"] = mainCPUUsage;
   	writevalueinfo["switchcpurate"] = exchangeCPUUsage;
   	writevalueinfo["mainmemrate"] = mainMemUsage;
   	writevalueinfo["switchmemrate"] = exchangeMemUsage;
   	writevalueinfo["mainmemtotal"] = mainMemTotal;
   	writevalueinfo["switchmemtotal"] = exchangeMemTotal;
   	writevalueinfo["maindiskrate"] = mainHarddiskUsage;
   	writevalueinfo["maindisktotal"] = mainHarddiskTotal;

   	string strJson = writevalueinfo.toStyledString();
   	cout << "device_json:" << strJson<< endl;
	sprintf(pt, "%s", strJson.c_str());
	pt+=strlen(pt);
	pt[0]='\0';
	pt++;
	return pt;
}


char* interface_info_t::dump(char *out)
{
	string strTmp;
	//char szbuf[128];
	char *pt = out;
	M_MAKERETURN_ITEM_D(pt, varName(Index), Index);
	M_MAKERETURN_ITEM_S(pt, varName(Name), Name.c_str());
	M_MAKERETURN_ITEM_S(pt, varName(Desc), Desc.c_str());
	M_MAKERETURN_ITEM_D(pt, varName(Status), Status);
	M_MAKERETURN_ITEM_D(pt, varName(channelbandwidth), channelbandwidth);
	M_MAKERETURN_ITEM_D(pt, varName(channeltype), channeltype);
	M_MAKERETURN_ITEM_F(pt, varName(channelerrorrate), channelerrorrate);
	strTmp = ip_ntostr(interfaceIP);
	M_MAKERETURN_ITEM_S(pt, varName(interfaceIP), strTmp.c_str());
	strTmp = ip_ntostr(IPMask);
	M_MAKERETURN_ITEM_S(pt, varName(IPMask), strTmp.c_str());

	strTmp = ip_ntostr(gateway);
	M_MAKERETURN_ITEM_S(pt, varName(gateway), strTmp.c_str());
	M_MAKERETURN_ITEM_D(pt, varName(dtperiod), dtperiod);
	M_MAKERETURN_ITEM_D(pt, varName(dtquene), dtquene);
	
	M_MAKERETURN_ITEM_D(pt, varName(OSPFCost), OSPFCost);
	//sprintf(szbuf, "%lld", InFlow);
	M_MAKERETURN_ITEM_F(pt, varName(InFlow), InFlow);
	//sprintf(szbuf, "%lld", OutFlow);
	M_MAKERETURN_ITEM_F(pt, varName(OutFlow), OutFlow);
	M_MAKERETURN_ITEM_F(pt, varName(PacketLoss), PacketLoss);
	M_MAKERETURN_ITEM_F(pt, varName(TimeDelay), TimeDelay);
	M_MAKERETURN_ITEM_D(pt, varName(jitter), jitter);
	M_MAKERETURN_ITEM_S(pt, varName(iprepeat), iprepeat.c_str());

	M_MAKERETURN_ITEM_F_END(pt, varName(BandwidthUtilization), BandwidthUtilization);

	cout<<"debug interface"<<endl;

	return pt;
}

char* business_info_t::dump(char *out)
{
	string strTmp;
	//char szbuf[128];
	char *pt = out;
	M_MAKERETURN_ITEM_D(pt, varName(Index), Index);
	M_MAKERETURN_ITEM_S(pt, varName(Name), Name.c_str());
	M_MAKERETURN_ITEM_S(pt, varName(Desc), Desc.c_str());
	M_MAKERETURN_ITEM_D(pt, varName(Type), Type);
	M_MAKERETURN_ITEM_D(pt, varName(Priority), Priority);
	strTmp = ip_ntostr(SourceIP);
	M_MAKERETURN_ITEM_S(pt, varName(SourceIP), strTmp.c_str());
	strTmp = ip_ntostr(SourceIPMask);
	M_MAKERETURN_ITEM_S(pt, varName(SourceIPMask), strTmp.c_str());
	strTmp = ip_ntostr(TargetIP);
	M_MAKERETURN_ITEM_S(pt, varName(TargetIP), strTmp.c_str());
	strTmp = ip_ntostr(TargetIPMask);
	M_MAKERETURN_ITEM_S(pt, varName(TargetIPMask), strTmp.c_str());
	M_MAKERETURN_ITEM_D(pt, varName(sptmin), sptmin);
	M_MAKERETURN_ITEM_D(pt, varName(sptmax), sptmax);
	M_MAKERETURN_ITEM_D(pt, varName(dstmin), dstmin);
	M_MAKERETURN_ITEM_D(pt, varName(dstmax), dstmax);
	M_MAKERETURN_ITEM_D(pt, varName(NetProtocol), NetProtocol);
	M_MAKERETURN_ITEM_D(pt, varName(mainforward), mainforward);
	M_MAKERETURN_ITEM_S(pt, varName(backforward), backforward.c_str());
	M_MAKERETURN_ITEM_D(pt, varName(backups), backups);
	//sprintf(szbuf, "%lld", nnibwth);
	M_MAKERETURN_ITEM_F(pt, varName(nnibwth), nnibwth);
	M_MAKERETURN_ITEM_F(pt, varName(nniberth), nniberth);
	M_MAKERETURN_ITEM_F(pt, varName(lostrateth), lostrateth);
	M_MAKERETURN_ITEM_F(pt, varName(avgrttth), avgrttth);
	
	M_MAKERETURN_ITEM_S(pt, varName(secondpath), secondpath.c_str());
	M_MAKERETURN_ITEM_S(pt, varName(thirdpath), thirdpath.c_str());
	M_MAKERETURN_ITEM_D(pt, varName(pathmode), pathmode);
	M_MAKERETURN_ITEM_D(pt, varName(dtperiod), dtperiod);
	M_MAKERETURN_ITEM_D(pt, varName(dtqueue), dtqueue);
	
	//sprintf(szbuf, "%lld", outrate);
	M_MAKERETURN_ITEM_F(pt, "outratecur", outrate);
	M_MAKERETURN_ITEM_F(pt, "lostratecur", lostrate);
	
	M_MAKERETURN_ITEM_D(pt, varName(curpath), curpath);
	M_MAKERETURN_ITEM_F(pt, varName(path1lostrate), path1lostrate);
	M_MAKERETURN_ITEM_D(pt, varName(path1avgrtt), path1avgrtt);
	M_MAKERETURN_ITEM_D(pt, varName(path1jitter), path1jitter);
	M_MAKERETURN_ITEM_F(pt, varName(path2lostrate), path2lostrate);
	M_MAKERETURN_ITEM_D(pt, varName(path2avgrtt), path2avgrtt);
	M_MAKERETURN_ITEM_D(pt, varName(path2jitter), path2jitter);
	M_MAKERETURN_ITEM_F(pt, varName(path3lostrate), path3lostrate);
	M_MAKERETURN_ITEM_D(pt, varName(path3avgrtt), path3avgrtt);
	M_MAKERETURN_ITEM_D(pt, varName(path3jitter), path3jitter);
	
	strTmp = ip_ntostr(pathdstne);
	M_MAKERETURN_ITEM_S_END(pt, varName(pathdstne), strTmp.c_str());


	return pt;
}


char* changed_flag_t::dump(char *out)
{
	cout<<"debug stephen2"<<endl;
	char *pt = out;
	//Json::FastWriter writer;
    Json::Value writevalueinfo;
   	writevalueinfo["NetDeviceUpdateTime"] = (Json::UInt)device_stamp;
   	writevalueinfo["InterfaceUpdateTime"] = (Json::UInt)interface_stamp;
   	writevalueinfo["AdjacencyListUpdateTime"] = (Json::UInt)neighbour_stamp;
   	string strJson = writevalueinfo.toStyledString();
   	//string strJson = writer.write(writevalueinfo);
   	cout << "json:" << strJson<< endl;
	sprintf(pt, "%s", strJson.c_str());
	pt+=strlen(pt);
	pt[0]='\0';
	pt++;
	return pt;
}

void get_db_config(string& db, string& host, string& usr, string& pwd, string& charset)
{
	bool ret = false;
	ifstream fin( DB_JSON.c_str());
	if (fin)
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(fin, root))
		{
			host = root["host"].asString();
			usr = root["user"].asString();
			pwd = root["password"].asString();
			db = root["db"].asString();
			charset = root["charset"].asString();
			ret = true;
		}
	}
	if(!ret)
	{
		db = EXAMPLE_DB;
		host = EXAMPLE_HOST;
		usr = EXAMPLE_USER;
		pwd = EXAMPLE_PASS;
		charset = EXAMPLE_CHARSET;
	}
}


string dump_interface(VEC_II& vt)
{
    Json::Value root;
   	root["recordcount"] = (Json::Value::UInt)vt.size();
   	for(unsigned int i=0; i<vt.size(); i++)
   	{
   		Json::Value val;
		val["InterfaceIndex"] = vt[i].Index;
		val["InterfaceName"] = vt[i].Name;
		val["InterfaceDesc"] = vt[i].Desc;
		val["InterfaceState"] = vt[i].Status;
		val["Bandwidth"] = vt[i].channelbandwidth;

		val["ChannelType"] = vt[i].channeltype;
		val["ErrorRate"] = vt[i].channelerrorrate;
		val["InterfaceIp"] = vt[i].interfaceIP;
		val["InterfaceMask"] = vt[i].IPMask;
		val["OSPF_Cost"] = vt[i].OSPFCost;
		val["PacketLoss"] = vt[i].PacketLoss;
		val["TimeDelay"] = vt[i].TimeDelay;
		val["jitter"] = vt[i].jitter;
		val["InFlow"] = vt[i].InFlow;
		val["OutFlow"] = vt[i].OutFlow;
		val["iprepeat"] = vt[i].iprepeat;
		
		root["data"].append(val);
   	}
    
   	string strJson = root.toStyledString();
	return strJson;
}
//string dump_business(VEC_BI& vt);
string dump_neighbour(VEC_NI& vt)
{
	Json::Value root;
   	root["recordcount"] = (Json::Value::UInt)vt.size();
   	for(unsigned int i=0; i<vt.size(); i++)
   	{
   		Json::Value val;
		val["interfaceIndex"] = vt[i].ethid;
		val["NetDeviceIp"] = vt[i].neighborip;
		val["interfaceIp"] = vt[i].ipaddr;
		val["interfaceMask"] = vt[i].nexthop;
		val["dstInterfaceIp"] = vt[i].nexthop;
		val["costs"] = vt[i].cost;
		val["avgrtt"] = vt[i].avgrtt;
		root["data"].append(val);
   	}
    
   	string strJson = root.toStyledString();
	cout<<"debug vpxGetNeighbourJson"<<endl;
	return strJson;
}


void business_info_t::dumpJson(Json::Value& val)
{
	string strTmp;
	val[varName(Index)] = Index;
	val[varName(Name)] = Name;
	val[varName(Desc)] = Desc;
	val[varName(Type)] = Type;
	val[varName(Priority)] = Priority;
	strTmp = ip_ntostr(SourceIP);
	val[varName(SourceIP)] = strTmp;
	strTmp = ip_ntostr(SourceIPMask);
	val[varName(SourceIPMask)] = strTmp;
	strTmp = ip_ntostr(TargetIP);
	val[varName(TargetIP)] = strTmp;
	strTmp = ip_ntostr(TargetIPMask);
	val[varName(TargetIPMask)] =  strTmp;
	val[varName(sptmin)] = sptmin;
	val[varName(sptmax)] =  sptmax;
	val[varName(dstmin)] =  dstmin;
	val[varName(dstmax)] = dstmax;
	val[varName(NetProtocol)] = NetProtocol;
	val[varName(mainforward)] =  mainforward;
	val[varName(backforward)] = backforward;
	val[varName(backups)] = backups;
	//sprintf(szbuf, "%lld", nnibwth);
	val[varName(nnibwth)] = nnibwth;
	val[varName(nniberth)] = nniberth;
	val[varName(lostrateth)] = lostrateth;
	val[varName(avgrttth)] = avgrttth;
	
	val[varName(secondpath)] = secondpath;
	val[varName(thirdpath)] = thirdpath;
	val[varName(pathmode)] = pathmode;
	val[varName(dtperiod)] = dtperiod;
	val[varName(dtqueue)] =  dtqueue;
	
	//sprintf(szbuf, "%lld", outrate);
	val["outratecur"] = outrate;
	val["lostratecur"] = lostrate;
	
	val[varName(curpath)] = curpath;
	val[varName(path1lostrate)] = path1lostrate;
	val[varName(path1avgrtt)] = path1avgrtt;
	val[varName(path1jitter)] = path1jitter;
	val[varName(path2lostrate)] =  path2lostrate;
	val[varName(path2avgrtt)] = path2avgrtt;
	val[varName(path2jitter)] = path2jitter;
	val[varName(path3lostrate)] = path3lostrate;
	val[varName(path3avgrtt)] = path3avgrtt;
	val[varName(path3jitter)] = path3jitter;

	strTmp = ip_ntostr(pathdstne);
	val[varName(pathdstne)] = strTmp;

}

string dump_business(VEC_BI& vt)
{
	Json::Value root;
   	root["recordcount"] = (Json::Value::UInt)vt.size();
   	for(unsigned int i=0; i<vt.size(); i++)
   	{
   		Json::Value val;
   		vt[i].dumpJson(val);
		root["data"].append(val);
   	}
    
   	string strJson = root.toStyledString();
	return strJson;
}