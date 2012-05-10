#include <avr/io.h>

uint8_t SvBDRow;				// which row to display at this time
uint8_t LED_PortB, LED_PortD;	// the bytes that will be sent to the Port Registers, PORTB and PORTD
uint16_t SvBDValue;			// the LSB bits (bits 9-0) will show which LEDs will be on (LEDs 10-1)

uint8_t LED_PortB = 0;
uint8_t LED_PortD = 0;
uint8_t SvBDRow = 0;
uint16_t SvBDValue = 0;

void Sf_Port_IO_init () {
						// clock fuse bits set PB7 and PB6 to be XTAL2 and XTAL1
						// the programmer uses PB5, PB6 and PB7 as SCK, MISO and MOSI
	DDRB = 0b00000001; 	// XTAL2, XTAL1, SCK, MISO, MOSI, INPUT, INPUT, OUTPUT
						// PC7 is not a pin, the programmer uses PC6 as RESET
	DDRC = 0b00000000; 	// N/A, RESET, INPUT, INPUT, INPUT, INPUT, INPUT, INPUT
						// turning on RXEN0 and TXEN0 in the UCR0B register makes PD1 TXD and PD0 RXD
	DDRD = 0b11111100; 	// OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, TRANSMIT, RECEIVE
	
	PORTB = 0;
	PORTD = 0;
}

// display value in SvBDValue on 10 segment LED bar display, show the 4 LEDs on the Row number in SvBDRow variable
void Sf_Bar_Display (uint16_t SvBDValue, uint8_t SvBDRow) {
	   
	LED_PortD |= 0b00011100; // turn all rows off, set PD4, PD3 and PD2 to 1, bitwise or
		   
													// row 1 left shift 2, row 2 left shift 3, row 3 left shift 4
	LED_PortD &= ~(1 << (2 + SvBDRow) ); // set Port pin for the current row to 0, bitwise and (with bitwise negation ~)
		   
	// copy out four bits for the columns in this current row, from SvBDValue to LED_PortD for the column data
	if ( (SvBDValue & (1 << ( (SvBDRow * 4) + 0 ) ) ) > 0){
		LED_PortD |= ( 1 << 5); // set column 1 bit to 1 (PORTD left shift 5 for PD5), if it is 1 in SvBDValue
	}
	else {
		LED_PortD &= ~( 1 << 5); // set column 1 bit to 0 (PORTD left shift 5 for PD5), if it is 0 in SvBDValue
	}
		   
	if ( (SvBDValue & (1 << ( (SvBDRow * 4) + 1 ) ) ) > 0){
		LED_PortD |= ( 1 << 6); // set column 2 bit to 1 (PORTD left shift 6 for PD6), if it is 1 in SvBDValue
	}
	else {
		LED_PortD &= ~( 1 << 6); // set column 2 bit to 0 (PORTD left shift 6 for PD6), if it is 0 in SvBDValue
	}
		   
	if ( (SvBDValue & (1 << ( (SvBDRow * 4) + 2 ) ) ) > 0){
		LED_PortD |= ( 1 << 7); // set column 3 bit to 1 (PORTD left shift 7 for PD7), if it is 1 in SvBDValue
	}
	else {
		LED_PortD &= ~( 1 << 7); // set column 3 bit to 0 (PORTD left shift 7 for PD7), if it is 0 in SvBDValue
	}
		   
	if ( (SvBDValue & (1 << ( (SvBDRow * 4) + 3 ) ) ) > 0){
		LED_PortB |= ( 1 << 0); // set column 4 bit to 1 (PORTB left shift 0 for PB0), if it is 1 in SvBDValue
	}
	else {
		LED_PortB &= ~( 1 << 0); // set column 4 bit to 0 (PORTB left shift 0 for PB0), if it is 0 in SvBDValue
	}
		   
	PORTB = LED_PortB;
	PORTD = LED_PortD;

}
