#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "keypad.h"


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



int main(void)
{	
	int count=0;
	char ch=0xFF;
	char buffer;

    lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	keypad_init();

   
    lcd_clrscr();

	while(1)
	{
		buffer=keypad_scan();
		if(ch!=buffer)
		{
			ch=buffer;
			count++;
			if(count>16)
			{
				lcd_clrscr();
				count=0;
			}
			lcd_putc(keypad_convert_to_char(ch));
		}		
	}
}
