#include <avr/io.h>
#include <avr/interrupt.h>
#include "sp_timers.h"

// variables for Start_Timer0_CTC function (8-bit Timer/Counter0 with PWM)
volatile uint8_t SvTimer0_CompA = 0;  // count value for Output Compare Register A (number of prescaler units to count, then back to 0)
volatile uint8_t SvTimer0_CompB = 0;  // count value for Output Compare Register B (number of prescaler units to count, then back to 0)
volatile uint8_t SvTimer0_SetupStatus = 0;   // to save RAM space, this byte has flag bits for setup status
#define SvfTimer0_Prescaler 0       // TCCR2B bits2:0 clock select, datasheet page 112, bits 2:0 in SvTimer0_SetupStatus
									// prescaler bits 2:0
									// 000 (0) - No clock source
									// 001 (1) - clk / 1 (full speed processor clock) (20 MHz) (Hz is "per second")
									// 010 (2) - clk/8 (one eighth clock) (2.5 MHz)
									// 011 (3) - clk/64 (one sixty-fourth clock) (312.5 KHz)
									// 100 (4) - clk/256 (divide by 256) (78,125 Hz)
									// 101 (5) - clk/1024 (divide by 1024) (19,531.25 Hz)
									// 110 (6) - external clock source on T0 pin (PD4, pin 6 on ATmega328).  Clock on falling edge.
									// 111 (7) - external clock source on T0 pin (PD4, pin 6 on ATmega328).  Clock on rising edge.
#define SvfTimer0_PrescalerNC 3     // no change to prescaler, bit number 3 in SvTimer0_SetupStatus
#define SvfTimer0_SetINTA 5         // set interrupt A, bit number 5 in SvTimer0_SetupStatus
#define SvfTimer0_SetINTB 4         // set interrupt B, bit number 4 in SvTimer0_SetupStatus
#define SvfTimer0_RTC_A 7           // use RTC version of ISR TIMER2_COMPA_vect, bit number 7 in SvTimer0_SetupStatus

// variables for Start_Timer1_CTC function (16-bit Timer/Counter1 with PWM)
volatile uint16_t SvTimer1_CompA = 0;  // count value for Output Compare Register A (number of prescaler units to count, then back to 0)
volatile uint16_t SvTimer1_CompB = 0;  // count value for Output Compare Register B (number of prescaler units to count, then back to 0)
volatile uint8_t SvTimer1_SetupStatus = 0;   // to save RAM space, this byte has flag bits for setup status
#define SvfTimer1_Prescaler 0       // TCCR1B bits2:0 clock select, datasheet page 112, bits 2:0 in SvTimer1_SetupStatus
									// prescaler bits 2:0
									// 000 (0) - No clock source
									// 001 (1) - clk / 1 (full speed processor clock) (20 MHz) (Hz is "per second")
									// 010 (2) - clk/8 (one eighth clock) (2.5 MHz)
									// 011 (3) - clk/64 (one sixty-fourth clock) (312.5 KHz)
									// 100 (4) - clk/256 (divide by 256) (78,125 Hz)
									// 101 (5) - clk/1024 (divide by 1024) (19,531.25 Hz)
									// 110 (6) - external clock source on T1 pin (PD5, pin 11 on ATmega328).  Clock on falling edge.
									// 111 (7) - external clock source on T1 pin (PD5, pin 11 on ATmega328).  Clock on rising edge.
#define SvfTimer1_PrescalerNC 3     // no change to prescaler, bit number 3 in SvTimer1_SetupStatus
#define SvfTimer1_SetINTA 5         // set interrupt A, bit number 5 in SvTimer1_SetupStatus
#define SvfTimer1_SetINTB 4         // set interrupt B, bit number 4 in SvTimer1_SetupStatus

// variables for Start_Timer2_CTC function (8-bit Timer/Counter2 with PWM and Asynchronous Operation)
volatile uint8_t SvTimer2_CompA = 0;  // count value for Output Compare Register A (number of prescaler units to count, then back to 0)
volatile uint8_t SvTimer2_CompB = 0;  // count value for Output Compare Register B (number of prescaler units to count, then back to 0)
volatile uint8_t SvTimer2_SetupStatus = 0;   // to save RAM space, this byte has flag bits for setup status
#define SvfTimer2_Prescaler 0       // TCCR2B bits2:0 clock select, datasheet page 112, bits 2:0 in SvTimer2_SetupStatus
									// prescaler bits 2:0
									// 000 (0) - No clock source
									// 001 (1) - clk / 1 (full speed processor clock) (20 MHz) (Hz is "per second")
									// 010 (2) - clk/8 (one eighth clock) (2.5 MHz)
									// 011 (3) - clk/64 (one thirty-second clock) (625 KHz)
									// 100 (4) - clk/64 (one sixty-fourth clock) (312.5 KHz)
									// 101 (5) - clk/128 (divide by 128) (156.25 KHz)
									// 110 (6) - clk/256 (divide by 256) (78,125 Hz)
									// 111 (7) - clk/1024 (divide by 1024) (19,531.25 Hz)
