#include "system_stm32f10x.h"
#include "stm32config.h"
#include "sim800c.h"

#include "net.h"
#include "stdio.h"
#include "string.h"

u8 sim800_rec_buf[512] = {0};
u16 pos = 0;

const u8 AT_CHECK_SIM[]={"AT+CPIN?\n"};
const u8 AT_CHECK_GPRS[]={"AT+CGATT?\n"};
const u8 AT_CSTT[]={"AT+CSTT=CMNET\n"};
const u8 AT_CIIR[]={"AT+CIICR\n"};
const u8 AT_CIFSR[]={"AT+CIFSR\n"};

const u8 AT_SHUT[] = {"AT+CIPSHUT\n"};
//TCP服务IP和端口设置
const u8 AT_CIPSTART[]={"AT+CIPSTART=TCP,139.224.16.58,7071\n"};

int init_state = INIT_CHECK_SIM;
int init_state_timer = INIT_CHECK_SIM;

int sim800_state = SIM800_INIT;

volatile int flagStartRec = 0;
int conFlag = 0;



//强制复位
__asm void SystemReset(void)				                        //STM32芯片的系统软复位
{
 MOV R0, #1           //;
 MSR FAULTMASK, R0    //; 清除FAULTMASK 禁止一切中断产生
 LDR R0, =0xE000ED0C  //;
 LDR R1, =0x05FA0004  //;
 STR R1, [R0]         //; 系统软件复位

deadloop
    B deadloop        //; 死循环使程序运行不到下面的代码
}


//打开SIM800电源
void open_sim800_pwr(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	delay_ms(2000);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
}

//关闭SIM800电源
void close_sim800_pwr(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	delay_ms(2000);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
	delay_ms(1000);
}


