#include "protocol.h"       

#include <stdio.h>          // string processing
#include <avr/io.h>
#include <avr/interrupt.h>

#define stringify(expr) "#expr"

void delay_ms(uint16_t x); //General purpose delay

void USARTSetup(void) {
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   // Turn on the transmission and reception circuitry 
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes 
	UBRR0H = (USART_PRESCALE >> 8);          // Load upper 8-bits of the baud rate value into the high byte of the UBRR register 
	UBRR0L = USART_PRESCALE;                 // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	
	// UCSR0B |= (1<<RXEN0)|(1<<RXCIE0);
	UCSR0B |= (1 << RXCIE0);                   // Enable the USART Receive Complete interrupt (USART_RXC)
	sei();                                   // Enable the Global Interrupt Enable flag
}

void TIMERSetup(void){
	TCCR2A |= (1 << WGM21);                  // CTC mode, timer 2, page 162
	OCR2A = 155;                             // set counter for about 1ms, 20MHz clock, 1/128 prescaler
		                                     // calculated value was 155.25, Newbie's Guide to AVR Timers formula page 8
	TCCR2B |= ((1 << CS22) | (1 << CS20));   // start timer with Fcpu/128, page 164
}

void protocol_send_message( int transmitter, int receiver, char message[] ) {
	// define the packet header
	char header[ 11 ];
	int header_len = sprintf( header, "st%#04dr%#04d", transmitter, receiver );
	
	// define the packet footer
	char footer[] = "e\n"; // NOTE THE NEWLINE, GET RID OF THIS IN THE FUTURE
	
	// start processing the message
	char packet[500];
	int len = sprintf(packet, "%s%s%s", header, message, footer);

	protocol_send_data( packet );
}

void protocol_send_data(char data[]) {
	for(int i; data[i] != '\0'; i++) {
		while ((UCSR0A & (1 << UDRE0)) == 0) {}; // wait until there is room for the character
		UDR0 = data[i];                          // put a character in the register
		delay_ms( 1 );                           // smallest communication delay 1000/sec.
	}
}

//General short delays
void delay_ms(uint16_t x)
{
	for ( ; x > 0 ; x--){
		GTCCR |= (1 << TSM); // reset timer counter, page 167
		// if (TIFR2 & (1 << OCF2A)) // temp, test for timer flag,  Newbie's Guide to AVR Timers, page 14
		while (!(TIFR2 & (1 << OCF2A))){} // while NOT timer flag test, page 165
		TIFR2 = (1 << OCF2A); // clear CTC flag, timer 2, page 165
	}
}

