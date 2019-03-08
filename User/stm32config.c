#include "stm32config.h"
#include "delay.h"


/*******************************************************************************
* Function Name  : SYD_RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
* Configurate parameter:  SYSCLK = 72M, HCLK = 72M, APB1 = 18M, APB2 = 36M,
*                         PLL = enable, HSE: enable, APB2PeriphClock = disable;
*                         APB1PeriphClock = disable.
*******************************************************************************/
void System_RCC_Configuration1(void)
{
  ErrorStatus HSEStartUpStatus;
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();									//RCC ϵͳ��λ,Ĭ��ȱʡֵRCC

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);					//����HSE

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();	//�ȴ�HSE׼����

  if (HSEStartUpStatus == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);	   //AHB clock  = SYSCLK

    /* PCLK2 = HCLK/1 */
    RCC_PCLK2Config(RCC_HCLK_Div1);		   //APB2 clock = HCLK

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);		   //APB1 clock = HCLK/2

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_1);

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 12MHz * 6 = 72 MHz */		//���໷��Ƶ
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);		   //PLLCLK = 8MHz * 9 = 72 MHz	   ����ΪKINCONYHSE�ľ���Ϊ8MHz��
   /* HCLK = SYSCLK */

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);				 //Enable PLL

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)			  //���໷ʱ��Դ׼������
    {}

    /* Select PLL as system clock source */						  //ѡ�����໷ʱ��ԴPLL��Ϊϵͳʱ��SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)						//Wait till PLL is used as system clock source
    {}


      /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                           |RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD
                           | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |RCC_APB2Periph_AFIO, ENABLE);

    /* SPI1 clocks enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);
    /* BKP clocks enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP , ENABLE);
    /* PWR clocks enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);

    /* TIMx clocks enable */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 ,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC���ʱ�䲻�ܳ���14M

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
 //   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* Enable USART3 clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);		   //��ȩ����������ʹ��
    /* Enable USART1 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* Enable USART3 clocks */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);		   //STM32f103����ƽ̨��PM25
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		   //STM32f101����ƽ̨��PM25
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  }
}


void System_RCC_Configuration(void)
{
//  ErrorStatus HSEStartUpStatus;
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();									//RCC ϵͳ��λ,Ĭ��ȱʡֵRCC

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);					//����HSE

 // RCC_HSICmd(ENABLE);
  /* Wait till HSE is ready */
 while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET) { }

  if (1)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);	   //AHB clock  = SYSCLK

    /* PCLK2 = HCLK/1 */
    RCC_PCLK2Config(RCC_HCLK_Div1);		   //APB2 clock = HCLK

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);		   //APB1 clock = HCLK/2

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_1);

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 12MHz * 6 = 72 MHz */		//���໷��Ƶ
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);		   //PLLCLK = 8MHz * 9 = 72 MHz	   ����ΪKINCONYHSE�ľ���Ϊ8MHz��
   /* HCLK = SYSCLK */

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);				 //Enable PLL

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)			  //���໷ʱ��Դ׼������
    {}

    /* Select PLL as system clock source */						  //ѡ�����໷ʱ��ԴPLL��Ϊϵͳʱ��SYSCLK
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)						//Wait till PLL is used as system clock source
    {}


      /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                           |RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD
                           | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |RCC_APB2Periph_AFIO, ENABLE);

    /* SPI1 clocks enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);
    /* BKP clocks enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP , ENABLE);
    /* PWR clocks enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);

    /* TIMx clocks enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC���ʱ�䲻�ܳ���14M

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
 //   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* Enable USART3 clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);		   //��ȩ����������ʹ��
    /* Enable USART1 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* Enable USART3 clocks */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);		   //STM32f103����ƽ̨��PM25
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		   //STM32f101����ƽ̨��PM25
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  }
}

void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
#endif

	/*
	 * Configures the priority grouping:
	 * NVIC_PriorityGroup_2: 2 bits for pre-emption priority
	 *                       2 bits for subpriority
	 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			   //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
}






unsigned int ADCConvertedValue;
#define 	ADC1_DR_Address    ((u32)0x4001244C)



//��ȩ������STM32F101 ����1��ʼ��
void HCHO_Usart1_Init(u32 Boudrate)					        //����1��ʼ��
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);			//USART1��ӳ�䣺��PA9 9A10��ӳ�䵽PB6 PB7


	USART_Cmd(USART1, DISABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


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

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//USART1_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




//ͨ�ö�ʱ��3�жϳ�ʼ����������STM32����ZigBee���ڽ��ճ�ʱ��ʱ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M����Ϊϵͳʱ��Ϊ72M����APB1Ϊ36M������ϵͳ��ʼ�������е�APB1Ԥ��Ƶ��Ϊ2������
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)							//STM32����ZigBee�������ݣ���ʱ�ж�
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	//��1��ʱ��ʹ��

	//��2����ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 				//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
					//��ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR ��TIx) ʹ�õĲ���Ƶ��֮��ķ�Ƶ����������������û�����á�

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���¼���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 		//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 				//��3��ʹ��ָ����TIM3�ж�,��������ж�


	//��4���ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  		//TIM3�ж�	   //TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  	//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  						//��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  	//��5��ʹ��TIMx�����������ﲻ�򿪣�TIM3���ڴ��ڽ��ճ�ʱ��⣬���Ե������жϽ��յ�һ���ֽ�ʱ��
}






//ͨ�ö�ʱ��4�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
/************   ��ȡ��ʪ�ȴ�����DHT11��ֵ���¶� + ʪ�ȣ���������������  ************
 ************                �ж���ʪ�ȵļ�ֵ�����¼�			         ***********/
/************   //����PM2.5��ʱ����---- 5s��һ���ж� +   ************
 ************                			         ***********/
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	//��1��ʱ��ʹ��

	//��2����ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; 				//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
					//��ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR ��TIx) ʹ�õĲ���Ƶ��֮��ķ�Ƶ����������������û�����á�

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���¼���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 		//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 				//��3��ʹ��ָ����TIM4�ж�,��������ж�


	//��4���ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  		//TIM4�ж�	   //TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  	//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  						//��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM4, ENABLE);  	//��5��ʹ��TIMx�����������ﲻ�򿪣�TIM4���ڴ��ڽ��ճ�ʱ��⣬���Ե������жϽ��յ�һ���ֽ�ʱ��

}



//��ʱ��TIM1----��ȩ������ģ����STM32F101�Ĵ��ڳ�ʱ�ж�
void HOCO_TIM1_init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 ,ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM1, DISABLE);
}


void Key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = KEY_PIN;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}
void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		   //wifi reset....wifi reload
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);


	GPIO_ResetBits(BEEP_GPIO, BEEP_PIN);
}


void beep_di(void)
{
	GPIO_ResetBits(BEEP_GPIO, BEEP_PIN);
	GPIO_SetBits(BEEP_GPIO, BEEP_PIN);
	delay_ms(300);
	GPIO_ResetBits(BEEP_GPIO, BEEP_PIN);
	delay_ms(300);
	return;
}

void beep_didi(void)
{
	beep_di();
	beep_di();
}

void beep_didididi(void)
{
	beep_di();
	beep_di();
	beep_di();
	beep_di();
}



extern void AT24C02_Erase_Zero(void);
extern unsigned char serverid[24];