void com_usart1(u32 Boudrate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_Cmd(USART1, DISABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = Boudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

    /* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


//串口接收中断
void USART1_IRQHandler(void)
{
	u8 byte;	                      //存放接收数据字节（8 bits）的临时变量


	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)   			//判断接收标志位
	{
		byte = USART_ReceiveData(USART1);						//读取接收到的数据

		if(sim800_state != SIM800_INIT)
		{
			flagStartRec = 1;
			//wifi_buffer_input(byte);
			sim800_rec_buf[pos++] = byte;
		}
		else
		{
			sim800_rec_buf[pos++] = byte;
		}


		USART_ClearITPendingBit(USART1,USART_IT_RXNE);		//清除USART1串口接收中断标志位
    	}

}

//SIM800发送
void sim800_send(unsigned char *p,u32 num)
{
	unsigned char *k=p;
	if(sim800_state == SIM800_INIT)
	{
		memset(sim800_rec_buf,0,sizeof(sim800_rec_buf));
		pos = 0;
	}

	while(num--)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1,*k++);									 	//串口发送
	}

}

#define ERR_CHECK_MAX	15
u8 check_err_cnt  =0;	//自检出错的次数

//检查状态
void check_init_state(void)
{
	check_err_cnt++;
	if(check_err_cnt > ERR_CHECK_MAX)
	{
		printf("sim800 init err,sys reset\n");
		sim800_send((unsigned char*)AT_SHUT,sizeof(AT_SHUT));
		close_sim800_pwr();
		SystemReset();
	}
}

//初始化SIM800
void init_sim800(void)
{
	switch(init_state)
	{
		case INIT_CHECK_SIM:
		{
			sim800_send((unsigned char*)AT_CHECK_SIM,sizeof(AT_CHECK_SIM));
			init_state = INIT_TIMER;
			init_state_timer = INIT_CHECK_SIM_DONE;
		}
		break;

		case INIT_CHECK_SIM_DONE:
		{
			if(strncmp(sim800_rec_buf+11,"+CPIN: READY",12) == 0)
			{
				init_state = INIT_CHECK_GPRS;
				printf("check sim card ok.\n");
			}
			else
			{
				printf("check sim card error.[%s]\n",sim800_rec_buf+11);
				init_state = INIT_CHECK_SIM;
				check_init_state();
			}
		}
		break;

		case INIT_CHECK_GPRS:
		{
			sim800_send((unsigned char*)AT_CHECK_GPRS,sizeof(AT_CHECK_GPRS));
			//init_state = INIT_TIMER_LONG;
			init_state = INIT_TIMER;
			init_state_timer = INIT_CHECK_GPRS_DONE;
		}
		break;

		case INIT_CHECK_GPRS_DONE:
		{
			if(strncmp(sim800_rec_buf+12,"+CGATT: 1",9) == 0)
			{
				init_state = INIT_SET_APN;
				printf("check GPRS ok.\n");
			}
			else
			{
				printf("check GPRS error.[%s]\n",sim800_rec_buf);
				init_state = INIT_CHECK_GPRS;
				check_init_state();
			}
		}
		break;

		case INIT_SET_APN:
		{
			sim800_send((unsigned char*)AT_CSTT,sizeof(AT_CSTT));
			init_state = INIT_TIMER;
			init_state_timer = INIT_SET_APN_DONE;
		}
		break;

		case INIT_SET_APN_DONE:
		{
			if(strncmp(sim800_rec_buf+16,"OK",2) == 0)
			{
				init_state = INIT_CREATE_NET;
				printf("set APN ok.\n");
			}
			else
			{
				printf("set APN error.[%s]\n",sim800_rec_buf);
				init_state = INIT_SET_APN;
				check_init_state();
			}
		}
		break;

		case INIT_CREATE_NET:
		{
			sim800_send((unsigned char*)AT_CIIR,sizeof(AT_CIIR));
			init_state = INIT_TIMER;
			init_state_timer = INIT_CREATE_NET_DONE;
		}
		break;

		case INIT_CREATE_NET_DONE:
		{
			if(strncmp(sim800_rec_buf+11,"OK",2) == 0)
			{
				init_state = INIT_GET_IP;
				printf("create net ok.\n");
			}
			else
			{
				printf("create net error.[%s]\n",sim800_rec_buf);
				init_state = INIT_CREATE_NET;
				check_init_state();
			}
		}
		break;

		case INIT_GET_IP:
		{
			sim800_send((unsigned char*)AT_CIFSR,sizeof(AT_CIFSR));
			init_state = INIT_TIMER;
			init_state_timer = INIT_GET_IP_DONE;
		}
		break;

		case INIT_GET_IP_DONE:
		{
			if(pos > 23)
			{
				init_state = INIT_SET_SOCKET;
				printf("get IP ok >>> %s\n",sim800_rec_buf+11);
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);//G
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//R
			}
			else
			{
				printf("get IP error.[%s]\n",sim800_rec_buf);
				init_state = INIT_GET_IP;
				check_init_state();
			}
		}
		break;

		case INIT_SET_SOCKET:
		{
			sim800_send((unsigned char*)AT_CIPSTART,sizeof(AT_CIPSTART));
			init_state = INIT_TIMER;
			init_state_timer = INIT_SET_SOCKET_DONE;
		}
		break;

		case INIT_SET_SOCKET_DONE:
		{
			//if(strncmp(sim800_rec_buf+43,"CONNECT OK",10) == 0)
			if((strncmp(sim800_rec_buf+44,"CONNECT OK",10) == 0)
				|| (strncmp(sim800_rec_buf+44,"ALREADY CONNECT",7) == 0)
				|| (strncmp(sim800_rec_buf+45,"CONNECT OK",10) == 0)
				|| (strncmp(sim800_rec_buf+45,"ALREADY CONNECT",7) == 0)
				|| (strncmp(sim800_rec_buf+46,"CONNECT OK",10) == 0)
				|| (strncmp(sim800_rec_buf+46,"ALREADY CONNECT",7) == 0)
				|| (strncmp(sim800_rec_buf+43,"CONNECT OK",10) == 0)
				|| (strncmp(sim800_rec_buf+43,"ALREADY CONNECT",7) == 0)
				)
			{
				init_state = INIT_CHECK_SIM;
				sim800_state = SIM800_GPRS_REC_WAIT;
				net_link = NET_SERVER_LINK_OK;
				printf("set socket ok.\n");
			}
			else
			{
				printf("set socket error.>>>%s\n",sim800_rec_buf+43);
				init_state = INIT_SET_SOCKET;
				check_init_state();
			}
		}
		break;

		case INIT_TIMER:
		{
			u32 num = 0x007Fffff;
			while(num--)
			{
				IWDG_Feed();
			};
			init_state = init_state_timer;
		}
		break;

		case INIT_TIMER_LONG:
		{
			u32 num = 0x01Ffffff;
			while(num--)
			{
				IWDG_Feed();
			};
			init_state = init_state_timer;
		}
		break;

		default:
			printf("case none[%s][%d]\n",__FILE__,__LINE__);
		break;
	}
}

//SIM800接收
void sim800_recevie(void)
{
	if(pos > 0)
	{
		int i = 0;
		for(i=0; i<pos; i++)
		{
			net_buffer_input(sim800_rec_buf[i]);
		}
	}
}

//SIM800运行函数
void sim800_run(void)
{
	char recvRecv = 0;
	switch(sim800_state)
	{
		case SIM800_INIT:
		{
			init_sim800();
		}
		break;

		case SIM800_GPRS_REC:
		{
		}
		break;

		case SIM800_GPRS_REC_WAIT:
		{
			if(flagStartRec == 1)
			{
				sim800_state = SIM800_TIMER_SHORT;
				printf("sim800:start receive data >>>\n");
			}
		}
		break;

		case SIM800_TIMER_SHORT:
		{
			u32 num = 0x000fffff;
			int i = 0;
			while(num--)
			{
				IWDG_Feed();
			};

			flagStartRec = 0;
			for(i=0; i<(pos+1); i++)
			{
				net_buffer_input(sim800_rec_buf[i]);
			}
			printf("rec len:%d,buf:%s\n",pos,sim800_rec_buf);
			for(i=0; i<(pos+1); i++)
			{
				//printf("%02x",sim800_rec_buf[i]);
				if(sim800_rec_buf[i]==0xaa || sim800_rec_buf[i]==0xbb )
				{
					printf("recv header:%02x\n",sim800_rec_buf[i]);
					recvRecv = 1;
					if(yeDataSumCheck(&sim800_rec_buf[i],pos-i) == 0)
					{
						yeDataProcess(&sim800_rec_buf[i],pos-i);
						break;
					}
				}
			}
			
			if(strstr(sim800_rec_buf,"+PDP: DEACT") != NULL)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_6);//G//GPIO_ResetBits
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//R//GPIO_SetBits
				printf("PDP error,dev offline.\n");
				sim800_state = SIM800_INIT;
				init_state = INIT_TIMER;
				init_state_timer = INIT_CHECK_SIM;
				net_link = NET_NOLINK_ROUTE;
				break;
			}
			else if(strstr(sim800_rec_buf,"CONNECT FAIL") != NULL)
			{
				printf("tcp socket error,dev offline.\n");
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);//G//GPIO_ResetBits
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//R//GPIO_SetBits
				sim800_state = SIM800_INIT;
				init_state = INIT_TIMER;
				init_state_timer = INIT_CHECK_SIM;
				net_link = NET_ROUTE_LINK_OK;
				break;
			}
			printf("\n");
			pos = 0;
			memset(sim800_rec_buf,0,sizeof(sim800_rec_buf));
			sim800_state = SIM800_GPRS_REC_WAIT;
			printf("sim800:receive packet done\n");
		}
		break;

		default:
			printf("case none[%s][%d]\n",__FILE__,__LINE__);
		break;
	}
}


//发送数据给SIM800
void send_sync_data(u8 *p,u32 num)	 
{
	net_send(p,num);
}

//发送数据
void sync_data(s16 tmp, u16 pm,u16 hm)
{
	//u8 str[64] ={0};
	//memset(str,0,sizeof(str));
	//sprintf((char*)str,"\nUAIR-SYNC,PM:%d,TP:%d,HM:%d\n\n",pm,tmp,hm);
	//send_sync_data(str,strlen(str));
	//printf("sync data:%s\n",str);
	//int i=0;
	//u8 hexStr[] = {0xAA,0x78,0x45,0x34,0x12,0x00,0x00,0x00,0x00,0x01,0x7B,0x20,0x22,0x55,0x55,0x49,0x44,0x22,0x3A,0x22,0x37,0x38,0x34,0x35,0x33,0x34,0x31,0x32,0x22,0x7D,0x00,0x9E};
	//printf("sync data len:%d\n",sizeof(hexStr));
	//yeDumpBuf(hexStr,pos);
	
	if(conFlag == 0)
	{
		char hexStr[100] = {0};
		char jsonBuf[] = "{ \"UUID\":\"78453412\"}";
		int len = 0;
		yeDumpBuf(jsonBuf,sizeof(jsonBuf));
		len = yeDataSendComBine(0,jsonBuf,sizeof(jsonBuf)-1,hexStr);
		send_sync_data(hexStr,len);
	}
	else
	{
		char hexStr[100] = {0};
		char jsonBuf[50] = {0};
		//printf("tp_val = %d.%d\n",tp_val/10,tp_val%10);
		char json[] = "{\"T\":\"%d.%d\",\"H\":\"%d\"}";//{"T":"22.2","H":"15.8"}
		int len = 0;
		len = sprintf(jsonBuf,json,tmp/10,tmp%10,hm);
		yeDumpBuf(jsonBuf,len);
		printf("%s\n",jsonBuf);
		len = yeDataSendComBine(0x90,jsonBuf,len,hexStr);
		send_sync_data(hexStr,len);
	}

}

//读PM25传感器
void read_pm(void)
{
	u8 ss[] = {0x42,0x4d,0xe2,0,0,0x02,0x11};
	clr_buf();
	com_send(ss,sizeof(ss));
}



