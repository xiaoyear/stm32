#include "delay.h"



#define SystemCoreClock  64000000   //ʹ���ڲ�ʱ�ӣ�64M

static u8  fac_us=0;			//us��ʱ������
static u16 fac_ms=0;			//ms��ʱ������


void delay_init(void)	 
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
	fac_us=SystemCoreClock/8000000;			 
	fac_ms=(u16)fac_us*1000;					  
}
								    

    								   
void delay_us(u32 nus)					 //��ʱus����
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 							//ʱ�����	  		 
	SysTick->VAL=0x00;        							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;         	//��ʼ����	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));					//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       		//�رռ�����
	SysTick->VAL =0X00;       							//��ռ�����	 
}


//��72M������,nms<=1864 
void delay_ms(u16 nms)					//��ʱms����
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;						//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           						//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));					//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       		//�رռ�����
	SysTick->VAL =0X00;       							//��ռ�����	  	    
}
 




