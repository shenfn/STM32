实验器材:
	MiniSTM32F103开发板V3版本
	
实验目的:
	学习STM32的USB功能使用,实现一个简单的USB读卡器
	
硬件资源:
	1,DS0(连接在PA8) 
	2,串口1(波特率:25600,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过GPIO驱动,连接关系见lcd.h)  
	4,SD卡,通过SPI1连接 
	5,USB接口(在开发板的侧面)
	
实验现象:
	本实验开机的时候先检测SD卡和SPI FLASH是否存在，如果存在则获取其容量，并显示在LCD上面
	（如果不存在，则报错）。之后开始USB配置，在配置成功之后就可以在电脑上发现两个可移动磁
	盘。我们用DS1来指示USB正在读写SD卡，并在液晶上显示出来，同样我们还是用DS0来指示程序正
	在运行。
	
注意事项:  
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源供电(5V电源，接VOUT2即可).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,LCD模块在开发板的LCD接口上面，必须靠右插 
	4,本例程改自ALIENTEK MiniSTM32 实验28 SD卡实验
	5,USB线，请务必插到开发板的USB端口（左侧的USB口）
	6,本实验需要自备SD卡一个！！(SD卡请用大卡，TF卡兼容性极不好！)
	 
	
	
					正点原子@ALIENTEK
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com