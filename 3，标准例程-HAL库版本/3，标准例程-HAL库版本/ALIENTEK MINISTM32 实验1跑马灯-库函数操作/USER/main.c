#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
/************************************************
 ALIENTEK Mini STM32F103开发板实验1
 跑马灯库函数操作实验-HAL库版本
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
/*下面主函数使用位带操作实现：*/

int main(void)
{ 
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9); //设置时钟,72M
	delay_init(72);               	//初始化延时函数
	LED_Init();						//初始化LED	

	while(1)
	{
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET); 	//LED0对应引脚PA8拉低，亮，等同于LED0(0)
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);   	//LED1对应引脚PD2拉高，灭，等同于LED1(1)
        delay_ms(500);											//延时500ms
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);   	//LED0对应引脚PA8拉高，灭，等同于LED0(1)
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET); 	//LED1对应引脚PD2拉低，亮，等同于LED1(0)
        delay_ms(500); 
	 }
}

