#include "led_output.h"

#include <avr/io.h>

uint8_t LED_row; 
uint8_t LED_PortB, LED_PortD; // the bytes that will be sent to the Port Registers, PORTB and PORTD
uint16_t static LEDs_On_Data;        // the LSB bits (bits 9-0) will show which LEDs will be on (LEDs 10-1)

void led_setup(void) {
	               // clock fuse bits set PB7 and PB6 to be XTAL2 and XTAL1
                   // the programmer uses PB5, PB6 and PB7 as SCK, MISO and MOSI
DDRB = 0b00000001; // XTAL2, XTAL1, SCK, MISO, MOSI, INPUT, INPUT, OUTPUT
                   // PC7 is not a pin, the programmer uses PC6 as RESET
DDRC = 0b00000000; // N/A, RESET, INPUT, INPUT, INPUT, INPUT, INPUT, INPUT
                   // turning on RXEN0 and TXEN0 in the UCR0B register makes PD1 TXD and PD0 RXD
DDRD = 0b11111100; // OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, TRANSMIT, RECEIVE

PORTB = 0;
PORTD = 0;
LED_PortB = 0;
LED_PortD = 0;

LEDs_On_Data = 0; // set to zero for counting test

	
	// PORTB = 0b11111111;
	// PORTD = 0b11100011;
}

void led_illuminate( int LEDs_On_Data ) {
	
			LEDs_On_Data = LEDs_On_Data >> 3;
			
		   for ( LED_row = 0 ; LED_row < 3 ; LED_row++ ){ // ? c question will this be run through this with 0, 1 and 2? not 3? I hope.
	   
		   LED_PortD = LED_PortD | 0b00011100; // turn all rows off, set PD4, PD3 and PD2 to 1, bitwise or
		   
		                                                   // row 1 left shift 2, row 2 left shift 3, row 3 left shift 4
		   LED_PortD = LED_PortD & ~(1 << (2 + LED_row) ); // set Port pin for the current row to 0, bitwise and (with bitwise negation ~)
		   
		   for( int i = 0; i < 3; i++) {
		     int z = 5 + i;
		
		   if ( (LEDs_On_Data & (1 << ( (LED_row * 4) + i ) ) ) > 0){
               LED_PortD = LED_PortD | ( 1 << z); // set column 1 bit to 1 (PORTD left shift 5 for PD5), if it is 1 in LEDs_On_Data
	       } else {
               LED_PortD = LED_PortD & ~( 1 << z); // set column 1 bit to 0 (PORTD left shift 5 for PD5), if it is 0 in LEDs_On_Data
		   }
		   }
		   
		   if ( (LEDs_On_Data & (1 << ( (LED_row * 4) + 3 ) ) ) > 0){
			LED_PortB = LED_PortB | ( 1 << 0); // set column 4 bit to 1 (PORTB left shift 0 for PB0), if it is 1 in LEDs_On_Data
	       } else {
		     LED_PortB = LED_PortB & ~( 1 << 0); // set column 4 bit to 0 (PORTB left shift 0 for PB0), if it is 0 in LEDs_On_Data
		   }

	       PORTB = LED_PortB;
           PORTD = LED_PortD;
           //		delay_ms(10);
	}
}