#define SvfTimer2_PrescalerNC 3     // no change to prescaler, bit number 3 in SvTimer2_SetupStatus
#define SvfTimer2_SetINTA 5         // set interrupt A, bit number 5 in SvTimer2_SetupStatus
#define SvfTimer2_SetINTB 4         // set interrupt B, bit number 4 in SvTimer2_SetupStatus
#define SvfTimer2_RTC_A 7           // use RTC version of ISR TIMER2_COMPA_vect, bit number 7 in SvTimer2_SetupStatus

// variables for the RTC
volatile uint8_t SvTenThousandths = 0;  // count 0-99 ten-thousandths of seconds
volatile uint8_t SvHundredths = 0;  // count 0-99 hundtedths of seconds
volatile uint16_t SvSeconds = 0;  // count 0-43,199 seconds in 12 hours
volatile uint16_t SvHalfDays = 9008;  	// 9008 = AM/Morning, May 1, 2012.  can count up to 89 years (65,536 half-days, 32,768 days).
										// 2000-2011 4,383 days + 2012 Jan-Apr 121 days = 4504.  9008 half days.
										// (can I make a compiler function to calculate this?)
										// (make a set date / time function?)
volatile uint8_t SvDisplayTime = 0;  // this is the counter for when to make it to the next row of the display

void Start_Timer0_RTC () {
	// 0.1mS (0.0001 Seconds) interrupt driven timer
	// count 250, clk/8 prescaler, with 20Mhz clock
	// sei();  *** need to put this in the main program - Enable global interrupts. ***
	// this was written using Dean Camera's tutorial "Newbie's Guide to AVR Timers" chapter 6
	
	SvTimer0_SetupStatus |= (1 << SvfTimer0_RTC_A);  // set flag to use RTC version of ISR TIMER0_COMPA_vect
	TCCR0A |= (1 << WGM01);  // set timer 0 for CTC mode
	TIMSK0 |= (1 << OCIE0A);  // Enable CTC interrupt
	OCR0A = 249;  // set Output Compare Register A to 249 (count 0-249) ( CTC compare value )
	TCCR0B |= (1 << CS01);  // start timer with prescaler clk/8
}

void Start_Timer2_RTC () {
	// 0.1mS (0.0001 Seconds) interrupt driven timer
	// count 250, clk/8 prescaler, with 20Mhz clock
	// sei();  *** need to put this in the main program - Enable global interrupts. ***
	// this was written using Dean Camera's tutorial "Newbie's Guide to AVR Timers" chapter 6
	
	SvTimer2_SetupStatus |= (1 << SvfTimer2_RTC_A);  // set flag to use RTC version of ISR TIMER2_COMPA_vect
	TCCR2A |= (1 << WGM21);  // set timer 0 for CTC mode
	TIMSK2 |= (1 << OCIE2A);  // Enable CTC interrupt
	OCR2A = 249;  // set Output Compare Register A to 249 (count 0-249) ( CTC compare value )
	TCCR2B |= (1 << CS21);  // start timer with prescaler clk/8
}

