#include "sys.h"
#include "delay.h"
#include "usart.h"
/************************************************
 ALIENTEK Mini STM32F103开发板实验0-1
 Template工程模板-新建工程章节使用-HAL库版本
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


/***注意：本工程和教程中的新建工程3.3小节对应***/


void Delay(__IO uint32_t nCount);

void Delay(__IO uint32_t nCount)
{
  while(nCount--){}
}

int main(void)
{
	GPIO_InitTypeDef GPIO_Initure;
     
    HAL_Init();                    	 			//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   			//设置时钟,72M

    __HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();           	//开启GPIOD时钟
	
    GPIO_Initure.Pin=GPIO_PIN_8; 				//PA8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_2; 				//PD2
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	while(1)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);		//PA8置1 
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);		//PD2置1  			
		Delay(0x7FFFFF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);		//PA8置0
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);		//PD2置0  
		Delay(0x7FFFFF);
	}
}
