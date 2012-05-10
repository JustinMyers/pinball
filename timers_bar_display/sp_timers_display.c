#include <avr/io.h>
#include <avr/interrupt.h>
#include "sp_timers.h"
#include "sp_bar_display.h"

uint8_t SvBDRow;				// which row to display at this time
uint16_t SvBDValue;			// the LSB bits (bits 9-0) will show which LEDs will be on (LEDs 10-1)
uint16_t SvTempSecs, SvPatternSecs;
uint8_t SvPattern, SvPatternCount, SvTempHuns, SvTempCount, SvTempDirection, SvTempCount2;

int main (void) {
	
	SvBDValue = 0;  // Bar Display Value - bits 9-0 are what shows on the LED bar display
	SvBDRow = 0;  // Bar Display Row - which row is lit
	
	SvPattern = 0;  // main while loop, which display pattern is running
	SvPatternCount = 0;  // main while loop, counter for how many seconds to keep with a pattern
	SvPatternSecs = SvSeconds;  // compare value, if the RTC SvSeconds changes, increment SvPatternCount
	SvTempSecs = SvSeconds;  // compare value for pattern 0, if the RTC SvSeconds changes, do pattern 0 thing
	SvTempHuns = SvHundredths;  // compare value for pattern 1, if the RTC SvHudredths changes, increment SvTempCount
	SvTempCount = 0;  // pattern 1 counter for how many hundredths until next step
	SvTempDirection = 0;  // pattern 1, indicates which direction the pattern is counting, 0 = count up, 1 = dount down
	SvTempCount2 = 0;  // pattern 1, which LED is lit (moves up and down on step, each time temp count is reset)
	
	sei();  // turn on global interrupts
	Start_Timer2_RTC ();  // start RTC (Real Time Clock)
	Sf_Port_IO_init ();  // setup IO pins for the bar LED and switches circuit
	
	while (1) {
		
		//SvBDValue = 0x3FFU;  // test all LEDs on
		
		if (SvPatternSecs != SvSeconds) {
			SvPatternSecs = SvSeconds;
			SvPatternCount++;  // basically another independent Seconds counter, resets when next SvPattern is updated
		}
		if (SvPatternCount > 9) {  // change patttern every 10 seconds
			SvPatternCount = 0;
			SvPattern++;
			SvBDValue = 0;  // reset display to 0 before startin a new pattern
			if (SvPattern > 1) {  // edit this for changing the total number of patterns used
				SvPattern = 0;
			}
		}
			
		if (SvPattern == 0) {  // shows the lower 10 bits of the RTC SvSeconds counter
			
			if (SvTempSecs != SvSeconds) {
				SvTempSecs = SvSeconds;
				SvBDValue = (SvSeconds & 0x3FFU);  // no, that's not what it means...  the 'U' means a 16 bit Unsigned integer
				//if (SvBDValue > 0x3FFU) {  
					//SvBDValue = 0;
				//}
			}
		}
		
		if (SvPattern == 1) {  // scans an LED up and down the bar display
			
			if (SvTempHuns != SvHundredths) {
				SvTempHuns = SvHundredths;
				SvTempCount++;
			}
			if (SvTempCount > 9) {
				SvTempCount = 0;
				if (SvTempDirection == 0) {
					SvTempCount2++;
					if (SvTempCount2 > 9) {
						SvTempCount2 = 8;
						SvTempDirection = 1;
					}
				}
				else {
					SvTempCount2--;
					if (SvTempCount2 == 0xFF) {
						SvTempCount2 = 1;
						SvTempDirection = 0;
					}
				}
				SvBDValue = 0;
				SvBDValue |= (1 << SvTempCount2);  // set LED on
			}
		}
		
		if (SvDisplayTime > 25) {  // SvDisplayTime varibale is added in the Start_Timer2_RTC function, increments every ten-thousandth of a second
			SvDisplayTime -= 25;  // go to the bar display function every 25 ten-thousandths of a second (400 per second)
			if (SvBDRow > 2) {
				SvBDRow = 0;
			}
			Sf_Bar_Display (SvBDValue, SvBDRow);
			SvBDRow++;  // next time, do the next row
		}
	}
}

