
#include "stm32config.h"
#include "stdio.h"
#include "stdlib.h"
#include "delay.h"

#include "net.h"
#include "string.h"

#include "timer.h"




typedef enum
{
  true=0,
  false=-1,
  other=1,
}BOOL_MODE;

unsigned char net_link=NET_ERR_MODE;
unsigned char netbuf[NETBUF_SIZE];
static NET_BUFFER net_buffer;





#define	NET_TX_PIN				GPIO_Pin_2
#define	NET_RX_PIN				GPIO_Pin_3
#define	NET_GPIO					GPIOA

#define	COM_TX_PIN				GPIO_Pin_10
#define	COM_RX_PIN				GPIO_Pin_11
#define	COM_GPIO					GPIOB






void net_val_init(void)
{
	memset((char*)&net_buffer,0,NET_BUFFER_LEN);
	net_buffer.rxbuffer=netbuf;
}

void net_buffer_input(unsigned char byte)
{

	net_buffer.rxbuffer[net_buffer.t_pos++]=byte;

	if(net_buffer.t_pos==NETBUF_SIZE)
		net_buffer.t_pos=0;

	if(net_buffer.t_pos>net_buffer.r_pos && net_buffer.rxcounter==NETBUF_SIZE)
	{
		net_buffer.r_pos++;

		if(net_buffer.r_pos==NETBUF_SIZE)
			net_buffer.r_pos=0;
	}

	net_buffer.rxcounter++;
	if(net_buffer.rxcounter>NETBUF_SIZE)
		net_buffer.rxcounter=NETBUF_SIZE;
}


void net_usart2(u32 Boudrate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);



	USART_Cmd(USART2, DISABLE);

	GPIO_InitStructure.GPIO_Pin = NET_TX_PIN;				//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(NET_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =NET_RX_PIN;				//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(NET_GPIO, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = Boudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

    /* Enable the USART1 */
    USART_Cmd(USART2, ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			//USART1_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void com_usart3(u32 Boudrate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);



	USART_Cmd(USART3, DISABLE);

	GPIO_InitStructure.GPIO_Pin = COM_TX_PIN;				//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(COM_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =COM_RX_PIN;				//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(COM_GPIO, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = Boudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

    /* Enable the USART1 */
    USART_Cmd(USART3, ENABLE);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			//USART1_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART3, (unsigned char) ch);
  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
  return (ch);
}

void USART2_Putc(unsigned char c)
{
    USART_SendData(USART3, c);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

void USART2_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART3, *str++);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
}

int GetKey (void) {

while (!(USART3->SR & USART_FLAG_RXNE));

return ((int)(USART3->DR & 0x1FF));

}



//串口接收缓存长度
#define COM_REC_LEN		200

//最小命令长度
#define COM_LEN			30

//缓存  环形队列
u8 com_rec_buf[COM_REC_LEN] = {0};
//队列头
u8 start = 0;
//队列尾
u8 end = 0;


void clr_buf(void)
{
	start = 0;
	end = 0;
	memset(com_rec_buf, 0, sizeof(com_rec_buf));
}

//缓存中存入一个字节
void add_one_byte(u8 data)
{
	com_rec_buf[end] = data;
	end++;
	if(end >= COM_REC_LEN)
	{
		end = 0;
	}
}

//缓存中删除一个字节
void del_one_byte(void)
{
	start++;
	if(start >= COM_REC_LEN)
	{
		start = 0;
	}
}

//获取缓存长度
u8 get_buf_len(void)
{
	u8 len = 0;

	if(end >= start)
	{
		len = end - start;
	}
	else
	{
		len = COM_REC_LEN + start - end;
	}
	return len;
}

void del_bytes(u8 n)
{
	if(get_buf_len() < n)
	{
		start = end;
	}
	else
	{
		start = start + n;
		if(start >= COM_REC_LEN)
		{
			start = start - COM_REC_LEN;
		}
	}
}


u16 pm25_data = 0;


//解析串口数据
void process_com_buf(void)
{
	u8 buf_len = 0;

	buf_len = get_buf_len();


	if(buf_len >= COM_LEN)
	{

		if((com_rec_buf[0] == 0x42)
			&& (com_rec_buf[1] == 0x4d)
			)
		{
			pm25_data = (com_rec_buf[12] << 8) + com_rec_buf[13];

			/* mod by xiaoye
			if(pm25_data < 80)
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);//G
				GPIO_SetBits(GPIOB,GPIO_Pin_7);//R
			}
			else
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_6);//G
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);//R
			}
			*/
		}
	}


}

//串口3接收中断
void USART3_IRQHandler(void)
{
	u8 byte;	                      //存放接收数据字节（8 bits）的临时变量

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)   			//判断接收标志位
	{
		byte = USART_ReceiveData(USART3);						//读取接收到的数据


		USART_ClearITPendingBit(USART3,USART_IT_RXNE);		//清除USART1串口接收中断标志位
    	}

}

void USART2_IRQHandler(void)
{
	u8 byte;	                      //存放接收数据字节（8 bits）的临时变量


	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   			//判断接收标志位
	{
		byte = USART_ReceiveData(USART2);						//读取接收到的数据


		add_one_byte(byte);

		USART_ClearITPendingBit(USART2,USART_IT_RXNE);		//清除USART1串口接收中断标志位
  }

}


void com_init(void)
{
	com_usart3(115200);
}


void com_send(unsigned char *p,u32 num)
{
	unsigned char *k=p;
	while(num--)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		USART_SendData(USART2,*k++);									 	//串口发送

	}
}

void net_send(unsigned char *p,u32 num)
{
	u8 AT_SEND_START[] = {"AT+CIPSEND\n"};
	u8 AT_SEND_END = 0X1A;
	sim800_send(AT_SEND_START,sizeof(AT_SEND_START));
	delay_ms(100);
	sim800_send(p,num);
	sim800_send(&AT_SEND_END,1);
}



