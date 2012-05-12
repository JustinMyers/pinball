int main (void) {
	
ISR(TIMER0_OVF_vect) {

void SfToneGenerator (struct NoteStruct SvNoteForPlayArray);

void Start_Timer0_PWM (void);
	
	

volatile uint8_t SvArrayElements[]

volatile uint8_t SvArrNoteDuration[]

volatile uint16_t SvDurationResolution[]

volatile uint8_t SvNoteFactor[2]

volatile uint8_t SvNoteHighNOTLow[]

volatile uint8_t SvPlayArray[2][256]

volatile uint8_t SvArrayLiveCounter;

volatile uint8_t SvFactor78KCounter;

volatile uint16_t SvLive78kCyclesCounter;

volatile uint8_t SvLiveArr;

volatile uint8_t SvLiveNoteDurationCounter;

volatile uint8_t SvNextLiveArr;

volatile uint8_t SvStartCalculatingNextNote;



uint8_t SvArrayCount;

uint8_t SvCyclesTotal;

uint16_t SvMusicTempoBPMxSub;

uint8_t SvNoteStructNum;

uint8_t SvNoteWaveLength78K[]

uint8_t SvNumberOfNotesToPlay;

uint8_t SvSineWaveLookup256[]

uint16_t temp1;

uint8_t temp2;

uint8_t temp3;

struct NoteStruct {

	uint16_t SvNoteDuration;

	uint8_t SvNoteNum;

	uint8_t SvNoteVolume;

SvArrayElements[]

SvArrayLiveCounter

SvArrNoteDuration[]

SvDurationResolution[]

SvFactor78KCounter

SvLive78kCyclesCounter

SvLiveArr

SvLiveNoteDurationCounter

SvNextLiveArr

SvNoteFactor[]

struct NoteStruct SvNoteForPlayArray;

SvNoteForPlayArray

SvNoteForPlayArray.SvNoteDuration;

SvNoteForPlayArray.SvNoteNum

SvNoteForPlayArray.SvNoteVolume

SvNoteHighNOTLow[]

struct NoteStruct SvNotesToPlay[];

SvNotesToPlay[]

SvNotesToPlay[].SvNoteDuration

SvNotesToPlay[].SvNoteNum

SvNotesToPlay[].SvNoteVolume

SvNoteWaveLength78K

SvNumberOfNotesToPlay

SvPlayArray[][]

SvSineWaveLookup256

SvStartCalculatingNextNote
