#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
/************************************************
 ALIENTEK Mini STM32F103开发板实验1
 跑马灯实验-HAL库版本
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
         LED0=0;			     	//LED0亮
	     LED1=1;				 	//LED1灭
		 delay_ms(500);
		 LED0=1;					//LED0灭
		 LED1=0;					//LED1亮
		 delay_ms(500);
	 }
}

