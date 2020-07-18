

#define KEYPAD_PORT		PORTB
#define KEYPAD_PIN_R0	7
#define KEYPAD_PIN_R1	6	
#define KEYPAD_PIN_R2	5
#define KEYPAD_PIN_R3	4
#define KEYPAD_PIN_C0	3
#define KEYPAD_PIN_C1	2
#define KEYPAD_PIN_C2	1
#define KEYPAD_PIN_C3	0

#define KEY_VALUE_C0_R0 	 	 0x01     //these values must not conflict
#define KEY_VALUE_C1_R0 	 	 0x02
#define KEY_VALUE_C2_R0 	     0x03 
#define KEY_VALUE_C3_R0 	 	 0x0A
#define KEY_VALUE_C0_R1 	 	 0x04
#define KEY_VALUE_C1_R1 	 	 0x05
#define KEY_VALUE_C2_R1 	 	 0x06
#define KEY_VALUE_C3_R1 	 	 0x0B
#define KEY_VALUE_C0_R2 	 	 0x07
#define KEY_VALUE_C1_R2      	 0x08
#define KEY_VALUE_C2_R2 	 	 0x09
#define KEY_VALUE_C3_R2 	 	 0x0C
#define KEY_VALUE_C0_R3  	 	 0x0E
#define KEY_VALUE_C1_R3 	 	 0x00
#define KEY_VALUE_C2_R3  	 	 0x0F
#define KEY_VALUE_C3_R3  	  	 0x0D
#define KEY_VALUE_NO_KEY 	  	 0xFF	  //no key pressed or no event msg	
#define KEY_VALUE_BUTTON_RELEASE 0x11	  //button release msg

#define KEY_CHAR_C0_R0   '1'
#define KEY_CHAR_C1_R0   '2'
#define KEY_CHAR_C2_R0   '3' 
#define KEY_CHAR_C3_R0   'A'
#define KEY_CHAR_C0_R1   '4'
#define KEY_CHAR_C1_R1   '5'
#define KEY_CHAR_C2_R1   '6'
#define KEY_CHAR_C3_R1   'B'
#define KEY_CHAR_C0_R2   '7'
#define KEY_CHAR_C1_R2   '8'
#define KEY_CHAR_C2_R2   '9'
#define KEY_CHAR_C3_R2   'C'
#define KEY_CHAR_C0_R3   '*'
#define KEY_CHAR_C1_R3   '0'
#define KEY_CHAR_C2_R3   '#'
#define KEY_CHAR_C3_R3   'D'
#define KEY_CHAR_DEFAULT '$'

#define KEYPAD_DEBOUNCE_CYCLES     100
//// no of loop itteration 
//// for successful confirmation of event(button press or release)


#define KEYPAD_COLUMN_SCAN_DELAY  30 
//// delay in micro seconds for each column scan
//// mandatory for transcient adjustment

void keypad_init();
//// initialises the keypad port

char keypad_loop();	
//// debounced funcion that is used in fast loops
//// KEY_VALUE_C(x)_R(x)//////////returns key value msg for button press 
//// KEY_VALUE_NO_KEY/////////////returns no key msg for no event
//// KEY_VALUE_BUTTON_RELEASE/////returns button release msg for button release

char keypad_scan();		
//// not recommended to be used in fast loop(not debounced)
//// function assumes that button was pressed a long time before calling
//// returns the status of keypad a the moment of function call
//// KEY_VALUE_C(x)_R(x)//////////returns key value for currently pressed button 
//// KEY_VALUE_NO_KEY/////////////returns no key for null status

int  keypad_key_pressed(char);	
//// not recommended to be used in fast loop(not debounced)
//// returns 1 if the button is pressed at the moment fo funtion call
//// ARGUMENT: KEY_VALUE_C(x)_R(x)///key value for button

void keypad_wait_till_key_pressed(char);
//// function does not return until the key is pressed
//// ARGUMENT: KEY_VALUE_C(x)_R(x)///key value for button

char keypad_convert_to_char(char);
//// returns character value for corresponding button
//// ARGUMENT: KEY_VALUE_C(x)_R(x)///key value for button


