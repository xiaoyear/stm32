#include "hcho.h"
#include "string.h"

#include "delay.h"



s16 tp_val = 0;
u16 hm_val = 0;


float hcho_value;





void hcho_adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);

	ADC_DeInit(ADC2);

	/* ����ADC1, ����DMA, ������Լ����� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		  //����ģʽ:����鿴�Ĵ���ADC_CR1��λ19:16
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			  //ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode =DISABLE;		  //ģ��ת�������ڵ���ת��ģʽ,��һ��ת�����������or�ⲿ�źŴ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		 //�����������ADCת����ʽ��������ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					 //�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;				  					 //˳����й���ת����ADCͨ������Ŀ1
	ADC_Init(ADC2, &ADC_InitStructure);



	/* Enable ADC1 */
	ADC_Cmd(ADC2, ENABLE);
	ADC_TempSensorVrefintCmd(ENABLE);					 //�����ڲ��¶ȴ�������Vrefͨ��

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC2);							 //ADC��λУ��
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC2));			 //����ȴ�У������

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC2);							 //ADCУ��
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC2));				 //����ȴ�У������
}

u16 tp_tbl[] = {
	3363	,	//	-20
	3329	,	//	-19
	3293	,	//	-18
	3257	,	//	-17
	3220	,	//	-16
	3182	,	//	-15
	3144	,	//	-14
	3104	,	//	-13
	3063	,	//	-12
	3022	,	//	-11
	2980	,	//	-10
	2936	,	//	-9
	2893	,	//	-8
	2848	,	//	-7
	2803	,	//	-6
	2757	,	//	-5
	2711	,	//	-4
	2664	,	//	-3
	2617	,	//	-2
	2569	,	//	-1
	2521	,	//	0
	2473	,	//	1
	2424	,	//	2
	2376	,	//	3
	2327	,	//	4
	2278	,	//	5
	2229	,	//	6
	2180	,	//	7
	2131	,	//	8
	2083	,	//	9
	2037	,	//	10
	1986	,	//	11
	1938	,	//	12
	1891	,	//	13
	1844	,	//	14
	1797	,	//	15
	1751	,	//	16
	1706	,	//	17
	1661	,	//	18
	1617	,	//	19
	1573	,	//	20
	1530	,	//	21
	1487	,	//	22
	1446	,	//	23
	1405	,	//	24
	1365	,	//	25
	1326	,	//	26
	1287	,	//	27
	1249	,	//	28
	1213	,	//	29
	1176	,	//	30
	1141	,	//	31
	1107	,	//	32
	1073	,	//	33
	1040	,	//	34
	1008	,	//	35
	977 ,	//	36
	947 ,	//	37
	918 ,	//	38
	889 ,	//	39
	861 ,	//	40
	834 ,	//	41
	808 ,	//	42
	782 ,	//	43
	757 ,	//	44
	733 ,	//	45
	710 ,	//	46
	687 ,	//	47
	665 ,	//	48
	644 ,	//	49
	621 ,	//	50
	603 ,	//	51
	584 ,	//	52
	565 ,	//	53
	547 ,	//	54
	529 ,	//	55
	512 ,	//	56
	496 ,	//	57
	480 ,	//	58
	464 ,	//	59
	449 ,	//	60
	435 ,	//	61
	421 ,	//	62

0,
0,
};

