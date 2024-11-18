#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "touch.h"
#include "lcd.h"
#include "includes.h"
/************************************************
 ALIENTEK Mini STM32F103开发板实验36
 UCOSII-信号量和邮箱-HAL库版本
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//START 任务
//设置任务优先级
#define START_TASK_PRIO			10  //开始任务的优先级为最低
//设置任务堆栈大小
#define START_STK_SIZE			128
//任务任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);

//触摸屏任务
//设置任务优先级
#define TOUCH_TASK_PRIO         7
//设置任务堆栈大小
#define TOUCH_STK_SIZE  	    128
//任务堆栈	
OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//任务函数
void touch_task(void *pdata);

//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO			6
//设置任务堆栈大小
#define LED0_STK_SIZE			128
//任务堆栈
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);

//LED01任务
//设置任务优先级
#define LED1_TASK_PRIO			5
//设置任务堆栈大小
#define LED1_STK_SIZE			128
//任务堆栈
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *pdata);

//主任务
//设置任务优先级
#define MAIN_TASK_PRIO          4
//设置任务堆栈大小
#define MAIN_STK_SIZE  	        128
//任务堆栈	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//任务函数
void main_task(void *pdata);

//按键扫描任务
//设置任务优先级
#define KEY_TASK_PRIO       	3
//设置任务堆栈大小
#define KEY_STK_SIZE  	        128
//创建任务堆栈空间	
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务函数接口
void key_task(void *pdata);

//////////////////////////////////////////////////////////////////////////////
OS_EVENT * msg_key;			//按键邮箱事件块指针
OS_EVENT * sem_led1;		//LED1信号量指针	  	  
//加载主界面   
void ucos_load_main_ui(void)
{
	LCD_Clear(WHITE);	//清屏
 	POINT_COLOR=RED;	//设置字体为红色 
	LCD_ShowString(30,10,200,16,16,"Mini STM32");	
	LCD_ShowString(30,30,200,16,16,"UCOSII TEST2");	
	LCD_ShowString(30,50,200,16,16,"ATOM@ALIENTEK");
   	LCD_ShowString(30,75,200,16,16,"KEY0:LED1 KEY_UP:ADJUST");	
   	LCD_ShowString(30,95,200,16,16,"KEY1:CLEAR"); 
	LCD_ShowString(80,210,200,16,16,"Touch Area");	
	LCD_DrawLine(0,120,lcddev.width,120);
	LCD_DrawLine(0,70,lcddev.width,70);
	LCD_DrawLine(150,0,150,70);
 	POINT_COLOR=BLUE;//设置字体为蓝色 
  	LCD_ShowString(160,30,200,16,16,"CPU:   %");	
   	LCD_ShowString(160,50,200,16,16,"SEM:000");	
}


int main(void)
{ 
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);              	//初始化USART
	LED_Init();							//初始化LED	
	KEY_Init();                    		//初始化按键
	LCD_Init();                     	//初始化LCD
    tp_dev.init();				    	//初始化触摸屏
    ucos_load_main_ui();		    	//加载主界面	 
	
	OSInit();                       	//UCOS初始化
    OSTaskCreateExt((void(*)(void*) )start_task,                //任务函数
                    (void*          )0,                         //传递给任务函数的参数
                    (OS_STK*        )&START_TASK_STK[START_STK_SIZE-1],//任务堆栈栈顶
                    (INT8U          )START_TASK_PRIO,           //任务优先级
                    (INT16U         )START_TASK_PRIO,           //任务ID，这里设置为和优先级一样
                    (OS_STK*        )&START_TASK_STK[0],        //任务堆栈栈底
                    (INT32U         )START_STK_SIZE,            //任务堆栈大小
                    (void*          )0,                         //用户补充的存储区
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);//任务选项,为了保险起见，所有任务都保存浮点寄存器的值
	OSStart(); //开始任务
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	msg_key=OSMboxCreate((void*)0);	//创建消息邮箱
	sem_led1=OSSemCreate(0);		//创建信号量	
	OSStatInit();  //开启统计任务
	OS_ENTER_CRITICAL();  //进入临界区(关闭中断)
    //触摸任务
    OSTaskCreateExt((void(*)(void*) )touch_task,                 
                    (void*          )0,
                    (OS_STK*        )&TOUCH_TASK_STK[TOUCH_STK_SIZE-1],
                    (INT8U          )TOUCH_TASK_PRIO,            
                    (INT16U         )TOUCH_TASK_PRIO,            
                    (OS_STK*        )&TOUCH_TASK_STK[0],         
                    (INT32U         )TOUCH_STK_SIZE,             
                    (void*          )0,                         
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);
	//LED0任务
    OSTaskCreateExt((void(*)(void*) )led0_task,                 
                    (void*          )0,
                    (OS_STK*        )&LED0_TASK_STK[LED0_STK_SIZE-1],
                    (INT8U          )LED0_TASK_PRIO,            
                    (INT16U         )LED0_TASK_PRIO,            
                    (OS_STK*        )&LED0_TASK_STK[0],         
                    (INT32U         )LED0_STK_SIZE,             
                    (void*          )0,                         
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);
	//LED1任务
    OSTaskCreateExt((void(*)(void*) )led1_task,                 
                    (void*          )0,
                    (OS_STK*        )&LED1_TASK_STK[LED1_STK_SIZE-1],
                    (INT8U          )LED1_TASK_PRIO,            
                    (INT16U         )LED1_TASK_PRIO,            
                    (OS_STK*        )&LED1_TASK_STK[0],         
                    (INT32U         )LED1_STK_SIZE,             
                    (void*          )0,                         
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);
    //主任务
    OSTaskCreateExt((void(*)(void*) )main_task,                 
                    (void*          )0,
                    (OS_STK*        )&MAIN_TASK_STK[MAIN_STK_SIZE-1],
                    (INT8U          )MAIN_TASK_PRIO,          
                    (INT16U         )MAIN_TASK_PRIO,            
                    (OS_STK*        )&MAIN_TASK_STK[0],         
                    (INT32U         )MAIN_STK_SIZE,            
                    (void*          )0,                           
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);
    //按键任务
    OSTaskCreateExt((void(*)(void*) )key_task,                 
                    (void*          )0,
                    (OS_STK*        )&KEY_TASK_STK[KEY_STK_SIZE-1],
                    (INT8U          )KEY_TASK_PRIO,          
                    (INT16U         )KEY_TASK_PRIO,            
                    (OS_STK*        )&KEY_TASK_STK[0],         
                    (INT32U         )KEY_STK_SIZE,            
                    (void*          )0,                           
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);
    OS_EXIT_CRITICAL();             //退出临界区(开中断)
	OSTaskSuspend(START_TASK_PRIO); //挂起开始任务
}
 
//LED任务
void led0_task(void *pdata)
{
	u8 t;
	while(1)
	{
		t++;
		delay_ms(10);
		if(t==8)LED0=1;	//LED0灭
		if(t==100)		//LED0亮
		{
			t=0;
			LED0=0;
		}
	}									 
}	   

//LED1任务
void led1_task(void *pdata)
{
	u8 err;
	while(1)
	{
		OSSemPend(sem_led1,0,&err);
		LED1=0;
		delay_ms(200);
		LED1=1;
		delay_ms(800);
	}									 
}
//触摸屏任务
void touch_task(void *pdata)
{	  	
	while(1)
	{
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)		//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height&&tp_dev.y[0]>120)
			{													   
				TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图	 
				delay_ms(2); 			   
			}
		}else delay_ms(10);	//没有按键按下的时候 
	}
}
//主任务
void main_task(void *pdata)
{							 
	u32 key=0;	
	u8 err;	
	u8 semmask=0;
	u8 tcnt=0;						 
	while(1)
	{
		key=(u32)OSMboxPend(msg_key,10,&err);   
		switch(key)
		{ 
			case KEY0_PRES://发送信号量
				semmask=1;
				OSSemPost(sem_led1);
				break;
			case KEY1_PRES://清除
				LCD_Fill(0,121,lcddev.width,lcddev.height,WHITE);
				break;
			case WKUP_PRES://校准
				OSTaskSuspend(TOUCH_TASK_PRIO);	//挂起触摸屏任务		 
 				if((tp_dev.touchtype&0X80)==0)TP_Adjust();	   
 				OSTaskResume(TOUCH_TASK_PRIO);	//解挂
				ucos_load_main_ui();			//重新加载主界面		 
				break;
		}
   		if(semmask||sem_led1->OSEventCnt)//需要显示sem		
		{
			POINT_COLOR=BLUE;
			LCD_ShowxNum(192,50,sem_led1->OSEventCnt,3,16,0X80);//显示信号量的值
			if(sem_led1->OSEventCnt==0)semmask=0;	//停止更新
		} 
		if(tcnt==50)//0.5秒更新一次CPU使用率
		{
			tcnt=0;
			POINT_COLOR=BLUE;		  
			LCD_ShowxNum(192,30,OSCPUUsage,3,16,0);	//显示CPU使用率   
		}
		tcnt++;
		delay_ms(10);
	}
}

//按键扫描任务
void key_task(void *pdata)
{	
	u8 key;		    						 
	while(1)
	{
		key=KEY_Scan(0);   
		if(key)OSMboxPost(msg_key,(void*)key);//发送消息
 		delay_ms(10);
	}
}
