#include <avr/io.h>
#include <avr/interrupt.h>

// remember to rewire and change programs for moving the connections to the LED columns
//     from "PD5, PD6, PD7, PB0" to "PC2, PC3, PC4, PC5".  from "pins 11, 12, 13 & 14" to "pins 25, 26, 27, 28"
//     to open up the OC0A (PD6 pin 12) and OC0B (PD5 pin 11) PWM pins.  (a couple the switch/button connections can move to pins 13 & 14)

// the periods frequencies of notes.  440Hz base, repeated octaves, binary multiples of the number of cycles at 78,125Hz (PWM duty cycle).
// note 48 is base note for octave 0, note 0.  note 0 is MIDI note 15, note 112 is MIDI note 127.
// lookup table array - 122 entries (0-121), 440Hz reference note is note 54, middle C is note entry 45 (299 periods of 78,125 Hz)
// when I learn how, put this in Flash (PROGMEM method)
uint8_t SvNoteWaveLength78K[] = { 250,236,223,210,198,187,177,167,157,148,140,132,250,236,223,210,198,187,177,167,157,148,140,132,250,236,223,210,198,187,177,167,157,148,140,132,250,236,223,210,198,187,177,167,157,148,140,132,250,236,223,210,198,187,177,167,157,148,140,132,250,236,223,210,198,187,177,167,157,148,140,132,250,236,223,210,198,187,177,167,157,148,140,132,250,236,223,210,198,187,177,167,157,148,140,132,250,236,223,210,198,187,177,167,157,148,140,132,250,236,223,210,198,187,177,167,157,148,140,132,250,236 } ;

// lookup table array - Sine Wave - 256 steps (0-255) with values from 0-255
// when I learn how, put this in Flash (PROGMEM method)
uint8_t SvCosineWaveLookup256[] = { 255,255,255,255,254,254,254,253,253,252,251,250,249,249,248,246,245,244,243,241,240,238,237,235,233,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,195,193,190,187,185,182,179,176,173,170,167,164,161,158,155,152,149,146,143,140,136,133,130,127,124,121,118,114,111,108,105,102,99,96,93,90,87,84,81,78,75,72,69,67,64,61,59,56,53,51,48,46,43,41,39,36,34,32,30,28,26,24,22,21,19,17,16,14,13,11,10,9,8,6,5,5,4,3,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,3,4,5,5,6,8,9,10,11,13,14,16,17,19,21,22,24,26,28,30,32,34,36,39,41,43,46,48,51,53,56,59,61,64,67,69,72,75,78,81,84,87,90,93,96,99,102,105,108,111,114,118,121,124,127,130,133,136,140,143,146,149,152,155,158,161,164,167,170,173,176,179,182,185,187,190,193,195,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,233,235,237,238,240,241,243,244,245,246,248,249,249,250,251,252,253,253,254,254,254,255,255,255 } ;

// two arrays, one that is being activley "played", the other that is being prepared for use
// initial settings here are the maximum size that is intended to be used
// SvArrayElements[] is the number of elements in the PlayArray[] array (initial value is 256, to match the initial PlayArray[] array, elements are numbered 0-255)
// the value of SvArrayElements[] is taken from the lookup table array SvNoteWaveLength78K[]
volatile uint8_t SvPlayArray[2][256] = { { 127,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,127,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124 } ,  { 127,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,127,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124 } } ;
volatile uint8_t SvArrayElements[] = { 255, 255 };

// SvNoteHighNOTLow[] - for ISR, indicates which method of counting cycles to use.
// Low notes (notes 0-47) count through SvPlayArray[] by binary multiples(?) of 78K cycles.  wait multiple (1 << SvNoteFactor) 78K cycles per array element.
// base octave, octave 0, notes 48-59, step through the 1 sine wave cycle in  SvPlayArray[], 1 step per 78K cycle.
// High notes (notes 48-121) move to the next element of SvPlayArray[] each 78K cycle.  SvPlayArray[] contains multiple (1 << SvNoteFactor) sine wave cycles.
// notes 0-11, SvNoteHighNOTLow[] = 0, SvNoteFactor[] = 
volatile uint8_t SvNoteHighNOTLow[] = { 1, 1 };  // just a binary value
volatile uint8_t SvNoteFactor[] = { 0, 0 };   // indicates how many octaves the note is from the base octave.  octave 0 is notes 48-59.
volatile uint8_t SvNoteFactoredCount[] = { 0, 0 };  // for compare value in ISR, will equal ( ( 1 << SvNoteFactor[] ) - 1 )

