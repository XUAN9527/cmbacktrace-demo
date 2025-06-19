#ifndef __ADDR_PRINT_H
#define __ADDR_PRINT_H

#include "log.h"
#include "iap_addr_def.h"

//打印flash区的地址，用于调试
static inline int dump_print(){
    logPrintln("Flash address and size:");
    logPrintln("BOOTLOAD_FIRMWARE_ADDR      =   0x%08x",BOOTLOAD_FIRMWARE_ADDR);
    logPrintln("BOOTLOAD_FIRMWARE_SIZE      =   %dK",BOOTLOAD_FIRMWARE_SIZE/1024);
    logPrintln("FACTORY_FIRMWARE_ADDR       =   0x%08x",FACTORY_FIRMWARE_ADDR);
    logPrintln("FACTORY_FIRMWARE_SIZE       =   %dK",FACTORY_FIRMWARE_SIZE/1024);
    logPrintln("APPLICATION_FIRMWARE_ADDR   =   0x%08x",APPLICATION_FIRMWARE_ADDR );
    logPrintln("APPLICATION_FIRMWARE_SIZE   =   %dK",APPLICATION_FIRMWARE_SIZE/1024);
    logPrintln("DOWNLOAD_AREA_ADDR          =   0x%08x",DOWNLOAD_AREA_ADDR);
    logPrintln("DOWNLOAD_AREA_SIZE      	=   %dK",DOWNLOAD_AREA_SIZE/1024);
    logPrintln("UPGRADE_DATA_ADDR       	=   0x%08x",UPGRADE_DATA_ADDR);
    logPrintln("UPGRADE_DATA_SIZE       	=   %dK",UPGRADE_DATA_SIZE/1024);
    logPrintln("DCD_DATA_ADDR           	=   0x%08x", DCD_DATA_ADDR);
    logPrintln("DCD_DATA_SIZE           	=   %dK", DCD_DATA_SIZE/1024);
    logPrintln("USER_DATA_ADDR          	=   0x%08x", USER_DATA_ADDR);
    logPrintln("USER_DATA_SIZE          	=   %dK", USER_DATA_SIZE/1024);
	return 0;
};

#endif
