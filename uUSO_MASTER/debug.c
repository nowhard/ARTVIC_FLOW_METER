#include <ADuC845.h>


#include "timer3.h"
#include "proto_uso/proto_uso.h"
#include "preferences.h"
#include "eeprom/eeprom.h"
#include "timer1.h"
#include "adc.h"
#include "watchdog.h"
#include "proto_uso/channels.h"
#include "frequency.h"


#include "ulongsort.h"

#include "calibrate/calibrate.h"
#include "pt/pt.h"






extern struct pt pt_proto, pt_wdt;
volatile struct pt pt_sort;


//-----------------------------------------
void Set_Handlers(void);//���������� ����������� �������
PT_THREAD(I2C_RepeatRead(struct pt *pt));
 //---------------------------------------

void main(void) //using 0
{			   
	EA = 0;
	
	
	PLLCON&=PLLCON_VAL;//��������� ������� ����������
	
	RestoreCalibrate();
	
	ChannelsInit();//������������� �������� �������
	Protocol_Init();	
	Timer1_Initialize(); //������ �������� 200��	
	ADC_Initialize();
	UART_Init();

	WDT_Init(WDT_2000);//�������� ���������� ������
	
	PT_INIT(&pt_sort);


	EA=1;

	while(1)
	{	
		ProtoProcess(&pt_proto);
	
		ulongsort_process(&pt_sort);
		WDT_Process(&pt_wdt);	    
	}
}
//-----------------------------------------------------------------------------
void Timer1_Interrupt(void) interrupt 3  //������ ��������
{

	TH1	= TH1_VAL; ///200 Hz;
	TL1 = TL1_VAL;//
	pt_sort.pt_time++;
	pt_proto.pt_time++;
	pt_wdt.pt_time++;
	return;	
}