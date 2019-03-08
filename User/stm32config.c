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
  RCC_DeInit();									//RCC 系统复位,默认缺省值RCC

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);					//开启HSE

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();	//等待HSE准备好

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

    /* PLLCLK = 12MHz * 6 = 72 MHz */		//锁相环倍频
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);		   //PLLCLK = 8MHz * 9 = 72 MHz	   （因为KINCONYHSE的晶振为8MHz）
   /* HCLK = SYSCLK */

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);				 //Enable PLL

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)			  //锁相环时钟源准备就绪
    {}

    /* Select PLL as system clock source */						  //选择锁相环时钟源PLL作为系统时钟SYSCLK
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
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC最大时间不能超过14M

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
 //   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* Enable USART3 clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);		   //甲醛传感器串口使用
    /* Enable USART1 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* Enable USART3 clocks */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);		   //STM32f103主机平台的PM25
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		   //STM32f101主机平台的PM25
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  }
}


void System_RCC_Configuration(void)
{
//  ErrorStatus HSEStartUpStatus;
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();									//RCC 系统复位,默认缺省值RCC

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);					//开启HSE

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

    /* PLLCLK = 12MHz * 6 = 72 MHz */		//锁相环倍频
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);		   //PLLCLK = 8MHz * 9 = 72 MHz	   （因为KINCONYHSE的晶振为8MHz）
   /* HCLK = SYSCLK */

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);				 //Enable PLL

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)			  //锁相环时钟源准备就绪
    {}

    /* Select PLL as system clock source */						  //选择锁相环时钟源PLL作为系统时钟SYSCLK
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
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC最大时间不能超过14M

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
 //   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* Enable USART3 clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);		   //甲醛传感器串口使用
    /* Enable USART1 clocks */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* Enable USART3 clocks */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);		   //STM32f103主机平台的PM25
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		   //STM32f101主机平台的PM25
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			   //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}






unsigned int ADCConvertedValue;
#define 	ADC1_DR_Address    ((u32)0x4001244C)



//甲醛传感器STM32F101 串口1初始化
void HCHO_Usart1_Init(u32 Boudrate)					        //串口1初始化
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);			//USART1重映射：将PA9 9A10重映射到PB6 PB7


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




//通用定时器3中断初始化————STM32接收ZigBee串口接收超时定时器
//这里时钟选择为APB1的2倍，而APB1为36M：因为系统时钟为72M，而APB1为36M。所以系统初始化函数中的APB1预分频数为2！！！
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)							//STM32接收ZigBee串口数据，超时判断
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	//（1）时钟使能

	//（2）定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; 				//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
					//定时器时钟(CK_INT)频率与数字滤波器(ETR ，TIx) 使用的采样频率之间的分频比例——该例程中没有作用。

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向下计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 		//根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 				//（3）使能指定的TIM3中断,允许更新中断


	//（4）中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  		//TIM3中断	   //TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  	//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  						//初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  	//（5）使能TIMx————这里不打开，TIM3用于串口接收超时检测，所以到串口中断接收第一个字节时打开
}






//通用定时器4中断初始化
//arr：自动重装值。
//psc：时钟预分频数
/************   读取温湿度传感器DHT11的值（温度 + 湿度），并进行修正，  ************
 ************                判断温湿度的极值触发事件			         ***********/
/************   //华数PM2.5定时发送---- 5s进一次中断 +   ************
 ************                			         ***********/
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	//（1）时钟使能

	//（2）定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; 				//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
					//定时器时钟(CK_INT)频率与数字滤波器(ETR ，TIx) 使用的采样频率之间的分频比例——该例程中没有作用。

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向下计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 		//根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 				//（3）使能指定的TIM4中断,允许更新中断


	//（4）中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  		//TIM4中断	   //TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  	//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  						//初始化NVIC寄存器


	TIM_Cmd(TIM4, ENABLE);  	//（5）使能TIMx————这里不打开，TIM4用于串口接收超时检测，所以到串口中断接收第一个字节时打开

}



//定时器TIM1----甲醛传感器模块与STM32F101的串口超时判断
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


