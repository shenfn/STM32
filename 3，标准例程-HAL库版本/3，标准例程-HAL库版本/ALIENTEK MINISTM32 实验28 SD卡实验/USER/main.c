#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "malloc.h" 
#include "MMC_SD.h"  
/************************************************
 ALIENTEK Mini STM32F103������ʵ��28
 SD��ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//��ȡSD����ָ�����������ݣ���ͨ������1���
//sec�����������ַ���
void SD_Read_Sectorx(u32 sec)
{
	u8 *buf;
	u16 i;
	buf=mymalloc(512);				//�����ڴ�
	if(SD_ReadDisk(buf,sec,1)==0)	//��ȡ0����������
	{	
		LCD_ShowString(60,190,200,16,16,"USART1 Sending Data...");
		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)printf("%x ",buf[i]);//��ӡsec��������    	   
		printf("\r\nDATA ENDED\r\n");
		LCD_ShowString(60,190,200,16,16,"USART1 Send Data Over!");
	}
	myfree(buf);//�ͷ��ڴ�	
}

int main(void)
{ 
	u8 key;		 
	u32 sd_size;
	u8 t=0;
	
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������
	LED_Init();							//��ʼ��LED	
	KEY_Init();							//��ʼ������
 	LCD_Init();							//��ʼ��LCD
 	mem_init();							//��ʼ���ڴ��
	
 	POINT_COLOR=RED;					//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"Mini STM32");	
	LCD_ShowString(60,70,200,16,16,"SD CARD TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2019/11/16");  
	LCD_ShowString(60,130,200,16,16,"KEY0:Read Sector 0");	   
 	while(SD_Init())		//��ⲻ��SD��
	{
		LCD_ShowString(60,150,200,16,16,"SD Card Error!");
		delay_ms(500);					
		LCD_ShowString(60,150,200,16,16,"Please Check! ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	//���SD���ɹ� 											    
	LCD_ShowString(60,150,200,16,16,"SD Card OK    ");
	LCD_ShowString(60,170,200,16,16,"SD Card Size:     MB");
	sd_size=SD_GetSectorCount();//�õ�������
	LCD_ShowNum(164,170,sd_size>>11,5,16);//��ʾSD������
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)SD_Read_Sectorx(0);//KEY0��,��ȡSD������0������
		t++;
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;
			t=0;
		}
	} 
}

