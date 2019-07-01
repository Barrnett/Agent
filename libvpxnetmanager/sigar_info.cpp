#include <stdio.h>
#include <sys/vfs.h>

#include <sigar.h>
#include <sigar_private.h>
#include <sigar_util.h>
#include <sigar_os.h>
#include <sigar_info.h>

bool get_siger_harddisk(sigar_t *sigar, float& fDiskusage, float& fDiskTotal)
{
#ifdef __x86_64__
	struct statfs64 sfs;
	statfs64("/", &sfs);
#else
	struct statfs sfs;
	statfs("/", &sfs);
#endif
	////////
	//cout << "---" << strDiskIndex << "---" << endl;
	//cout << "f_type: " << sfs.f_type << endl; 	
	//cout << "bsize: " << sfs.f_bsize << endl; 	
	//cout << "blocks : " << sfs.f_blocks << endl; 	
	//cout << "bfree : " << sfs.f_bfree << endl; 	
	//cout << "bavail : " << sfs.f_bavail << endl; 	
	//cout << "files : " << sfs.f_files << endl; 	
	//cout << " ffree : " << sfs.f_ffree << endl; 	
	//cout << "fsid 0:" << sfs.f_fsid.__val[0] << endl; 	
	//cout << "fsid 1:" << sfs.f_fsid.__val[1] << endl; 	
	//cout << "namelen : " << sfs.f_namelen << endl; 	
	//cout << "frsize : " << sfs.f_frsize << endl; 	
	////////

	float fTotal = 0.0;
	float fUsage = 0.0;
	fTotal = 1.0 *sfs.f_blocks / 1024.0 / 1024.0 * sfs.f_bsize ;	
	if(sfs.f_blocks != 0)
		fUsage = (sfs.f_blocks - sfs.f_bfree ) * 100.0 / (sfs.f_blocks -sfs.f_bfree + sfs.f_bavail) + 1 ;

	if(sfs.f_blocks == sfs.f_bfree)
		fUsage = 0.0;
	if (fUsage > 100.0)
		fUsage = 100.0;
		
	fDiskusage = fUsage;
	fDiskTotal = fTotal;
	return true;
}
bool get_siger_mem(sigar_t *sigar, float& fMemusage, float& fMemTotal)
{
	bool ret = false;
	sigar_mem_t mem;
	mem.total = 0;
	mem.used = 0;
	float fMemUsed;
	do
	{
		int rc = sigar_mem_get(sigar, &mem);
		if(rc != SIGAR_OK)
			break;

		fMemTotal = (float)mem.total/1024/1024;
		fMemUsed = (float)mem.actual_used/1024/1024;
		if(fMemTotal == 0)
			fMemusage = 0;
		else
			fMemusage = 100 * fMemUsed/fMemTotal;
		ret = true;
	}while(0);
	return ret;
}

bool get_siger_cpu(sigar_t *sigar, float& fCPUusage)
{
	bool ret = false;
	sigar_cpu_t cpu0, cpu1;
	do
	{
		int rc = sigar_cpu_get(sigar, &cpu0);
		if(rc != SIGAR_OK)
			break;
		sleep(SLEEP_TIME);
		sigar_cpu_get(sigar, &cpu1);
		unsigned long ul_user = (cpu1.user<cpu0.user)?(ULONG_MAX-cpu0.user+cpu1.user):(cpu1.user-cpu0.user);
		unsigned long ul_sys = (cpu1.sys<cpu0.sys)?(ULONG_MAX-cpu0.sys+cpu1.sys):(cpu1.sys-cpu0.sys);
		unsigned long ul_nice = (cpu1.nice<cpu0.nice)?(ULONG_MAX-cpu0.nice+cpu1.nice):(cpu1.nice-cpu0.nice);
		unsigned long ul_idle = (cpu1.idle<cpu0.idle)?(ULONG_MAX-cpu0.idle+cpu1.idle):(cpu1.idle-cpu0.idle);
		unsigned long ul_wait = (cpu1.wait<cpu0.wait)?(ULONG_MAX-cpu0.wait+cpu1.wait):(cpu1.wait-cpu0.wait);
		unsigned long ul_irq = (cpu1.irq<cpu0.irq)?(ULONG_MAX-cpu0.irq+cpu1.irq):(cpu1.irq-cpu0.irq);
		unsigned long ul_soft_irq = (cpu1.soft_irq<cpu0.soft_irq)?(ULONG_MAX-cpu0.soft_irq+cpu1.soft_irq):(cpu1.soft_irq-cpu0.soft_irq);
		unsigned long ul_stolen = (cpu1.stolen<cpu0.stolen)?(ULONG_MAX-cpu0.stolen+cpu1.stolen):(cpu1.stolen-cpu0.stolen);
		unsigned long ul_total = (cpu1.total<cpu0.total)?(ULONG_MAX-cpu0.total+cpu1.total):(cpu1.total-cpu0.total);
	 
		fCPUusage = 0.0;
		float fCPUuser = 0.0;
		float fCPUsys = 0.0;
		float fCPUnice = 0.0;
		float fCPUidle = 0.0;
		float fCPUwait = 0.0;
		float fCPUirq = 0.0;
		float fCPUsoft_irq = 0.0;
		float fCPUstolen = 0.0;

		if(ul_total != 0)
		{
			fCPUuser = (float)ul_user / ul_total * 100.0;	
			fCPUsys = (float)ul_sys / ul_total * 100.0;	
			fCPUnice = (float)ul_nice / ul_total * 100.0;	
			fCPUidle = (float)ul_idle / ul_total * 100.0;	
			fCPUwait = (float)ul_wait / ul_total * 100.0;	
			fCPUirq = (float)ul_irq / ul_total * 100.0;	
			fCPUsoft_irq = (float)ul_user / ul_total * 100.0;	
			fCPUstolen = (float)ul_stolen / ul_total * 100.0;
			fCPUusage = 100.0 - fCPUidle;	
		}

		ret = true;
	}while(0);
	return ret;
}



bool get_sigar_info(sigar_info_t& info)
{
	bool ret = false;
	sigar_t *sigar = NULL;
	int nStatus = 0;

	do
	{
		nStatus = sigar_open(&sigar);
		if(nStatus != SIGAR_OK)
			break;

		if(!get_siger_cpu(sigar, info.cpu_used_percent))
			break;

		if(!get_siger_mem(sigar, info.mem_used_percent, info.mem_total))
			break;
		
		get_siger_harddisk(sigar, info.hard_disk_used_percent, info.hard_disk_total);
		
		sigar_close(sigar);
		ret = true;
	}while(0);
	return ret;
}

