#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/11/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

TIM_HandleTypeDef 	TIM1_Handler;      	//��ʱ����� 
TIM_OC_InitTypeDef 	TIM1_CH1Handler;	//��ʱ��1ͨ��1���

//TIM1 PWM���ֳ�ʼ�� 
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM1_PWM_Init(u16 arr,u16 psc)
{  
    TIM1_Handler.Instance=TIM1;         	//��ʱ��1
    TIM1_Handler.Init.Prescaler=psc;       	//��ʱ����Ƶ
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM1_Handler.Init.Period=arr;          	//�Զ���װ��ֵ
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM1_Handler);       	//��ʼ��PWM
    
    TIM1_CH1Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM1_CH1Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM1_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH1Handler,TIM_CHANNEL_1);//����TIM1ͨ��1
	
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);//����PWMͨ��1
	 	   
}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();			//ʹ�ܶ�ʱ��1
		__HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
		
		GPIO_Initure.Pin=GPIO_PIN_8;           	//PA8
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;          //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
		HAL_GPIO_Init(GPIOA,&GPIO_Initure); 	
	}
}

//����TIM1ͨ��1��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM1Compare1(u32 compare)
{
	TIM1->CCR1=compare; 
}

//��ȡTIM����/�ȽϼĴ���ֵ
u32 TIM_GetTIM1Capture1(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM1_Handler,TIM_CHANNEL_1);
}

//��ʱ��1�жϷ�����
void TIM1_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);
}

/***************************************************************************
****************************************************************************
  ���������벶�����Դ��ʵ����غ���Դ��
****************************************************************************
****************************************************************************/

TIM_HandleTypeDef TIM5_Handler;         //��ʱ��5���

//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM5��16λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{  
    TIM_IC_InitTypeDef TIM5_CH1Config;  
    
    TIM5_Handler.Instance=TIM5;                          //ͨ�ö�ʱ��5
    TIM5_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM5_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_IC_Init(&TIM5_Handler);						//��ʼ�����벶��ʱ������
    
    TIM5_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    //�����ز���
    TIM5_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//ӳ�䵽TI1��
    TIM5_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          //���������Ƶ������Ƶ
    TIM5_CH1Config.ICFilter=0;                          //���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CH1Config,TIM_CHANNEL_1);//����TIM5ͨ��1
	
    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_1);   //����TIM5�Ĳ���ͨ��1�����ҿ��������ж�
    __HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);   //ʹ�ܸ����ж�
	
	HAL_NVIC_SetPriority(TIM5_IRQn,2,0);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�0
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM5�ж�ͨ��  
}

//��ʱ��5�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_IC_Init()����
//htim:��ʱ��5���
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM5ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_AF_INPUT; 	//������������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    HAL_NVIC_SetPriority(TIM5_IRQn,2,0);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�0
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM5�ж�ͨ��  
}

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���
u8  TIM5CH1_CAPTURE_STA=0;							//���벶��״̬		    				
u16	TIM5CH1_CAPTURE_VAL;							//���벶��ֵ(TIM5��16λ)

//��ʱ��5�жϷ�����
void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM5_Handler);				//��ʱ�����ô�����
}
 
//��ʱ�������жϣ�����������жϴ���ص������� �ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�����жϣ����������ʱִ��
{
	if((TIM5CH1_CAPTURE_STA&0X80)==0)				//��δ�ɹ�����
	{
		if(TIM5CH1_CAPTURE_STA&0X40)				//�Ѿ����񵽸ߵ�ƽ��
		{
			if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)	//�ߵ�ƽ̫����
			{
				TIM5CH1_CAPTURE_STA|=0X80;			//��ǳɹ�������һ��
				TIM5CH1_CAPTURE_VAL=0XFFFF;
			}else TIM5CH1_CAPTURE_STA++;
		}	 
	}		
}

//��ʱ�����벶���жϴ���ص��������ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//�����жϷ���ʱִ��
{
	if((TIM5CH1_CAPTURE_STA&0X80)==0)				//��δ�ɹ�����
	{
		if(TIM5CH1_CAPTURE_STA&0X40)				//����һ���½��� 		
		{	  			
			TIM5CH1_CAPTURE_STA|=0X80;				//��ǳɹ�����һ�θߵ�ƽ����
            TIM5CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
			TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
            TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//����TIM5ͨ��1�����ز���
		}else  										//��δ��ʼ,��һ�β���������
		{
			TIM5CH1_CAPTURE_STA=0;					//���
			TIM5CH1_CAPTURE_VAL=0;
			TIM5CH1_CAPTURE_STA|=0X40;				//��ǲ�����������
			__HAL_TIM_DISABLE(&TIM5_Handler);      	//�رն�ʱ��5
			__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);
			TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
			TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//��ʱ��5ͨ��1����Ϊ�½��ز���
			__HAL_TIM_ENABLE(&TIM5_Handler);		//ʹ�ܶ�ʱ��5
		}		    
	}		
}
