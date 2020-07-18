#define F_CPU 8000000
#define UART_BAUD_RATE      9600
#define PF_LIMIT  0.700
#define C_LIMIT   0.7 
#define U_LIMIT   0.003
#define TARRIF1   3.25
#define TARRIF2	  3.5

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <avr/eeprom.h>

#include "lcd.h"
#include "keypad.h"
#include "uart.h"

int var=0;
int sec=0;
int min=0;

int pf_flag=1;
int ma_flag=1;
int ta_flag=1;

int voltage_adc=0;
int voltage=0;
int current_adc=0;
float current=0;
int vflag=0;

float power=0;
float unit=0;
float bill=0;

int v_timer=0;
float v_frequency;
int i_timer;
float pf=0;
int ZCS_flag=0;


ISR(INT0_vect)
{
	if(ZCS_flag==0)i_timer=0;
	ZCS_flag=0;
	v_timer=TCNT1;
	TCNT1=0x0000; //reset timer
	
}	

ISR(INT1_vect)
{
	ZCS_flag=1;	
	i_timer=TCNT1;
}

ISR(ADC_vect)
{
	if(vflag)
    {
		voltage_adc=ADC;
 		ADMUX |=0b00000001;//set ADC 1 for current
		vflag=0; 		   //clear voltage adc flag
		ADCSRA|=0b01000000;//start adc current conversion
	}
	else
	{	
		current_adc=ADC;
		ADMUX&=0b11100000; //ADC 0 for voltage
	}
		
}

