extern volatile uint8_t SvTimer0_SetupStatus;
extern volatile uint8_t SvTimer0_CompA;
extern volatile uint8_t SvTimer0_CompB;

extern volatile uint8_t SvTimer1_SetupStatus;
extern volatile uint16_t SvTimer1_CompA;
extern volatile uint16_t SvTimer1_CompB;

extern volatile uint8_t SvTimer2_SetupStatus;
extern volatile uint8_t SvTimer2_CompA;
extern volatile uint8_t SvTimer2_CompB;

extern volatile uint8_t SvTenThousandths;
extern volatile uint8_t SvHundredths;
extern volatile uint16_t SvSeconds;
extern volatile uint16_t SvHalfDays;
extern volatile uint8_t SvDisplayTime;

void Start_Timer0_RTC (void);  // Timer0, 0.1mS interrupt driven clock, ISA TIMER0_COMPA_vect
void Start_Timer0_CTC (uint8_t SvTimer0_SetupStatus, uint8_t SvTimer0_CompA, uint8_t SvTimer0_CompB);  // Timer0 CTC mode (Clear on Timer Compare)
void Timer0_Reset_to_Startup (void);  // set Timer0 registers to 0

void Start_Timer1_CTC (uint8_t SvTimer1_SetupStatus, uint16_t SvTimer1_CompA, uint16_t SvTimer1_CompB);  // Timer1 CTC mode (Clear on Timer Compare)
void Timer1_Reset_to_Startup (void);  // set Timer1 registers to 0

void Start_Timer2_RTC (void);  // Timer2, 0.1mS interrupt driven clock, ISA TIMER2_COMPA_vect
void Start_Timer2_CTC (uint8_t SvTimer2_SetupStatus, uint8_t SvTimer2_CompA, uint8_t SvTimer2_CompB);  // Timer2 CTC mode (Clear on Timer Compare)
void Timer2_Reset_to_Startup (void);  // set Timer2 registers to 0

// should I declare these in the header file?  I think yes, so the compiler/linker will know where they are
ISR(TIMER0_COMPA_vect);
ISR(TIMER2_COMPA_vect);
// ISR(TIMER0_COMPB_vect)
// ISR(TIMER1_COMPA_vect)
// ISR(TIMER1_COMPB_vect)
// ISR(TIMER2_COMPB_vect)