void Start_Timer0_CTC (uint8_t SvTimer0_SetupStatus, uint8_t SvTimer0_CompA, uint8_t SvTimer0_CompB) {
	// set Timer 0 and start it running with a chosen prescaler
	// sei();  *** if interrupts are being used, then need to put this in the main program - Enable global interrupts. ***
	// this was written using Dean Camera's tutorial "Newbie's Guide to AVR Timers" chapter 6

	TCCR0A |= (1 << WGM01);  // set timer 0 for CTC mode
	if (SvTimer0_SetupStatus & (1 << SvfTimer0_SetINTA)) {  // test for interrupt setting A
		TIMSK0 |= (1 << OCIE0A);  // enable CTC interrupt A
		SvTimer0_SetupStatus &= ~(1 << SvfTimer0_RTC_A);  // set flag to use NON-RTC version of ISR TIMER0_COMPA_vect
	}
	else {
		TIMSK0 &= ~(1 << OCIE0A);  // disable CTC interrupt A
	}
	OCR0A = SvTimer0_CompA;  // set Output Compare Register A to SvTimer0_CompA ( CTC compare value )
	if (SvTimer0_SetupStatus & (1 << SvfTimer0_SetINTB)) {  // test for interrupt setting B
		TIMSK0 |= (1 << OCIE0B);  // enable CTC interrupt B
	}
	else {
		TIMSK0 &= ~(1 << OCIE0B);  // disable CTC interrupt B
	}
	OCR0B = SvTimer0_CompB;  // set Output Compare Register A to SvTimer0_CompB ( CTC compare value )
	if (0 == (SvTimer0_SetupStatus & (1 << SvfTimer0_PrescalerNC))) {  // test for prescaler NC (no change) setting, if it's NOT 1 then go on to change prescaler
		TCCR0B &= ~(0x07);  // set bits 2:0 of TCCR0B to 0 (stop timer, disable clock)
		TCCR0B |= (SvTimer0_SetupStatus & 0x07);  // set all three bits 2:0 of TCCR0B with bits 2:0 of SvTimer0_SetupStatus (all at once to start the timer in the correct mode)
	}
}

void Start_Timer1_CTC (uint8_t SvTimer1_SetupStatus, uint16_t SvTimer1_CompA, uint16_t SvTimer1_CompB) {
	// set Timer 1 and start it running with a chosen prescaler
	// sei();  *** if interrupts are being used, then need to put this in the main program - Enable global interrupts. ***
	// this was written using Dean Camera's tutorial "Newbie's Guide to AVR Timers" chapter 6

	TCCR1B |= (1 << WGM12);  // set timer 1 for CTC mode
	if (SvTimer1_SetupStatus & (1 << SvfTimer1_SetINTA)) {  // test for interrupt setting A
		TIMSK1 |= (1 << OCIE1A);  // enable CTC interrupt A
	}
	else {
		TIMSK1 &= ~(1 << OCIE1A);  // disable CTC interrupt A
	}
	OCR1A = SvTimer1_CompA;  // set Output Compare Register A to SvTimer1_CompA ( CTC compare value )
	if (SvTimer1_SetupStatus & (1 << SvfTimer1_SetINTB)) {  // test for interrupt setting B
		TIMSK1 |= (1 << OCIE1B);  // enable CTC interrupt B
	}
	else {
		TIMSK1 &= ~(1 << OCIE1B);  // disable CTC interrupt B
	}
	OCR1B = SvTimer1_CompB;  // set Output Compare Register A to SvTimer1_CompB ( CTC compare value )
	if (0 == (SvTimer1_SetupStatus & (1 << SvfTimer1_PrescalerNC))) {  // test for prescaler NC (no change) setting, if it's NOT 1 then go on to change prescaler
		TCCR1B &= ~(0x07);  // set bits 2:0 of TCCR1B to 0 (stop timer, disable clock)
		TCCR1B |= (SvTimer1_SetupStatus & 0x07);  // set all three bits 2:0 of TCCR1B with bits 2:0 of SvTimer1_SetupStatus (all at once to start the timer in the correct mode)
	}
}

void Start_Timer2_CTC (uint8_t SvTimer2_SetupStatus, uint8_t SvTimer2_CompA, uint8_t SvTimer2_CompB) {
	// set Timer 2 and start it running with a chosen prescaler
	// sei();  *** if interrupts are being used, then need to put this in the main program - Enable global interrupts. ***
	// this was written using Dean Camera's tutorial "Newbie's Guide to AVR Timers" chapter 6

	TCCR2A |= (1 << WGM21);  // set timer 2 for CTC mode
	if (SvTimer2_SetupStatus & (1 << SvfTimer2_SetINTA)) {  // test for interrupt setting A
		TIMSK2 |= (1 << OCIE2A);  // enable CTC interrupt A
		SvTimer2_SetupStatus &= ~(1 << SvfTimer2_RTC_A);  // set flag to use NON-RTC version of ISR TIMER2_COMPA_vect
	}
	else {
		TIMSK2 &= ~(1 << OCIE2A);  // disable CTC interrupt A
	}
	OCR2A = SvTimer2_CompA;  // set Output Compare Register A to SvTimer2_CompA ( CTC compare value )
	if (SvTimer2_SetupStatus & (1 << SvfTimer2_SetINTB)) {  // test for interrupt setting B
		TIMSK2 |= (1 << OCIE2B);  // enable CTC interrupt B
	}
	else {
		TIMSK2 &= ~(1 << OCIE2B);  // disable CTC interrupt B
	}
	OCR2B = SvTimer2_CompB;  // set Output Compare Register A to SvTimer2_CompB ( CTC compare value )
	if (0 == (SvTimer2_SetupStatus & (1 << SvfTimer2_PrescalerNC))) {  // test for prescaler NC (no change) setting, if it's NOT 1 then go on to change prescaler
		TCCR2B &= ~(0x07);  // set bits 2:2 of TCCR2B to 0 (stop timer, disable clock)
		TCCR2B |= (SvTimer2_SetupStatus & 0x07);  // set all three bits 2:0 of TCCR2B with bits 2:0 of SvTimer2_SetupStatus (all at once to start the timer in the correct mode)
	}
}