volatile uint8_t SvArrNoteDuration[] = { 31, 31 };  // numbered in SvDurationResolution units (the fractional note durations of BPM), counting from 0 to total counts, up to 255
volatile uint16_t SvDurationResolution[] = { 304, 304 };  // start counting from 0.  number of 78K cycles for in the minimum fractional note duration
volatile uint8_t SvLiveArr;  // binary value, selects which SvPlayArray[] is currently being "played"
volatile uint8_t SvNextLiveArr;  // binary value, selects which SvPlayArray[] is next
volatile uint8_t SvNextPWMValue;
volatile uint8_t SvArrayLiveCounter;  // counts which element is currently being "played", from 0 up to SvArrayElements[]
volatile uint8_t SvFactor78KCounter;  // for Low notes, counts the current number of 78K cycles, from 0 up to ( ( 1 << SvNoteFactor[] ) - 1 ).  ( up to 1, 3, 7 or 15 )
volatile uint8_t SvLiveNoteDurationCounter;  // counts how long the current note has been played, when SvLiveNoteDurationCounter = SvArrNoteDuration[], toggle SvLiveArr
volatile uint16_t SvLive78KCyclesCounter;  // counts how many 78K cycles have been played in the current duration resolution unit
volatile uint8_t SvStartCalculatingNextNote;  // flag that indicates that SvLiveArray has been toggled, and it's time to start calculating the next note's SvPlayArray[]
volatile uint8_t SvResetGoToNextNote;  // flag for the ISR to reset the counter variables and change to the next note, swap SvLiveArr and SvNextLiveArr settings

struct NoteStruct {
	uint8_t SvNoteNum;
	uint8_t SvNoteDuration;  // counting from 0 to total counts
	uint16_t SvNoteVolume;
};
uint8_t SvNumberOfNotesToPlay;
struct NoteStruct SvNotesToPlay[27];  // start counting number of notes from 1 here
struct NoteStruct SvNoteForPlayArray;

void SfToneGenerator (struct NoteStruct SvNoteForPlayArray);
void Start_Timer0_PWM (void);

