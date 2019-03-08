#include "delay.h"



#define SystemCoreClock  64000000   //使用内部时钟，64M

static u8  fac_us=0;			//us延时倍乘数
static u16 fac_ms=0;			//ms延时倍乘数


void delay_init(void)	 
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
	fac_us=SystemCoreClock/8000000;			 
	fac_ms=(u16)fac_us*1000;					  
}
								    

    								   
void delay_us(u32 nus)					 //延时us级别
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 							//时间加载	  		 
	SysTick->VAL=0x00;        							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;         	//开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));					//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       		//关闭计数器
	SysTick->VAL =0X00;       							//清空计数器	 
}


//对72M条件下,nms<=1864 
void delay_ms(u16 nms)					//延时ms级别
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;						//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           						//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));					//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       		//关闭计数器
	SysTick->VAL =0X00;       							//清空计数器	  	    
}
 





