实验器材:
	MiniSTM32F103开发板V3版本
	
实验目的:
	学习辅助调试工具:USMART的使用
	
硬件资源:
	1,DS0(连接在PA8),DS1(连接在PD2)
	2,串口1(波特率:9600,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过GPIO驱动,连接关系见lcd.h) 
	
实验现象:
	本实验通过usmart调用单片机里面的函数，实现对LCD显示和LED以及延时的控制。
	
注意事项:  
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源供电(5V电源，接VOUT2即可).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,LCD模块在开发板的LCD接口上面，必须靠右插
	3,串口波特率为9600
	4,请用USB线连接在USB_232,找到USB转串口后测试本例程.
	5,PA9/PA10必须通过跳线帽连接在RXD/TXD上.
	 
	
	
					正点原子@ALIENTEK
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com