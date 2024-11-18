#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "malloc.h" 
/************************************************
 ALIENTEK Mini STM32F103开发板实验27
 内存管理实验  -HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{ 
	u8 key;		 
 	u8 i=0;	    
	u8 *p=0;
	u8 *tp=0;
	u8 paddr[18];						//存放P Addr:+p地址的ASCII值 
	
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);					//初始化串口
	LED_Init();							//初始化LED	
	KEY_Init();							//初始化按键
 	LCD_Init();							//初始化LCD
	mem_init();							//初始化内存池
			    
 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,50,200,16,16,"Mini STM32");	
	LCD_ShowString(60,70,200,16,16,"MALLOC TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2019/11/15");  
	LCD_ShowString(60,130,200,16,16,"KEY0:Malloc");
	LCD_ShowString(60,150,200,16,16,"KEY1:Write Data");
	LCD_ShowString(60,170,200,16,16,"WK_UP:Free");

 	POINT_COLOR=BLUE;//设置字体为蓝色  
	LCD_ShowString(60,190,200,16,16,"SRAM USED:   %");  
  	while(1)
	{	
		key=KEY_Scan(0);//不支持连按	
		switch(key)
		{
			case 0:		//没有按键按下	
				break;
			case 1:		//KEY0按下
				p=mymalloc(2048);	//申请2K字节
				if(p!=NULL)sprintf((char*)p,"Memory Malloc Test%03d",i);//向p写入一些内容
				break;
			case 2:		//KEY1按下	   
				if(p!=NULL)
				{
					sprintf((char*)p,"Memory Malloc Test%03d",i);//更新显示内容 	 
					LCD_ShowString(60,250,200,16,16,p);			 //显示P的内容
				}
				break;
			case 3:		//WK_UP按下	  
				myfree(p);	//释放内存
				p=0;		//指向空地址
				break; 
		}
		if(tp!=p)
		{
			tp=p;
			sprintf((char*)paddr,"P Addr:0X%08X",(u32)tp);
			LCD_ShowString(60,230,200,16,16,paddr);	//显示p的地址
			if(p)LCD_ShowString(60,250,200,16,16,p);//显示P的内容
		    else LCD_Fill(60,250,239,266,WHITE);	//p=0,清除显示
		}
		delay_ms(10);   
		i++;
		if((i%20)==0)//DS0闪烁.
		{
			LCD_ShowNum(60+80,190,mem_perused(),3,16);//显示内存使用率
 			LED0=!LED0;
 		}
	}
}

