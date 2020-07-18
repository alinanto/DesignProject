#define F_CPU 8000000
#include <avr/io.h>
#include "keypad.h"
#include <util/delay.h>



#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
    /* on ATmega64/128 PINF is on port 0x00 and not 0x60 */
    #define PIN(x) ( &PORTF==&(x) ? _SFR_IO8(0x00) : (*(&x - 2)) )
#else
	#define PIN(x) (*(&x - 2))    /* address of input register of port x          */
#endif


#define SCAN_COLUMN(x) DDR(KEYPAD_PORT)=_BV(KEYPAD_PIN_C##x)   
#define CHECK_ROW_CLEAR(x)    bit_is_clear( PIN(KEYPAD_PORT) , KEYPAD_PIN_R##x)
#define RELAX_COLUMN() DDR(KEYPAD_PORT)=0x00 

struct DEBOUNCE
{
	int pressed;
	int confidence;
	char current_key;
}STRUCT_KEY={0,0,KEY_VALUE_NO_KEY};	 

void keypad_init()
{
	RELAX_COLUMN();
	KEYPAD_PORT=( _BV(KEYPAD_PIN_R0) | _BV(KEYPAD_PIN_R1) | _BV(KEYPAD_PIN_R2) | _BV(KEYPAD_PIN_R3 ) );
}

char keypad_scan()
{
	SCAN_COLUMN(0);
	_delay_us(KEYPAD_COLUMN_SCAN_DELAY); 

		if( CHECK_ROW_CLEAR(0) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C0_R0;
		}

	
		if( CHECK_ROW_CLEAR(1) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C0_R1;
		}
	
	
		if( CHECK_ROW_CLEAR(2) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C0_R2;
		}

	
		if( CHECK_ROW_CLEAR(3) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C0_R3;
		}


	SCAN_COLUMN(1);
	_delay_us(KEYPAD_COLUMN_SCAN_DELAY);

		if( CHECK_ROW_CLEAR(0) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C1_R0;
		}

	
		if( CHECK_ROW_CLEAR(1) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C1_R1;
		}

	
		if( CHECK_ROW_CLEAR(2) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C1_R2;
		}


		if( CHECK_ROW_CLEAR(3) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C1_R3;
		}

	SCAN_COLUMN(2);
	_delay_us(KEYPAD_COLUMN_SCAN_DELAY);

		if( CHECK_ROW_CLEAR(0) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C2_R0;
		}

	
		if( CHECK_ROW_CLEAR(1) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C2_R1;
		}

	
		if( CHECK_ROW_CLEAR(2) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C2_R2;
		}


		if( CHECK_ROW_CLEAR(3) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C2_R3;
		}

	SCAN_COLUMN(3);
	_delay_us(KEYPAD_COLUMN_SCAN_DELAY);

		if( CHECK_ROW_CLEAR(0) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C3_R0;
		}


		if( CHECK_ROW_CLEAR(1) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C3_R1;
		}

		
		if( CHECK_ROW_CLEAR(2) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C3_R2;
		}


		if( CHECK_ROW_CLEAR(3) )
		{
			RELAX_COLUMN();
			return KEY_VALUE_C3_R3;
		}

	RELAX_COLUMN();

	return KEY_VALUE_NO_KEY;// if no key is pressed
}

