#include "master.h"
#include "../protocol.h"
#include "../pinball.h"
#include <avr/io.h>
#include <avr/interrupt.h>

char node_id = MASTER_NODE;
char ReceivedMessage[500];
char *rm = ReceivedMessage;

int main(void) {
	USARTSetup();
	TIMERSetup();

	for(;;) { // ever
		// protocol_send_message( MASTER_NODE, SLAVE1_NODE, "Hi!  This is a cool message." );
	}
}

ISR(USART_RX_vect) {
	// *rm++ = UDR0;
	// if( *rm == 'e' )
	char message[10];
	message[0] = UDR0;
	protocol_send_data( message );
}
