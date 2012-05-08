#ifndef PINBALL_PROTOCOL
#define PINBALL_PROTOCOL

#define USART_BAUDRATE 115200
// #define USART_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define USART_PRESCALE 10 // baudrate 115200, from ATmega328 datasheet, page 200


#define MAX_MESSAGE_LENGTH 100

void USARTSetup(void);
void TIMERSetup(void);

void protocol_send_message( int from, int to, char message[] );
void protocol_send_data(char data[]);

#endif
