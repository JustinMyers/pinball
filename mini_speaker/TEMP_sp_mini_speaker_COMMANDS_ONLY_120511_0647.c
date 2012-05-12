#include <avr/io.h>
#include <avr/interrupt.h>
uint16_t SvNoteWaveLength78K[] = { 251,237,224,211,199,188,178,168,158,149,141,133,251,237,224,211,199,188,178,168,158,149,141,133,251,237,224,211,199,188,178,168,158,149,141,133,251,237,224,211,199,188,178,168,158,149,141,133,251,237,224,211,199,188,178,168,158,149,141,133,251,237,224,211,199,188,178,168,158,149,141,133,251,237,224,211,199,188,178,168,158,149,141,133,251,237,224,211,199,188,178,168,158,149,141,133,251,237,224,211,199,188,178,168,158,149,141,133,251,237,224,211,199,188,178,168,158,149,141,133,251,237 } ;
uint8_t SvSineWaveLookup256[] = { 127,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,127,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124 } ;
volatile uint8_t SvPlayArray[2][256] = { { 127,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,127,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124 } ,  { 127,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,127,124,121,118,115,112,109,106,103,100,97,93,90,88,85,82,79,76,73,70,67,65,62,59,57,54,52,49,47,44,42,40,37,35,33,31,29,27,25,23,21,20,18,17,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,17,18,20,21,23,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,65,67,70,73,76,79,82,85,88,90,93,97,100,103,106,109,112,115,118,121,124 } } ;
volatile uint8_t SvArrayElements[] = { 255, 255 };
volatile uint8_t SvNoteHighNOTLow[] = { 1, 1 };  // just a binary value
volatile uint8_t SvNoteFactor[2];  // initializes to 0
volatile uint8_t SvArrNoteDuration[] = { 64, 64 };
volatile uint16_t SvDurationResolution[] = { 305, 305 };
volatile uint8_t SvLiveArr;  // binary value, selects which SvPlayArray[] is currently being "played"
volatile uint8_t SvArrayLiveCounter;  // counts which element is currently being "played", from 0 up to SvArrayElements[]
volatile uint8_t SvFactor78KCounter;  // for Low notes, counts the current count of 78K cycles, from 0 up to (1 << SvNoteFactor[])-1
volatile uint8_t SvLiveNoteDurationCounter;  // counts how long the current note has been played, when SvLiveNoteDurationCounter = SvArrNoteDuration[], toggle SvLiveArr
volatile uint8_t SvLive78kCyclesCounter;  // counts how many 78K cycles have been played in the current duration resolution unit
volatile uint8_t SvStartCalculatingNextNote;  // flag that indicates that SvLiveArray has been toggled, and it's time to start calculating the next note's SvPlayArray[]
struct NoteStruct {
	uint8_t SvNoteNum;
	uint16_t SvNoteDuration;
	uint16_t SvNoteVolume;
	};
