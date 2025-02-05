#include <stdio.h>
#include "src/visa.h"
void main(int argc, char** argv){
	ViSession defaultRM,handle;
    char * out;
    if (argc <2 ){
        puts("No resource specified");
        return;
    }
    switch(viOpenDefaultRM(&defaultRM)){
        case VI_SUCCESS:
            out = "VI_SUCCESS";
            break;

        case VI_WARN_CONFIG_NLOADED:
            out = "VI_WARN_CONFIG_NLOADED";
            break;


        case VI_ERROR_SYSTEM_ERROR:
            out = "VI_ERROR_SYSTEM_ERROR";
            break;

        case VI_ERROR_ALLOC:
            out = "VI_ERROR_ALLOC";
            break;

        case VI_ERROR_INV_SETUP:
            out = "VI_ERROR_INV_SETUP";
            break;

        case VI_ERROR_LIBRARY_NFOUND:
            out = "VI_ERROR_LIBRARY_NFOUND";
            break;
    }
    printf("Open RM:%s\n",out);

	int res = viOpen(defaultRM, argv[1], VI_NULL, VI_TMO_IMMEDIATE, &handle);
    switch(res){


        case VI_SUCCESS:
            out = "VI_SUCCESS";
            break;

        case VI_SUCCESS_DEV_NPRESENT:
            out = "VI_SUCCESS_DEV_NPRESENT";
            break;

        case VI_WARN_CONFIG_NLOADED:
            out = "VI_WARN_CONFIG_NLOADED";
            break;

        case VI_ERROR_INV_OBJECT:
            out = "VI_ERROR_INV_OBJECT";
            break;

        case VI_ERROR_NSUP_OPER:
            out = "VI_ERROR_NSUP_OPER";
            break;

        case VI_ERROR_INV_RSRC_NAME:
            out = "VI_ERROR_INV_RSRC_NAME";
            break;

        case VI_ERROR_INV_ACC_MODE:
            out = "VI_ERROR_INV_ACC_MODE";
            break;

        case VI_ERROR_RSRC_NFOUND:
            out = "VI_ERROR_RSRC_NFOUND";
            break;

        case VI_ERROR_ALLOC:
            out = "VI_ERROR_ALLOC";
            break;

        case VI_ERROR_RSRC_BUSY:
            out = "VI_ERROR_RSRC_BUSY";
            break;

        case VI_ERROR_RSRC_LOCKED:
            out = "VI_ERROR_RSRC_LOCKED";
            break;

        case VI_ERROR_TMO:
            out = "VI_ERROR_TMO";
            break;

        case VI_ERROR_LIBRARY_NFOUND:
            out = "VI_ERROR_LIBRARY_NFOUND";
            break;

        case VI_ERROR_INTF_NUM_NCONFIG:
            out = "VI_ERROR_INTF_NUM_NCONFIG";
            break;

        case VI_ERROR_MACHINE_NAVAIL:
            out = "VI_ERROR_MACHINE_NAVAIL";
            break;

        case VI_ERROR_NPERMISSION:
            out = "VI_ERROR_NPERMISSION";
            break;

    }
    puts(out);
}
