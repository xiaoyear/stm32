/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
***/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_iwdg.h"
#include "stm32f10x.h"
#include "stm32config.h"
#include "delay.h"
#include "net.h"
#include "timer.h"

#include "sim800c.h"

#include "stdio.h"
#include "tool.h"



extern void process_com_buf(void);





//初始化独立看门狗
//prer:分频数:0~7(只有低 3 位有效!)
//分频因子=4*2^prer.但最大值只能是 256!
//rlr:重装载寄存器值:低 11 位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
void IWDG_Init(u8 prer,u16 rlr)
{
IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //①使能对寄存器 I 写操作
IWDG_SetPrescaler(prer);  //②设置 IWDG 预分频值:设置 IWDG 预分频值
IWDG_SetReload(rlr);  //②设置 IWDG 重装载值
IWDG_ReloadCounter(); //③按照 IWDG 重装载寄存器的值重装载 IWDG 计数器
IWDG_Enable();  //④使能 IWDG
}
//喂独立看门狗
void IWDG_Feed(void)
{
IWDG_ReloadCounter();//reload
}



//延时函数
void Delay_ms(vu16 time)
{
  vu16 count;
  while( time-- )
  {
    // While SYSCLK = 72M
    count = 0x2000;
    while( count--);
  }
}

//设置PM25复位脚
void set_pm25_rst_pin(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_5);
}

//初始化LED的GPIO
void init_led_gpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_6);//G//GPIO_ResetBits
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);//R//GPIO_SetBits
}

extern s16 tp_val;
extern u16 hm_val;
extern int sim800_state;

u8 flag_print = 0;



//主函数
int main(void)
{
    /* Systmeinit Configuration */
	System_RCC_Configuration();		 //系统时钟初始化
	/* NVIC Configuration */
	NVIC_Configuration();			 //设置NVIC中断分组2————2位抢占优先级，2位响应优先级
	delay_init();
	open_sim800_pwr();
	set_pm25_rst_pin();
	init_led_gpio();
	hcho_adc_Init();
	timer1_init();
	//调试打印串口
	com_init();
	com_usart1(38400);	//GPRS
	net_usart2(9600);	//PM25
	net_val_init();
	TIM2_Int_Init(999,639);      // TIM2 ARR:999 PSC:639


	printf(">>>>>>>>>>>main start[%s %s]>>>>>>>>>>>>\n",__DATE__,__TIME__);

	//看门狗初始化
	IWDG_Init(7,2000);

	while(1)
	{
		sim800_run();
		process_com_buf();
			//串口打印输出温度和湿度参数
	 	if(flag_print || (sim800_state == SIM800_INIT))
	 	{
	 		flag_print = 0;
	 		printf("tp_val = %d.%d\n",tp_val/10,tp_val%10);
	 		printf("hm value = %d\n",hm_val);
	 	}
		//喂狗
		IWDG_Feed();
	}
}
