#include "button_input.h"

#include <avr/io.h>

#define BUTTON_BOUNCE 100

int BUTTON_1 = 0;
int BUTTON_2 = 0;

int BUTTON_1_COUNTER = 0;
int BUTTON_2_COUNTER = 0;

void read_button(void) {
	
	if( !(PINC & ( 1 << PINC0 ) ) ) {
		if( BUTTON_1_COUNTER < BUTTON_BOUNCE ) {
			BUTTON_1_COUNTER++;
		}
		else {
			if( BUTTON_1 == 1 ) {
				BUTTON_1 = 2;
			}
			else {
				BUTTON_1 = 1;
			}
			// BUTTON_1 = 1;
		}
	}
	else {
		// BUTTON_1 = 0;
		BUTTON_1_COUNTER = 0;
	}
	
	if( !(PINC & ( 1 << PINC1 ) ) ) {
		if( BUTTON_2_COUNTER < BUTTON_BOUNCE ) {
			BUTTON_2_COUNTER++;
		}
		else {
			BUTTON_2 = 1;
		}
	}
	else {
		BUTTON_2 = 0;
		BUTTON_2_COUNTER = 0;
	}
	
}