int keypad_key_pressed(char ch)
{
	switch (ch)
	{	
		case KEY_VALUE_C0_R0:{
								SCAN_COLUMN(0);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(0) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C0_R1:{
								SCAN_COLUMN(0);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(1) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C0_R2:{
								SCAN_COLUMN(0);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(2) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C0_R3:{
								SCAN_COLUMN(0);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(3) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C1_R0:{
								SCAN_COLUMN(1);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(0) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C1_R1:{
								SCAN_COLUMN(1);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(1) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C1_R2:{
								SCAN_COLUMN(1);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(2) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C1_R3:{
								SCAN_COLUMN(1);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(3) )
								{
									RELAX_COLUMN();
									return KEY_VALUE_C0_R0;
								}
								break;
							 }
		case KEY_VALUE_C2_R0:{
								SCAN_COLUMN(2);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(0) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C2_R1:{
								SCAN_COLUMN(2);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(1) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C2_R2:{
								SCAN_COLUMN(2);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(2) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C2_R3:{
								SCAN_COLUMN(2);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(3) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C3_R0:{
								SCAN_COLUMN(3);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(0) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C3_R1:{
								SCAN_COLUMN(3);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(1) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C3_R2:{
								SCAN_COLUMN(3);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(2) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
		case KEY_VALUE_C3_R3:{
								SCAN_COLUMN(3);
								_delay_us(KEYPAD_COLUMN_SCAN_DELAY);
								if( CHECK_ROW_CLEAR(3) )
								{
									RELAX_COLUMN();
									return 1;
								}
								break;
							 }
	}
	return 0;
}

void keypad_wait_till_key_pressed(char ch)
{
	while(!keypad_key_pressed(ch))
	{
	}
}

char keypad_loop()
{
 char buffer;
 buffer=keypad_scan();
 if(buffer==STRUCT_KEY.current_key)
 {
 	if(buffer==KEY_VALUE_NO_KEY)	return KEY_VALUE_NO_KEY;
	else if(!STRUCT_KEY.pressed)
	{
		STRUCT_KEY.confidence++;
 		if(STRUCT_KEY.confidence>=KEYPAD_DEBOUNCE_CYCLES)
		{
			STRUCT_KEY.pressed=1;
			return STRUCT_KEY.current_key;
		}	
	}
	else STRUCT_KEY.confidence=KEYPAD_DEBOUNCE_CYCLES;
 }
 else 
 {
	if(STRUCT_KEY.pressed)
	{
		STRUCT_KEY.confidence--;
 		if(STRUCT_KEY.confidence<=0)
		{
			STRUCT_KEY.pressed=0;
			STRUCT_KEY.current_key=buffer;
			return KEY_VALUE_BUTTON_RELEASE;
		}	
	}
	else 
	{
		STRUCT_KEY.confidence=0;
		STRUCT_KEY.current_key=buffer;
	}

 }
 return KEY_VALUE_NO_KEY;
}

char keypad_convert_to_char(char ch)
{
	switch(ch)
	{
		case KEY_VALUE_C0_R0 :return KEY_CHAR_C0_R0;break;
		case KEY_VALUE_C0_R1 :return KEY_CHAR_C0_R1;break;
		case KEY_VALUE_C0_R2 :return KEY_CHAR_C0_R2;break;
		case KEY_VALUE_C0_R3 :return KEY_CHAR_C0_R3;break;
		case KEY_VALUE_C1_R0 :return KEY_CHAR_C1_R0;break;
		case KEY_VALUE_C1_R1 :return KEY_CHAR_C1_R1;break;
		case KEY_VALUE_C1_R2 :return KEY_CHAR_C1_R2;break;
		case KEY_VALUE_C1_R3 :return KEY_CHAR_C1_R3;break;
		case KEY_VALUE_C2_R0 :return KEY_CHAR_C2_R0;break;
		case KEY_VALUE_C2_R1 :return KEY_CHAR_C2_R1;break;
		case KEY_VALUE_C2_R2 :return KEY_CHAR_C2_R2;break;
		case KEY_VALUE_C2_R3 :return KEY_CHAR_C2_R3;break;
		case KEY_VALUE_C3_R0 :return KEY_CHAR_C3_R0;break;
		case KEY_VALUE_C3_R1 :return KEY_CHAR_C3_R1;break;
		case KEY_VALUE_C3_R2 :return KEY_CHAR_C3_R2;break;
		case KEY_VALUE_C3_R3 :return KEY_CHAR_C3_R3;break;
		default				 :return KEY_CHAR_DEFAULT;break;
	}
}






	