int main (void) {
	
	uint16_t SvMusicTempoBPMxSub;
	uint8_t SvNoteStructNum;
	
	//avr-libc assures us that variables will automatically be initiallized to zero.
	
	// the smallest value for note duration should be no less than 1/305 of a second, 256 "78K cycles".
	// this minimum can be increased up to 512 "78K cycles", to work out various BPM settings.
	// beginning example 120 BPM with 128th notes.  1/256 of a second minimum note duration, this is 305 "78K cycles".
	SvMusicTempoBPMxSub = 100 * 128;  // beats per minute times the number of fractional note durations, if using a faster BPM than 120, use less than 128th fractional note durations
	SvDurationResolution[0] = ( (uint32_t)4687500 / SvMusicTempoBPMxSub ) - 1 ;  // number of PWM duty cycles in a minute is 4,687,500 = 78,125 * 60
	SvDurationResolution[1] = ( (uint32_t)4687500 / SvMusicTempoBPMxSub ) - 1 ;  // number of PWM duty cycles in a minute is 4,687,500 = 78,125 * 60
	
	SvNumberOfNotesToPlay = 26;  // start counting from 0 here
	
	SvNotesToPlay[0].SvNoteNum = 47;
	SvNotesToPlay[0].SvNoteDuration = 95;
	SvNotesToPlay[0].SvNoteVolume = 127;
	
	SvNotesToPlay[1].SvNoteNum = 47;
	SvNotesToPlay[1].SvNoteDuration = 31;
	SvNotesToPlay[1].SvNoteVolume = 127;
	
	SvNotesToPlay[2].SvNoteNum = 49;
	SvNotesToPlay[2].SvNoteDuration = 63;
	SvNotesToPlay[2].SvNoteVolume = 127;
	
	SvNotesToPlay[3].SvNoteNum = 47;
	SvNotesToPlay[3].SvNoteDuration = 63;
	SvNotesToPlay[3].SvNoteVolume = 127;
	
	SvNotesToPlay[4].SvNoteNum = 52;
	SvNotesToPlay[4].SvNoteDuration = 63;
	SvNotesToPlay[4].SvNoteVolume = 127;
	
	SvNotesToPlay[5].SvNoteNum = 50;
	SvNotesToPlay[5].SvNoteDuration = 127;
	SvNotesToPlay[5].SvNoteVolume = 127;
	
	SvNotesToPlay[6].SvNoteNum = 47;
	SvNotesToPlay[6].SvNoteDuration = 95;
	SvNotesToPlay[6].SvNoteVolume = 127;
	
	SvNotesToPlay[7].SvNoteNum = 47;
	SvNotesToPlay[7].SvNoteDuration = 31;
	SvNotesToPlay[7].SvNoteVolume = 127;
	
	SvNotesToPlay[8].SvNoteNum = 49;
	SvNotesToPlay[8].SvNoteDuration = 63;
	SvNotesToPlay[8].SvNoteVolume = 127;
	
	SvNotesToPlay[9].SvNoteNum = 47;
	SvNotesToPlay[9].SvNoteDuration = 63;
	SvNotesToPlay[9].SvNoteVolume = 127;
	
	SvNotesToPlay[10].SvNoteNum = 54;
	SvNotesToPlay[10].SvNoteDuration = 63;
	SvNotesToPlay[10].SvNoteVolume = 127;
	
	SvNotesToPlay[11].SvNoteNum = 52;
	SvNotesToPlay[11].SvNoteDuration = 127;
	SvNotesToPlay[11].SvNoteVolume = 127;
	
	SvNotesToPlay[12].SvNoteNum = 47;
	SvNotesToPlay[12].SvNoteDuration = 95;
	SvNotesToPlay[12].SvNoteVolume = 127;
	
	SvNotesToPlay[13].SvNoteNum = 47;
	SvNotesToPlay[13].SvNoteDuration = 31;
	SvNotesToPlay[13].SvNoteVolume = 127;
	
	SvNotesToPlay[14].SvNoteNum = 59;
	SvNotesToPlay[14].SvNoteDuration = 63;
	SvNotesToPlay[14].SvNoteVolume = 127;
	
	SvNotesToPlay[15].SvNoteNum = 56;
	SvNotesToPlay[15].SvNoteDuration = 63;
	SvNotesToPlay[15].SvNoteVolume = 127;
	
	SvNotesToPlay[16].SvNoteNum = 52;
	SvNotesToPlay[16].SvNoteDuration = 63;
	SvNotesToPlay[16].SvNoteVolume = 127;
	
	SvNotesToPlay[17].SvNoteNum = 50;
	SvNotesToPlay[17].SvNoteDuration = 63;
	SvNotesToPlay[17].SvNoteVolume = 127;
	
	SvNotesToPlay[18].SvNoteNum = 49;
	SvNotesToPlay[18].SvNoteDuration = 63;
	SvNotesToPlay[18].SvNoteVolume = 127;
	
	SvNotesToPlay[19].SvNoteNum = 57;
	SvNotesToPlay[19].SvNoteDuration = 95;
	SvNotesToPlay[19].SvNoteVolume = 127;
	
	SvNotesToPlay[20].SvNoteNum = 57;
	SvNotesToPlay[20].SvNoteDuration = 31;
	SvNotesToPlay[20].SvNoteVolume = 127;
	
	SvNotesToPlay[21].SvNoteNum = 56;
	SvNotesToPlay[21].SvNoteDuration = 63;
	SvNotesToPlay[21].SvNoteVolume = 127;
	
	SvNotesToPlay[22].SvNoteNum = 52;
	SvNotesToPlay[22].SvNoteDuration = 63;
	SvNotesToPlay[22].SvNoteVolume = 127;
	
	SvNotesToPlay[23].SvNoteNum = 54;
	SvNotesToPlay[23].SvNoteDuration = 63;
	SvNotesToPlay[23].SvNoteVolume = 127;

	SvNotesToPlay[24].SvNoteNum = 52;
	SvNotesToPlay[24].SvNoteDuration = 127;
	SvNotesToPlay[24].SvNoteVolume = 127;
	
	
	SvNotesToPlay[25].SvNoteNum = 45;
	SvNotesToPlay[25].SvNoteDuration = 127;
	SvNotesToPlay[25].SvNoteVolume = 1;

	SvNotesToPlay[26].SvNoteNum = 45;
	SvNotesToPlay[26].SvNoteDuration = 127;
	SvNotesToPlay[26].SvNoteVolume = 1;

	
	
	//SvNotesToPlay[0].SvNoteNum = 0;
	//SvNotesToPlay[0].SvNoteDuration = 95;
	//SvNotesToPlay[0].SvNoteVolume = 127;
	
	SvLiveArr = 1;
	SvNextLiveArr = 0;
	SfToneGenerator(SvNotesToPlay[0]);  // fill the SvPlayArray[0] with useful data
	SvLiveArr = 0;
	SvNextLiveArr = 1;
	SvNoteStructNum = 1;
	SvResetGoToNextNote = 0;
	SvStartCalculatingNextNote = 1;
	SvNextPWMValue = 255;
	SvLive78KCyclesCounter = 0;
	SvLiveNoteDurationCounter = 0;
	SvArrayLiveCounter = 0;
	SvFactor78KCounter = 0;
	
	sei();  // DUH!
	Start_Timer0_PWM ();
	
	for (;;) {
		
		//if ( SvStartCalculatingNextNote == 1 ) {
			//SvNotesToPlay[0].SvNoteNum = SvNoteStructNum;  // cycle through every note from NoteNum 0 to NoteNum 121
			//SfToneGenerator( SvNotesToPlay[0] );
			//if ( SvNoteStructNum >= 121 ) {
				//SvNoteStructNum = 0;  // start over at beginning of NoteNums
			//}
			//else SvNoteStructNum++;
		//}
		
		if (SvStartCalculatingNextNote == 1) {
			SfToneGenerator(SvNotesToPlay[SvNoteStructNum]);  // play the notes from the list in the array
			if (SvNoteStructNum >= SvNumberOfNotesToPlay) {
				SvNoteStructNum = 0;  // start over at beginning of list
			}
			else SvNoteStructNum++;
		}
	}
}

