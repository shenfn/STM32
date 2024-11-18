#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
/************************************************
 ALIENTEK Mini STM32F103������ʵ��1
 ����ƿ⺯������ʵ��-HAL��汾
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
/*����������ʹ��λ������ʵ�֣�*/

int main(void)
{ 
    HAL_Init();                    	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9); //����ʱ��,72M
	delay_init(72);               	//��ʼ����ʱ����
	LED_Init();						//��ʼ��LED	

	while(1)
	{
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET); 	//LED0��Ӧ����PA8���ͣ�������ͬ��LED0(0)
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);   	//LED1��Ӧ����PD2���ߣ��𣬵�ͬ��LED1(1)
        delay_ms(500);											//��ʱ500ms
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);   	//LED0��Ӧ����PA8���ߣ��𣬵�ͬ��LED0(1)
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET); 	//LED1��Ӧ����PD2���ͣ�������ͬ��LED1(0)
        delay_ms(500); 
	 }
}

