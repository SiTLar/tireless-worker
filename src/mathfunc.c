#include <math.h>
#define INCL_RXFUNC
#include "rexxsaa.h"
#include "defines.h"
#include "rxpack.h"
#include "rxdef.h"
typedef  struct {
const char* first;
/*
APIRET APIENTRY (*)( RFH_ARG0_TYPE , RFH_ARG1_TYPE , RFH_ARG2_TYPE , RFH_ARG3_TYPE , RFH_ARG4_TYPE ) second;
*/
void * second;
} RXFDATA;

int rsaaGetInt(RXSTRING*arg);
double rsaaGetDouble(RXSTRING*arg);

APIRET APIENTRY rfldexp( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname, RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 2, 2 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le", ldexp( rsaaGetDouble(&(argv[0])), rsaaGetInt(&(argv[1]))));
	return 0;
};
APIRET APIENTRY rfacos( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",acos( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfasin( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",asin( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfatan( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",atan( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfatan2( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 2, 2 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",atan2( rsaaGetDouble(&(argv[0])), rsaaGetDouble(&(argv[1])) ));
	return 0;
};
APIRET APIENTRY rfcos( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
retstr->strlength = sprintf( (char *)retstr->strptr, "%le",	cos( rsaaGetDouble(&(argv[0])) ));
return 0;
};
APIRET APIENTRY rfcosh( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",cosh( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfexp( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",exp( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rffabs( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",fabs( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rffmod( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",fmod( rsaaGetDouble(&(argv[0])), rsaaGetDouble(&(argv[1])) ));
	return 0;
};
APIRET APIENTRY rflog( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",log( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rflog10( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",log10( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfpow( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 2, 2 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",pow( rsaaGetDouble(&(argv[0])), rsaaGetDouble(&(argv[1])) ));
	return 0;
};
APIRET APIENTRY rfsin( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",sin( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfsinh( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",sinh( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfsqrt( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",sqrt( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rftan( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",tan( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rftanh( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",tanh( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfacosh( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",acosh( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfasinh( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",asinh( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfatanh( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",atanh( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfhypot( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 2, 2 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",hypot( rsaaGetDouble(&(argv[0])), rsaaGetDouble(&(argv[1])) ));
	return 0;
};
APIRET APIENTRY rflog2( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",log2( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfj0( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",j0( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfj1( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",j1( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfjn( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 2, 2 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",jn( rsaaGetInt(&(argv[0])), rsaaGetDouble(&(argv[1])) ));
	return 0;
};
APIRET APIENTRY rfy0( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",y0( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfy1( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",y1( rsaaGetDouble(&(argv[0])) ));
	return 0;
};
APIRET APIENTRY rfyn( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 2, 2 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",yn( rsaaGetInt(&(argv[0])), rsaaGetDouble(&(argv[1])) ));
	return 0;
};
/* APIRET APIENTRY rffrexp( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc,
		RFH_ARG2_TYPE argv, RFH_ARG3_TYPE qname , RFH_ARG4_TYPE retstr ){
	if ( my_checkparam( NULL, (char *)name, argc, 1, 1 ) ) return -1;
	retstr->strlength = sprintf( (char *)retstr->strptr, "%le",frexp( double __value, int *__exp ));
	return 0;
}; */
RXFDATA rfArray[] ={
	{"ldexp", rfldexp},
/*	{ "modf", rfmodf},*/
	{ "acos", rfacos},
	{ "asin", rfasin},
	{ "atan", rfatan},
	{ "atan2", rfatan2},
	{ "cos", rfcos},
	{ "cosh", rfcosh},
	{ "exp", rfexp},
	{ "fabs", rffabs},
	{ "fmod", rffmod},
	{ "log", rflog},
	{ "log10", rflog10},
	{ "pow", rfpow},
	{ "sin", rfsin},
	{ "sinh", rfsinh},
	{ "sqrt", rfsqrt},
	{ "tan", rftan},
	{ "tanh", rftanh},
	{ "acosh", rfacosh},
	{ "asinh", rfasinh},
	{ "atanh", rfatanh},
	{ "hypot", rfhypot},
	{ "log2", rflog2},
	{ "j0", rfj0},
	{ "j1", rfj1},
	{ "jn", rfjn},
	{ "y0", rfy0},
	{ "y1", rfy1},
	{ "yn", rfyn},
	{ NULL, NULL}
};

double rsaaGetDouble(RXSTRING*arg){
	char *format;
	double ret;
	unsigned long counter =0, length;
	length =arg->strlength; 
	while(length = length>>1)++counter;
	format = malloc(counter/3.32+4 );
	sprintf(format, "%%%dle", arg->strlength);
	sscanf(arg->strptr,format,  &ret);
	free(format);
	return ret;
};
int rsaaGetInt(RXSTRING*arg){
	char *format;
	int ret;
	unsigned long counter =0, length;
	length =arg->strlength; 
	while(length = length>>1)++counter;
	format = malloc(counter/3.32+4 );
	sprintf(format, "%%%dd", arg->strlength);
	sscanf(arg->strptr,format,  &ret);
	free(format);
	return ret;
};
void fLoadRexxMath(){
	RXFDATA *funcs = rfArray;
	do RexxRegisterFunctionExe(funcs->first, funcs->second); 
	while((++funcs)->first);
}

