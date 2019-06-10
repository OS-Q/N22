
#include <string.h>
#include <intrins.h> 
#include <stdio.h>

#include "N76E003.h"
#include "WS281X.h"
#include "SFR_Macro.h"
#include "define.h"
#include "I2C.h"
#include "pwm.h"

#define SYS_CLK_EN              0
#define SYS_SEL                 2
#define SYS_DIV_EN              0                   //0: Fsys=Fosc, 1: Fsys = Fosc/(2*CKDIV)
#define SYS_DIV                 1



#define TH0_INIT        1300 
#define TL0_INIT        1300	//��ʱ��


U8 u8TH0_Tmp,u8TL0_Tmp;
U16 Timflag;
U16 zhuflag;
U8 systatus;
U8 led_flag;
U8 num_2 = 0;

/*
U8 xdata led_eff[88]={0x00,0x00,0xf1,0x00,0x00,0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,0xf1,0xff,
0x00 ,0x00,0x00 ,0x00,0x00,0xf1, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,0xf1, 0x00, 0x00, 0x00, 0xff,
0x00 ,0x00 ,0x00, 0x00, 0x00 ,0x00 ,0x00,0x00,0xf1, 0x00 ,0x00 ,0x00 ,0x00,0x00,0xf1, 0x00, 0x00, 0x00, 0x00 ,0x00, 0x00, 0xff,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00, 0x00,0x00 ,0x00, 0x00,0x00,0xf1 ,0x00 ,0x00 ,0x00 ,0x00, 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xff};

	U8 xdata led_eff[88]={0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xff,
0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xff,
0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xff,
0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xf1,0xff,};*/

void Timer0_init(void)
{
	TIMER0_MODE1_ENABLE;	
	u8TH0_Tmp = (65536-TH0_INIT)/256;
    u8TL0_Tmp = (65536-TL0_INIT)%256; 
	TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;	
	set_ET0;                                  
    set_EA;  
	set_TR0;
	
}

void Timer0_ISR (void) interrupt 1 
{
    TH0 = u8TH0_Tmp;
    TL0 = u8TL0_Tmp;    
		Timflag++;
		zhuflag++;
}


void main(void)
{	
	//U8 led_i = 0;	
	Set_All_GPIO_Quasi_Mode;
	Timer0_init();
	pwm_init();
	Init_I2C();
	P00_PushPull_Mode;

	set_PI2C;
	set_PI2CH;
	P00 = 1;
	
	InitialUART0_Timer3(115200);
	Send_Data_To_UART0(0x10);
	printf("\ntest printf %d ", 0x01);
	
	while(1)
	{			
		if(zhuflag>=4)
		{
				i2c_wait_task();	
				
				zhuflag = 0;
		}
		if(Timflag>=6)
		{
				
				i2c_send_task();
				Timflag = 0;
		}			
	}		
}


/************************ (C) COPYRIGHT Qitas **********************/