void Timer0_Reset_to_Startup () {
	// Reset Timer0 registers to Startup
	// Clear previous Timer settings
	// sei - may need to be turned off in the main program

	SvTimer0_SetupStatus = 0;
	TCCR0A = 0;
	TCCR0B = 0;
	OCR0A = 0;
	OCR0B = 0;
	TIMSK0 = 0;
}

void Timer1_Reset_to_Startup () {
	// Reset Timer1 registers to Startup
	// Clear previous Timer settings
	// sei - may need to be turned off in the main program

	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	OCR1A = 0;
	OCR1B = 0;
	TIMSK1 = 0;
}

void Timer2_Reset_to_Startup () {
	// Reset Timer2 registers to Startup
	// Clear previous Timer settings
	// sei - may need to be turned off in the main program

	SvTimer2_SetupStatus = 0;
	TCCR2A = 0;
	TCCR2B = 0;
	OCR2A = 0;
	OCR2B = 0;
	TIMSK2 = 0;
	// ASSR rest?
	// GTCCR reset ?
}

ISR(TIMER0_COMPA_vect) {
	// RTC version and NON-RTC versions
	
	if (SvTimer0_SetupStatus & (1 << SvfTimer0_RTC_A)) {  // test if the flag is set to use the RTC version of ISR TIMER0_COMPA_vect
		// use this with the Real Time Clock - Start_Timer0_RTC function
		// count every 0.1mS (0.0001 Seconds)
		
		SvTenThousandths++ ;
		// anything that would need to happen each 0.0001 seconds should go here
		// but it should be something pretty brief
		if (SvTenThousandths > 99) {  // count 0-99 ten-thousandths in a hundredth
			SvTenThousandths = 0 ;
			SvHundredths++ ;
			// anything that would need to happen each 0.01 seconds should go here
			if (SvHundredths > 99) {  // count 0-99 hundredths in a second
				SvHundredths = 0 ;
				SvSeconds++ ;
				// anything that would need to happen each second should go here
				if (SvSeconds > 43199) {  // count 0-43,199 seconds (12 hours)
					SvSeconds = 0 ;
					SvHalfDays++ ;
					// anything that would need to happen each 12 hours should go here
				}
			}
		}
	}
	else {
		//  put NON-RTC version of ISR here
	}
}

ISR(TIMER2_COMPA_vect) {
	// RTC version and NON-RTC versions
	
	if (SvTimer2_SetupStatus & (1 << SvfTimer2_RTC_A)) {  // test if the flag is set to use the RTC version of ISR TIMER2_COMPA_vect
		// use this with the Real Time Clock - Start_Timer2_RTC function
		// count every 0.1mS (0.0001 Seconds)
		
		SvTenThousandths++ ;
		// anything that would need to happen each 0.0001 seconds should go here
		// but it should be something pretty brief
		SvDisplayTime++;  // this is the counter for when to make it to the next row of the display
		if (SvTenThousandths > 99) {  // count 0-99 ten-thousandths in a hundredth
			SvTenThousandths = 0 ;
			SvHundredths++ ;
			// anything that would need to happen each 0.01 seconds should go here
			if (SvHundredths > 99) {  // count 0-99 hundredths in a second
				SvHundredths = 0 ;
				SvSeconds++ ;
				// anything that would need to happen each second should go here
				if (SvSeconds > 43199) {  // count 0-43,199 seconds (12 hours)
					SvSeconds = 0 ;
					SvHalfDays++ ;
					// anything that would need to happen each 12 hours should go here
				}
			}
		}
	}
	else {
		// put NON-RTC version of ISR here
	}
}

// if other timers are used with interrupts, here are the starting place to put the ISR functions
// ISR(TIMER0_COMPB_vect)
// ISR(TIMER1_COMPA_vect)
// ISR(TIMER1_COMPB_vect)
// ISR(TIMER2_COMPB_vect)