void SfToneGenerator (struct NoteStruct SvNoteForPlayArray) {
	
	uint16_t SvArrayCount;  // for loop counter
	uint16_t temp1;
	uint8_t temp2;
	uint16_t temp3;
	uint8_t temp4;
	
	SvStartCalculatingNextNote = 0;  // calculating the next note SvPlayArray is started
	SvArrNoteDuration[SvNextLiveArr] = SvNoteForPlayArray.SvNoteDuration;  // currently going to set up the NON live array
	
	if ( SvNoteForPlayArray.SvNoteNum < 48 ) {  // low note version, notes 0-47.
		//step one or more (2, 4, 8, or 16) 78K step(s) along the one sine wave in SvPlayArray[] 
		
		SvNoteHighNOTLow[SvNextLiveArr] = 0;  // set to false, this is a low note, not a high note
		SvNoteFactor[SvNextLiveArr] = ( 59 - SvNoteForPlayArray.SvNoteNum ) / 12;  // (1 << SvNoteFactor[]) is the number of 78K steps per entry in SvPlayArray[] (count by (1 << SvNoteFactor[]) when playing back
		SvNoteFactoredCount[SvNextLiveArr] = ( 1 << SvNoteFactor[SvNextLiveArr] ) - 1;    // for compare value in ISR, counts the multiplier for the sinewave (1, 3, 7, 15)
		SvArrayElements[SvNextLiveArr] = SvNoteWaveLength78K[SvNoteForPlayArray.SvNoteNum];
		for ( SvArrayCount = 0; SvArrayCount <= SvArrayElements[SvNextLiveArr]; SvArrayCount++ ) {
			temp1 = SvArrayCount << 8;  // temp1 is 16 bit, need 16 bit result
			SvPlayArray[SvNextLiveArr][SvArrayCount] = ( SvCosineWaveLookup256[ temp1 / ( SvArrayElements[SvNextLiveArr] + 1 ) ] * SvNoteForPlayArray.SvNoteVolume ) >> 8;
		}
	}
	else {  // high note version, SvNoteNum >= 48.  notes 48-121.
		//step one 78K step along the two or more (2, 4, 8, 16, 32 or 64) sine waves in SvPlayArray[] 
		
		SvNoteHighNOTLow[SvNextLiveArr] = 1;  // set to true, this is a high note, not a low note
		SvNoteFactor[SvNextLiveArr] = ( (SvNoteForPlayArray.SvNoteNum - 48) / 12 );  // (1 << SvNoteFactor) is number of Sine Wave Cycles in the SvPlayArray
		SvNoteFactoredCount[SvNextLiveArr] = 0; // this value is not used in high note.  this is for the low note counter
		SvArrayElements[SvNextLiveArr] = SvNoteWaveLength78K[SvNoteForPlayArray.SvNoteNum];
		for ( SvArrayCount = 0; SvArrayCount <= SvArrayElements[SvNextLiveArr]; SvArrayCount++ ) {
			temp1 = SvArrayCount << 8;  // temp1 is 16 bit, need 16 bit result
			temp2 = temp1 / ( SvArrayElements[SvNextLiveArr] + 1 ) ;  // temp2 is intermediate result to make the equation shorter
			temp3 = SvArrayCount << SvNoteFactor[SvNextLiveArr];  // temp3 is 16 bit, need 16 bit result
			temp4 = ( temp3 / ( SvArrayElements[SvNextLiveArr] + 1 ) ) << ( 8 - SvNoteFactor[SvNextLiveArr] );  // temp4 is intermediate result to make the equation shorter
			SvPlayArray[SvNextLiveArr][SvArrayCount] = ( SvCosineWaveLookup256[ (temp2 - temp4) << SvNoteFactor[SvNextLiveArr] ] * SvNoteForPlayArray.SvNoteVolume ) >> 8;
		}
	}
}