ISR(TIMER0_OVF_vect,ISR_NOBLOCK)
{
	var++;
	if(var>30)
	{
		var=0;
		sec++;
		if(sec>59)
		{
			sec=0;
			min++;
		}

		vflag=1;			//set voltage adc  flag
		ADCSRA|=0b01000000; //start adc voltage conversion
        
		v_frequency=(float)(1000000.00/v_timer);
		voltage=(int)((float)voltage_adc*0.281);
		current=current_adc*0.00243;
		
		pf=cos( (float)6.28315 * (  (float)i_timer/(float)v_timer  ) );
		
		power=voltage*current*pf;
		unit+=(power/3600000.00);
		bill=unit*TARRIF1;

		switch(sec%12)
		{		
		case 0:
		case 1:
		case 2:{
				lcd_clrscr();
				lcd_puts("Voltage: ");
				lcd_putc(keypad_convert_to_char(digit(voltage,3)));
				lcd_putc(keypad_convert_to_char(digit(voltage,2)));
				lcd_putc(keypad_convert_to_char(digit(voltage,1)));	
				lcd_gotoxy(0,1);
				lcd_puts("Current: ");
				lcd_putc(keypad_convert_to_char(digit(current,1)));	
				lcd_putc('.');
				lcd_putc(keypad_convert_to_char(digit(current,-1)));
				lcd_putc(keypad_convert_to_char(digit(current,-2)));
				lcd_putc(keypad_convert_to_char(digit(current,-3)));	
				break;
			   }
		case 3:
		case 4:
		case 5:{
				lcd_clrscr();
				lcd_puts("PF: ");
			   	lcd_putc(keypad_convert_to_char(digit(pf,1)));	
				lcd_putc('.');
				lcd_putc(keypad_convert_to_char(digit(pf,-1)));
				lcd_putc(keypad_convert_to_char(digit(pf,-2)));
				lcd_putc(keypad_convert_to_char(digit(pf,-3)));	
				lcd_gotoxy(0,1);
				lcd_puts("FREQ: ");
				lcd_putc(keypad_convert_to_char(digit(v_frequency,2)));
				lcd_putc(keypad_convert_to_char(digit(v_frequency,1)));	
				lcd_putc('.');
				lcd_putc(keypad_convert_to_char(digit(v_frequency,-1)));
				lcd_putc(keypad_convert_to_char(digit(v_frequency,-2)));
				lcd_putc(keypad_convert_to_char(digit(v_frequency,-3)));	
				break;
			   }
		case 6:
		case 7:
		case 8:{
				lcd_clrscr();
				lcd_puts("POWER: ");
				lcd_putc(keypad_convert_to_char(digit(power,3)));
				lcd_putc(keypad_convert_to_char(digit(power,2)));
				lcd_putc(keypad_convert_to_char(digit(power,1)));	
				lcd_putc('.');
				lcd_putc(keypad_convert_to_char(digit(power,-1)));
				lcd_putc(keypad_convert_to_char(digit(power,-2)));
				lcd_putc(keypad_convert_to_char(digit(power,-3)));	
				lcd_gotoxy(0,1);
				lcd_puts("Units: ");
				lcd_putc(keypad_convert_to_char(digit(unit,3)));
				lcd_putc(keypad_convert_to_char(digit(unit,2)));
				lcd_putc(keypad_convert_to_char(digit(unit,1)));	
				lcd_putc('.');
				lcd_putc(keypad_convert_to_char(digit(unit,-1)));
				lcd_putc(keypad_convert_to_char(digit(unit,-2)));
				lcd_putc(keypad_convert_to_char(digit(unit,-3)));	
				break;
			   }
		case  9:
		case 10:
		case 11:
				{
				lcd_clrscr();
				lcd_puts("BILL : ");
				lcd_putc(keypad_convert_to_char(digit(bill,3)));
				lcd_putc(keypad_convert_to_char(digit(bill,2)));
				lcd_putc(keypad_convert_to_char(digit(bill,1)));	
				lcd_putc('.');
				lcd_putc(keypad_convert_to_char(digit(bill,-1)));
				lcd_putc(keypad_convert_to_char(digit(bill,-2)));
				lcd_putc(keypad_convert_to_char(digit(bill,-3)));
				lcd_gotoxy(0,1);
				lcd_puts("Tarrif:");
				lcd_putc(keypad_convert_to_char(digit(TARRIF1,1)));	
				lcd_putc('.');
				lcd_putc(keypad_convert_to_char(digit(TARRIF1,-1)));
				lcd_putc(keypad_convert_to_char(digit(TARRIF1,-2)));
				lcd_puts("/Kwh");
				break;
				}
		}	

		if(fabs(pf)<PF_LIMIT&&pf_flag){	uart_puts("\n\rWARNING: Low power factor!\n\r");				pf_flag=0;	}
		else if	(fabs(pf)>PF_LIMIT)																		pf_flag=1;   

		if(current>C_LIMIT&&ma_flag)  {	uart_puts("\n\rWARNING: Max Connected load exceeded!\n\r");		ma_flag=0;	}
		else if	(current<C_LIMIT)																		ma_flag=1;   
		
		if(unit>U_LIMIT&&ta_flag)	  {	uart_puts("\n\rWARNING: Subsidy usage limit exceeded!\n\r");	ta_flag=0;	}
		else if	(unit<U_LIMIT)																			ta_flag=1;   

		

	}		
}