void hcho_adc_sample(void)
{
	u16 tmp = 0;
	u16 i = 0;

	ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 1, ADC_SampleTime_239Cycles5);  //PB0���ţ�ͨ��8��Ϊ��1��ת������������55.5ADCʱ��

 	ADC_ClearFlag(ADC2, ADC_FLAG_EOC);						  //���ADC��־λ
    	ADC_SoftwareStartConvCmd(ADC2, ENABLE); 				  //�������ADCת��
   	 while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==0);			  //�ж�ADCת������

      tmp  = ADC_GetConversionValue(ADC2);		  //��ȡһ��PM2.5ֵ�����������

	  while(1)
	  {
	  	if(tp_tbl[i] < tmp)
	  	{
			break;
	  	}
		else
		{
			i++;
			if(i>= (sizeof(tp_tbl)/sizeof(u16)))
			{
				break;
			}
		}
	  }
	  //printf("tp_adc = %d\n",tmp);

		tp_val = (tp_tbl[i-1] - tmp)*10 / (tp_tbl[i-1] - tp_tbl[i]) + 10*(i - 21);

	  //printf("tp_val = %d.%d\n",tp_val/10,tp_val%10);

}


const unsigned int TAD[]={	
//90----80---70---60---50---40---30---20--10
//????
3771,3485,2779,1753,774,262,45,8,3, 
3839,3571,2952,1938,964,354,61,12,5, 
3886,3653,3118,2170,1186,496,90,14,6, 
3920,3726,3273,2429,1412,616,119,19,8, 
3954,3793,3387,2659,1645,757,153,29,11, 
3979,3843,3466,2807,1890,936,209,41,15, 
3998,3886,3563,2952,1973,993,218,49,19, 
4020,3918,3638,3088,2150,1107,284,63,26, 
4036,3946,3716,3239,2354,1284,359,79,35, 
4043,3966,3782,3360,2551,1528,433,107,47, 
4050,3983,3821,3475,2740,1698,534,129,65
};
  
//90->92->95 80->82->87->89 70->76.5 60->65 50->54 40->44 30->31.5
void hm_adc_sample(void)
{
	u16 hm_tmp = 0;
	//HG_ADD---------------------------
	u16 Rehum,k,CurAD[9];
	 u8 num;//????????
	//HG_ADD_END

	ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 1, ADC_SampleTime_239Cycles5);  //PB0���ţ�ͨ��8��Ϊ��1��ת������������55.5ADCʱ��

 	ADC_ClearFlag(ADC2, ADC_FLAG_EOC);						  //���ADC��־λ
    	ADC_SoftwareStartConvCmd(ADC2, ENABLE); 				  //�������ADCת��
   	 while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==0);			  //�ж�ADCת������

      hm_tmp  = ADC_GetConversionValue(ADC2);		  //��ȡһ��PM2.5ֵ�����������

	
	//HG ADD----------------------
	//Tem = tp_val;
	 if(tp_val<=0)//0??????0?
	{
		for(num=0;num<9;num++)
		{
	    CurAD[num] = TAD[num];
		}

	}
	else if(tp_val>=500)//??50????
	{
		for(num=0;num<9;num++)
		{
	    CurAD[num] = TAD[90+num];
		}
	}
	else 
	{
	 for(k=1;k<11;k++)//?1??
	 {
	 	if(tp_val<(k*50))	//????10??
		{
		   Rehum = k*50-tp_val;
		   k = 9*k;
			for(num=0;num<9;num++)
			{
			    CurAD[num] = TAD[k+num] - (u32)(TAD[k+num]-TAD[k-9+num])*Rehum/50;//????????
			}
			break;
		}
	 }
	}

	if(hm_tmp<CurAD[8])//10%--
	{
		hm_val =  10-(CurAD[8]-hm_tmp)*10/(CurAD[7]-CurAD[8]);//�ֱ���1
		if(hm_val>20)hm_val=0;
	}
	else
		for(k=1;k<9;k++)//?1??,???   90%>HumAD>80%
		{
			 if(hm_tmp>=CurAD[k])	//???90%-80%-70%-60%-50%-40%
			{
				hm_val =  (u32)(hm_tmp-CurAD[k])*10/(CurAD[k-1]-CurAD[k])+90-k*10;//�ֱ���1
				break;
			}	
		}
	//if(hm_val>=980)hm_val=980;//�ֱ���0.1
		if(hm_val>=98)hm_val=98;


}

