#include "string.h"
#include "tool.h"

extern int conFlag;
void yeDumpBuf(char *buf,int len)
{
	u8 i=0; 
	printf("*****dumpBuf len:%d*****\n",len);
	for(;i<len;i++)
	{
		printf("%02x",buf[i]);
	}
	printf("\n");
}

u8 yeDataSumCalculate(char *buf,int len)
{
	u8 sum=0;
	u8 i=0; 
	for(;i<len;i++)
	{
		sum += buf[i];
	}
	sum = (u8)~sum;
	printf("data calculate sum:%02x\n",sum);
	return sum;
}

s8 yeDataSumCheck(char *buf,int len)
{
	u8 sum=0;
	u8 i=0; 
	for(;i<len-1;i++)
	{
		sum += buf[i];
	}
	yeDumpBuf(buf,len);
	sum = (u8)~sum;
	printf("local calculate sum:%02x|recv sum:%02x\n",sum,buf[len-1]);
	if(sum == buf[len-1])
	{
		printf("sum true\n");
		return 0;
	}
	else
	{
		printf("sum false\n");
		return -1;
	}
}

u32 reportInterval = 1000;//10s
int yeDataProcess(char *buf,int len)
{
	//1(header) 4(dev) 1(cmd) 4(sn) json 1(result) 1(sum)
	char header = buf[0];
	char devNum[4] = {0};
	char cmd = buf[5];
	char sn[4] = {0};
	char result = buf[len-2];
	char sum = buf[len-1];
	char json[50] = {0};
	memcpy(devNum,&buf[1],4);
	memcpy(sn,&buf[6],4);
	if(len-12>0)
	{
			memcpy(json,&buf[10],len-12);
	}
		
	printf("header:%02x\n",header);
	printf("devNum:%02x%02x%02x%02x\n",devNum[0],devNum[1],devNum[2],devNum[3]);
	printf("cmd   :%02x\n",cmd);
	printf("sn    :%02x%02x%02x%02x\n",sn[0],sn[1],sn[2],sn[3]);
	printf("result:%02x\n",result);
	printf("sum   :%02x\n",sum);
	printf("json  :%s\n",json);
	if(header == 0xbb)
	{
		if(cmd == 0x20)
		{
			printf("need mod report time:d\n",json[0]);
			reportInterval = json[0]*10;
		}
	}
	if(header == 0xaa)
	{
		if(cmd == 0 && result == 0x03)
		{
			printf("login cloud success!\n");
			GPIO_ResetBits(GPIOB,GPIO_Pin_6);//G
			GPIO_SetBits(GPIOB,GPIO_Pin_7);//R
			conFlag = 1;
		}
	}
		
}

int yeDataSendComBine(char cmd,char *buf,int bufLen,char *outBuf )
{
	static int sendSn = 0;
	static int devNum = 0x78453412;//0x12344578
	int tmpInt;
	int pos = 0;
	outBuf[pos] = 0xaa;
	pos += 1;
	tmpInt = BigLittleSwap32(devNum);
	memcpy(&outBuf[pos],&tmpInt,4);
	pos += 4;
	outBuf[pos] = cmd;
	pos += 1;
	tmpInt = BigLittleSwap32(sendSn);
	memcpy(&outBuf[pos],&tmpInt,4);
	pos += 4;
	sendSn ++;
	memcpy(&outBuf[pos],buf,bufLen);
	pos += bufLen;
	outBuf[pos] = 0;
	pos += 1;
	outBuf[pos] = yeDataSumCalculate(outBuf,pos);
	pos += 1;
	yeDumpBuf(outBuf,pos);
	return pos;
}