uint8_t SvNumberOfNotesToPlay;
uint8_t SvNumberOfNotesToPlay = 4;
struct NoteStruct SvNotesToPlay[4];
struct NoteStruct SvNoteForPlayArray;
void SfToneGenerator (struct NoteStruct SvNoteForPlayArray);
void Start_Timer0_PWM (void);
int main (void) {
	uint16_t SvMusicTempoBPMxSub;
	uint8_t SvNoteStructNum;
	SvStartCalculatingNextNote = 1;
	SvMusicTempoBPMxSub = 120 * 128;  // beats per minute times number of fractional note durations, if using a faster than 120 BPM, use less than 128 fractional note durations
	SvDurationResolution[SvLiveArr] = ( (uint32_t)4687500 / SvMusicTempoBPMxSub );  // 4,687,500 = 78,125 * 60
	SvDurationResolution[ ~(SvLiveArr) ] = ( (uint32_t)4687500 / SvMusicTempoBPMxSub );  // 4,687,500 = 78,125 * 60
	SvNotesToPlay[0].SvNoteNum = 45;
	SvNotesToPlay[0].SvNoteDuration = 64;
	SvNotesToPlay[0].SvNoteVolume = 127;
	SvNotesToPlay[1].SvNoteNum = 48;
	SvNotesToPlay[1].SvNoteDuration = 64;
	SvNotesToPlay[1].SvNoteVolume = 127;
	SvNotesToPlay[2].SvNoteNum = 51;
	SvNotesToPlay[2].SvNoteDuration = 64;
	SvNotesToPlay[2].SvNoteVolume = 127;
	SvNotesToPlay[3].SvNoteNum = 54;
	SvNotesToPlay[3].SvNoteDuration = 64;
	SvNotesToPlay[3].SvNoteVolume = 127;
	sei();  // DUH!
	Start_Timer0_PWM ();
	while (1) {
		if (SvStartCalculatingNextNote == 1) {
			if (SvNoteStructNum >= SvNumberOfNotesToPlay) {
				SvNoteStructNum = 0;  // start over at beginning of list
			}
			SvArrayLiveCounter = 0;
			SvFactor78KCounter = 0;
			SfToneGenerator(SvNotesToPlay[SvNoteStructNum]);
			SvNoteStructNum++;
		}
	}
}
void SfToneGenerator (struct NoteStruct SvNoteForPlayArray) {
	uint8_t SvArrayCount;  // for loop counter
	uint8_t SvCyclesTotal;  // = ( 1 << SvNoteFactor[] )
	SvStartCalculatingNextNote = 0;  // calculating the next note SvPlayArray is started
	SvArrNoteDuration[ ~(SvLiveArr) ] = SvNoteForPlayArray.SvNoteDuration;  // currently going to set up the NON live array
	if (SvNoteForPlayArray.SvNoteNum < 48) {
		SvNoteHighNOTLow[ ~(SvLiveArr) ] = 0;  // false, this is not a high note, it is a low note
		SvNoteFactor[ ~(SvLiveArr) ] = (60-SvNoteForPlayArray.SvNoteNum) / 12;  // (1 << SvNoteFactor[]) is the number of 78K steps per entry in SvPlayArray[] (count by (1 << SvNoteFactor[]) when playing back
		SvArrayElements[ ~(SvLiveArr) ] = SvNoteWaveLength78K[SvNoteForPlayArray.SvNoteNum];
		for (SvArrayCount = 0; SvArrayCount < SvArrayElements[ ~(SvLiveArr) ]; SvArrayCount++) {
			SvPlayArray[ ~(SvLiveArr) ][SvArrayCount] = (SvSineWaveLookup256[ (SvArrayCount << 8) / SvArrayElements[ ~(SvLiveArr) ] ] * SvNoteForPlayArray.SvNoteVolume) >> 8;
		}
	}
	else {  // SvNoteNum > 47
		SvNoteHighNOTLow[ ~(SvLiveArr) ] = 0xFF;  // true, this is a high note, not a low note
		SvNoteFactor[ ~(SvLiveArr) ] = (SvNoteForPlayArray.SvNoteNum - 36) / 12;  // (1 << SvNoteFactor) is number of Sine Wave Cycles in the SvPlayArray
		SvCyclesTotal = ( 1 << SvNoteFactor[ ~(SvLiveArr) ] );
		SvArrayElements[ ~(SvLiveArr) ] = SvNoteWaveLength78K[SvNoteForPlayArray.SvNoteNum];
		for (SvArrayCount = 0; SvArrayCount < SvArrayElements[ ~(SvLiveArr) ]; SvArrayCount++) {
			SvPlayArray[ ~(SvLiveArr) ][SvArrayCount] = (SvSineWaveLookup256[ (((SvArrayCount << 8) / SvArrayElements[ ~(SvLiveArr) ]) - (((SvArrayCount * SvCyclesTotal) / SvArrayElements[ ~(SvLiveArr) ]) << (8 - SvNoteFactor[ ~(SvLiveArr) ]))) * SvCyclesTotal] * SvNoteForPlayArray.SvNoteVolume) >> 8;
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
	if (SvNoteHighNOTLow[SvLiveArr]) {  // this is a High note, notes > 47.  count through an element in the SvPlayArray[] for every 78k cycle.
		if (SvArrayLiveCounter >= SvArrayElements[SvLiveArr] ) {
			SvArrayLiveCounter = 0;
		}
		OCR0A = SvPlayArray[SvLiveArr][SvArrayLiveCounter];
		SvArrayLiveCounter++;
		if (SvLive78kCyclesCounter >= SvDurationResolution[SvLiveArr] ) {
			SvLive78kCyclesCounter = 0;
			SvLiveNoteDurationCounter++;
		}
		if (SvLiveNoteDurationCounter >= SvArrNoteDuration[SvLiveArr] ) {
			SvLiveArr = ~(SvLiveArr);  // toggle to the other array, it's now the "live" array being "played"
			SvStartCalculatingNextNote = 0;  // flag indicates it's time to start calculating the new next note SvPlayArray
		}
	}
	else {  // this is a low note, notes < 48.  count through an element in the SvPlayArray[] for every (1 << SvNoteFactor) times 78k cycle.
		if (SvFactor78KCounter >= (1 << SvNoteFactor[SvLiveArr] ) ) {
			SvFactor78KCounter = 0;
			if (SvArrayLiveCounter >= SvArrayElements[SvLiveArr] ) {
				SvArrayLiveCounter = 0;
			}
			OCR0A = SvPlayArray[SvLiveArr][SvArrayLiveCounter];
			SvArrayLiveCounter++;
		}
		if (SvLive78kCyclesCounter >= SvDurationResolution[SvLiveArr] ) {
			SvLive78kCyclesCounter = 0;
			SvLiveNoteDurationCounter++;
		}
		if (SvLiveNoteDurationCounter >= SvArrNoteDuration[SvLiveArr] ) {
			SvLiveArr = ~(SvLiveArr);  // toggle to the other array, it's now the "live" array being "played"
			SvStartCalculatingNextNote = 0;  // flag indicates it's time to start calculating the new next note SvPlayArray
		}
	}
}
