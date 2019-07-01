
#ifndef _SIGAR_INFO_H
#define _SIGAR_INFO_H

//#define DISK_INDEX	"_disk"
//#define PROC_INDEX	"_proc"
//#define INTERFACE_INDEX	"_interface"

#define ALLOC_MEM_SIZE	128
#define INCREMENT_SIZE	64
#define SLEEP_TIME	2	

typedef struct {
    float cpu_used_percent;
    float mem_used_percent;
    float mem_total;
    float hard_disk_used_percent;
    float hard_disk_total;
} sigar_info_t;


bool get_sigar_info(sigar_info_t& info);

#endif
