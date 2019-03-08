#ifndef __TOOL_H
#define __TOOL_H 	

#include "stm32f10x.h"		   

// ????????

#define BigLittleSwap16(A)  ((((uint16_t)(A) & 0xff00) >> 8) | \
                            (((uint16_t)(A) & 0x00ff) << 8))

 

// ????????

#define BigLittleSwap32(A)  ((((uint32_t)(A) & 0xff000000) >> 24) | \
                            (((uint32_t)(A) & 0x00ff0000) >> 8) | \
                            (((uint32_t)(A) & 0x0000ff00) << 8) | \
                            (((uint32_t)(A) & 0x000000ff) << 24))
																								 
void yeDumpBuf(char *buf,int len);
u8 yeDataSumCalculate(char *buf,int len);
s8 yeDataSumCheck(char *buf,int len);
int yeDataProcess(char *buf,int len);
int yeDataSendComBine(char cmd,char *buf,int bufLen,char *outBuf );
#endif