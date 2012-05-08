#include "slave1.h"
#include "../protocol.h"
#include "../pinball.h"
#include "../led_output.h"
#include "../button_input.h"

#include <avr/io.h>

char node_id = SLAVE1_NODE;

extern int BUTTON_1;
extern int BUTTON_2;

void animate();

int main(void) {
	USARTSetup();
	TIMERSetup();
	
	led_setup();
	
	int direction = 1;
	long unsigned int standard = 0b11;
	
	long unsigned int num = standard;
	
	//uint16_t direction = 1;
	//uint16_t standard = 0b11;
	
	//uint16_t num = standard;
	
	for(;;) { // ever
		// protocol_send_message( node_id, MASTER_NODE, "Hello from SLAVE1!" );
		// delay_ms(2000); // two second delay

		//if( direction == 1 ){
			//num = num << 1;
		//}
		//if( direction == 0 ){
			//num = num >> 1;
		//}
		
		//if( num > 0b0111111111111111 ) {
			//direction = 0;
		//}
		//if( num < 0b0000000000000100 ) {
			//direction = 1;
		//}
		
		num = 0b0;
		
		read_button();
		
		if( BUTTON_1 == 1 ) {
			num = num | 0b1111111100000000;
		}
		else {
			// num = num & 0b0000000000110000;
		}
		
		if( BUTTON_2 == 1 ) {
			num = num | 0b0000000011111111;
		}
		else {
			// num = num & 0b0000001100000000;
		}
		
		led_illuminate( num );
		
		// delay_ms(500);
		
	}
}