int main(void)
{	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei();
	lcd_init(LCD_DISP_ON);
	keypad_init();
    PORTC|=0b00000001;   
    lcd_clrscr();
	lcd_puts("LOADING....\nv1.8");
	_delay_ms(1000);
	uart_puts("\n\rDevice Online\n\r");

	TCCR1A=0b00000000;
	TCCR1B=0b00000010; //8 prescaler timer 1
	TCCR0 =0b00000101; //1024 prescaler timer 0 
	TIMSK|=0b00000001; //interuppt on overflow Timer 0

	ADMUX =0b01000000; //ADC 0 for voltage
	ADCSRA=0b10001111; //INTERUPPT ENABLE-128_prescaler

	MCUCR|=0b00001111; //INT0,INT1 on rising edge
	GICR |=0b11000000; //INT0,INT1 enable
	
	char ch;
	unsigned int c;
	while(1)
	{

		c = uart_getc();
        if ( c & UART_NO_DATA )
        {
            
        }
        else
        {
            if ( c & UART_FRAME_ERROR )
            uart_puts_P("UART Frame Error:\n\r ");
            if ( c & UART_OVERRUN_ERROR )
            uart_puts_P("UART Overrun Error:\n\r ");
            if ( c & UART_BUFFER_OVERFLOW )
            uart_puts_P("Buffer overflow error:\n\r ");

			ch= ( 0x00FF&c);

			if(ch=='f'||ch=='F')	{	unit=0;	uart_puts("\n\rMemory flushed\n\r");	}
			if(ch=='c'||ch=='C')		uart_puts("\n\rDevice Online\n\r");
			if(ch=='r'||ch=='R')
			{
				uart_puts_P("\n\rPOWER: ");
				uart_putc(keypad_convert_to_char(digit(power,3)));
				uart_putc(keypad_convert_to_char(digit(power,2)));
				uart_putc(keypad_convert_to_char(digit(power,1)));	
				uart_putc('.');
				uart_putc(keypad_convert_to_char(digit(power,-1)));
				uart_putc(keypad_convert_to_char(digit(power,-2)));
				uart_putc(keypad_convert_to_char(digit(power,-3)));	

				uart_puts_P("\n\rUNITS: ");
				uart_putc(keypad_convert_to_char(digit(unit,3)));
				uart_putc(keypad_convert_to_char(digit(unit,2)));
				uart_putc(keypad_convert_to_char(digit(unit,1)));	
				uart_putc('.');
				uart_putc(keypad_convert_to_char(digit(unit,-1)));
				uart_putc(keypad_convert_to_char(digit(unit,-2)));
				uart_putc(keypad_convert_to_char(digit(unit,-3)));

				uart_puts_P("\n\rVOLTAGE: ");
				uart_putc(keypad_convert_to_char(digit(voltage,3)));
				uart_putc(keypad_convert_to_char(digit(voltage,2)));
				uart_putc(keypad_convert_to_char(digit(voltage,1)));

				uart_puts_P("\n\rCURRENT: ");
				uart_putc(keypad_convert_to_char(digit(current,1)));	
				uart_putc('.');
				uart_putc(keypad_convert_to_char(digit(current,-1)));
				uart_putc(keypad_convert_to_char(digit(current,-2)));
				uart_putc(keypad_convert_to_char(digit(current,-3)));

				uart_puts_P("\n\rFREQUENCY: ");
				uart_putc(keypad_convert_to_char(digit(v_frequency,2)));
				uart_putc(keypad_convert_to_char(digit(v_frequency,1)));	
				uart_putc('.');
				uart_putc(keypad_convert_to_char(digit(v_frequency,-1)));
				uart_putc(keypad_convert_to_char(digit(v_frequency,-2)));
				uart_putc(keypad_convert_to_char(digit(v_frequency,-3)));
				
				uart_puts_P("\n\rPOWER FACTOR: ");
				uart_putc(keypad_convert_to_char(digit(pf,1)));	
				uart_putc('.');
				uart_putc(keypad_convert_to_char(digit(pf,-1)));
				uart_putc(keypad_convert_to_char(digit(pf,-2)));
				uart_putc(keypad_convert_to_char(digit(pf,-3)));

				uart_puts_P("\n\rBILL AMOUNT: ");
				uart_putc(keypad_convert_to_char(digit(bill,3)));
				uart_putc(keypad_convert_to_char(digit(bill,2)));
				uart_putc(keypad_convert_to_char(digit(bill,1)));	
				uart_putc('.');
				uart_putc(keypad_convert_to_char(digit(bill,-1)));
				uart_putc(keypad_convert_to_char(digit(bill,-2)));
				uart_putc(keypad_convert_to_char(digit(bill,-3)));
				uart_puts_P("\n\rTarrif: ");
				uart_putc(keypad_convert_to_char(digit(TARRIF1,1)));	
				uart_putc('.');
				uart_putc(keypad_convert_to_char(digit(TARRIF1,-1)));
				uart_putc(keypad_convert_to_char(digit(TARRIF1,-2)));
				uart_putc(keypad_convert_to_char(digit(TARRIF1,-3)));
				uart_puts_P("/UNIT");
        	}
			ch=0;
		}
	}
}
