#include "stm32f10x.h"
#include "stm32config.h"
#include "stdio.h"
#include "stdlib.h"
#include "delay.h"
#include "net.h"
#include "string.h"

#include "timer.h"


TIMER timer[TIMER_MAX_NUM];

u8 smart_link_timer2=0;
u32 smart_link_cnt=0;


void Delay_us(vu16 time)
{
    vu16 count;
    while( time-- )
    {
        // While SYSCLK = 72M
        count = 0x08;
        while( count--) ;
    }
}


void TIM2_Int_Init(u16 arr,u16 psc)							//STM32����ZigBee�������ݣ���ʱ�ж�
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 	//��1��ʱ��ʹ��

	//��2����ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 				//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
					//��ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR ��TIx) ʹ�õĲ���Ƶ��֮��ķ�Ƶ����������������û�����á�

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		      //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 				        //��3��ʹ��ָ����TIM3�ж�,��������ж�


	//��4���ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  		       //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;  	     //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		       //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  						               //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM2, ENABLE);  	//��5��ʹ��TIMx�����������ﲻ�򿪣�TIM3���ڴ��ڽ��ճ�ʱ��⣬���Ե������жϽ��յ�һ���ֽ�ʱ��
}



void close_smartlink_timer2(void)
{
	smart_link_timer2=TIMER_CLOSE;

   	TIM_Cmd(TIM2, DISABLE);
}




void start_smartlink_timer2(void)
{
	TIM_Cmd(TIM2, DISABLE);
	smart_link_timer2=TIMER_START;
	smart_link_cnt=0;
   	TIM2->CNT=0;
   	TIM_Cmd(TIM2, ENABLE);
}


extern u16 pm25_data;
extern s16 tp_val;
extern u16 hm_val;

extern u8 flag_print;
extern u32 reportInterval;


u16 time_sync = 0;

u8 flag_hum = 0;
u16 flag_cnt = 0;


void timer2_irq(void)
{
	u8 i=0;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  	//���TIM3�����жϷ�����񣨼�鴮�ڽ��ճ�ʱ�Ƿ�����
	{

			for(i=0;i<TIMER_MAX_NUM;i++)
			{
				if(timer[i].timer_run)
				{
					if(timer[i].timer_count==timer[i].timer_value)
					{
						timer[i].timer_timeout=1;
						timer[i].timer_run=0;
					}

					timer[i].timer_count++;
				}
			}

	//����ʱ�ϱ�����
		if(net_link == NET_SERVER_LINK_OK)
		{
			time_sync++;
			if(time_sync > reportInterval)//10s
			{
				time_sync = 0;
				sync_data(tp_val,pm25_data,hm_val);
			}
			if((time_sync % 200) == 0)
			{
				read_pm();
				flag_print = 1;
			}
		}



			flag_cnt++;
			if(flag_cnt > 500)
			{
				flag_cnt = 0;
				Delay_us(100);
				hcho_adc_sample();
			}
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update );
}



 void timer_all_init(void)
{
	memset((char*)timer,0,TIMER_MAX_NUM*TIMER_LEN);
}


void timer_signal_init(unsigned int num)
{
	memset((char*)&timer[num],0,TIMER_LEN);
}

void timer_signal_set(unsigned int num,unsigned int value)
{
	timer[num].timer_count=0;
	timer[num].timer_value=value;
	timer[num].timer_timeout=0;
	timer[num].timer_run=1;
}

int timer_get_state(unsigned int num)
{
	return timer[num].timer_timeout;
}

int timer_get_run(unsigned int num)
{
   return timer[num].timer_run;
}


void timer_clr_state(unsigned int num)
{
   timer[num].timer_timeout=0;
}

/*
void RTC_IRQHandler(void)							  //RTC�ж�1s��һ�θ��жϺ���
{
	unsigned char i;
	if(RTC_GetFlagStatus(RTC_IT_SEC))
	{
		for(i=0;i<MAX_CLIENT;i++)
		{
			if(client_id[i].client[0]!=0)
			{
				if(client_id[i].timer_count++>60)
					client_id[i].client[0]=0;
			}
		}

		for(i=0;i<TIMER_MAX_NUM;i++)
		{
			if(timer[i].timer_run)
			{
				if(timer[i].timer_count==timer[i].timer_value)
				{
					timer[i].timer_timeout=1;
					timer[i].timer_run=0;
				}

				timer[i].timer_count++;
			}
		}

		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
	return ;
}
*/
void timer1_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 ,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler =639/2 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 99;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM1, ENABLE);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE );
}

u16 flag_hm_cnt = 0;
void TIM1_UP_IRQHandler(void)   	//TIM3�ж�
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  	//���TIM3�����жϷ�����񣨼�鴮�ڽ��ճ�ʱ�Ƿ�����
	{
		if(flag_hum)
		{
			flag_hum = 0;
		//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
		//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
			flag_hm_cnt++;
			if(flag_hm_cnt > 2000)
			{
				flag_hm_cnt = 0;
				Delay_us(100);
				hm_adc_sample();
			}
		}
		else
		{
			flag_hum = 1;
		//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		//	GPIO_SetBits(GPIOB,GPIO_Pin_1);
				GPIO_SetBits(GPIOB,GPIO_Pin_0);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update );  		//���TIMx�����жϱ�־���ֶ��������жϱ�־λ����
	}
}


