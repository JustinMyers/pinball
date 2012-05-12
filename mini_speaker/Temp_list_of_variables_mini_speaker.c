
uint8_t SvNoteWaveLength78K[]

uint8_t SvSineWaveLookup256[] 

volatile uint8_t SvPlayArray[2][256]

volatile uint8_t SvArrayElements[] = { 255, 255 };

volatile uint8_t SvNoteHighNOTLow[] = { 1, 1 };

volatile uint8_t SvNoteFactor[2];  // initializes to 0

volatile uint8_t SvArrNoteDuration[] = { 63, 63 };

volatile uint16_t SvDurationResolution[] = { 305, 305 };

volatile uint8_t SvLiveArr;

volatile uint8_t SvNextLiveArr;

volatile uint8_t SvArrayLiveCounter;

volatile uint8_t SvFactor78KCounter;

volatile uint8_t SvLiveNoteDurationCounter;

volatile uint16_t SvLive78kCyclesCounter;

volatile uint8_t SvStartCalculatingNextNote;

struct NoteStruct {

	uint8_t SvNoteNum;

	uint16_t SvNoteDuration;

	uint8_t SvNoteVolume;

	};

uint8_t SvNumberOfNotesToPlay;

struct NoteStruct SvNotesToPlay[4];

struct NoteStruct SvNoteForPlayArray;

void SfToneGenerator (struct NoteStruct SvNoteForPlayArray);

void Start_Timer0_PWM (void);



int main (void) {

uint16_t SvMusicTempoBPMxSub;

uint8_t SvNoteStructNum;

SvStartCalculatingNextNote = 1;

SvLiveArr = 0;

SvNextLiveArr = 1;

SvDurationResolution[]

SvNumberOfNotesToPlay = 4;

SvArrayLiveCounter

SvFactor78KCounter

SvLive78kCyclesCounter

SvLiveNoteDurationCounter

SvNotesToPlay[]

SvNotesToPlay[0].SvNoteNum = 45;

SvNotesToPlay[0].SvNoteDuration = 64;

SvNotesToPlay[0].SvNoteVolume = 127;



void SfToneGenerator (struct NoteStruct SvNoteForPlayArray)

SvNoteForPlayArray

SvNoteForPlayArray.SvNoteNum

SvNoteForPlayArray.SvNoteDuration;

SvNoteForPlayArray.SvNoteVolume

uint8_t SvArrayCount;

uint8_t SvCyclesTotal;

uint16_t temp1;

uint8_t temp2;

uint8_t temp3;

SvStartCalculatingNextNote = 0;

SvArrNoteDuration

SvNextLiveArr

SvNoteHighNOTLow[]

SvNoteFactor[]

SvArrayElements[]

SvNoteWaveLength78K

SvPlayArray[][]

SvSineWaveLookup256


	
ISR(TIMER0_OVF_vect) {
	
SvNoteHighNOTLow[]

SvLiveArr

SvPlayArray[][]

SvArrayLiveCounter

SvArrayElements[]

SvArrayLiveCounter = 0;

SvLive78kCyclesCounter

SvDurationResolution[]

SvLiveNoteDurationCounter

SvArrNoteDuration[]

SvStartCalculatingNextNote = 1;

SvFactor78KCounter

SvNoteFactor[]