void Start_Timer0_PWM (void) {
	
	DDRD = (1 << DDD6);  // set OC0A PWM pin as output (PD6, pin 12)
	TCCR0A |= (1 << WGM01) | (1 << WGM00);  // set timer 0 for Fast PWM mode
	TCCR0A |= (1 << COM0A1);  // Clear OC0A on Compare Match, set OC0A at BOTTOM
	TIMSK0 |= (1 << TOIE0);  // Enable PWM overflow interrupt, REMEMBER TO START ALL INTERRUPTS sei(); 
	OCR0A = 0;  // set Output Compare Register A to 0 (one cycle on per 256)
	TCCR0B |= (1 << CS00);  // start PWM timer with no prescaler
}

ISR(TIMER0_OVF_vect) {
	
	OCR0A = SvNextPWMValue;  // all that for this, put the PWM value in the timer register
	SvNextPWMValue = SvPlayArray[SvLiveArr][SvArrayLiveCounter];
	
	if ( SvLive78KCyclesCounter >= SvDurationResolution[SvLiveArr] ) {  // end of a Duration Resolution unit
		SvLive78KCyclesCounter = 0;
		if ( SvLiveNoteDurationCounter >= SvArrNoteDuration[SvLiveArr] ) {  // end of duration for playing this note
			SvLiveNoteDurationCounter = 0;
			SvResetGoToNextNote = 1;
		}
		else SvLiveNoteDurationCounter++;  // count through the Duration for this note
	}
	else SvLive78KCyclesCounter++;  // count through one DurationResolution unit
	
	if ( SvResetGoToNextNote > 0 ) {  // reset the counter variables and change to the next note, swap SvLiveArr and SvNextLiveArr settings
		SvResetGoToNextNote = 0;
		SvNextPWMValue = 255;
		SvLive78KCyclesCounter = 0;
		SvLiveNoteDurationCounter = 0;
		SvArrayLiveCounter = 0;
		SvFactor78KCounter = 0;
		SvStartCalculatingNextNote = 1;  // flag indicates it's time to start calculating the new next note SvPlayArray
		if ( SvLiveArr == 1 ) {  // toggle to the other array, it's now the "live" array being "played"
			SvLiveArr = 0;
			SvNextLiveArr = 1;
		}
		else {
			SvLiveArr = 1;
			SvNextLiveArr = 0;
		}
	} else {  // continue with normal counting through this note in the SvPlayArray
		if ( SvNoteHighNOTLow[SvLiveArr] == 1 ) {  // this is a High note, notes > 47.  count through an element in the SvPlayArray[] for every 78k cycle.
			if ( SvArrayLiveCounter >= SvArrayElements[SvLiveArr] ) {
				SvArrayLiveCounter = 0;
			}
			else SvArrayLiveCounter++;
		}
		else {  // this is a low note, notes <= 47.  count through an element in the SvPlayArray[] for every (1 << SvNoteFactor) times 78k cycle.
			if ( SvFactor78KCounter >= SvNoteFactoredCount[SvLiveArr] ) {
				SvFactor78KCounter = 0;
				if (SvArrayLiveCounter >= SvArrayElements[SvLiveArr] ) {
					SvArrayLiveCounter = 0;
				}
				else SvArrayLiveCounter++;
			}
			else SvFactor78KCounter++;
		}
	}
}
