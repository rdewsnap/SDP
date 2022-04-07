#include <asf.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define ADC_CHANNEL_0   0b00000000	// ADC0 Channel
#define ADC_CHANNEL_1   0b00000001  // ADC1 Channel
#define ADC_CHANNEL_2   0b00000010	// ADC2 Channel

char data[16] = "0000,0000,0000\n";
char delimiter = ',';

void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);
	
	/* Enable receiver and transmitter */

	
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

char USART_Receive(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_Transmit(char data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_send_str(char * str) {
	while (*str) 
	{
		USART_Transmit(*str++);
	}
}

uint16_t read_adc_channel(uint8_t channel_to_read)
{
	
	volatile uint16_t ADC_result = 0;
	
	// ATmega328p ADC Configuration Code
	
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  // Select ADC Prescalar to 128
	
	ADMUX   = ADMUX & 0xF0;   // Clear the lower bits MUX0-MUX3 of ADMUX,Will cause problems when channels are changed if not cleared
	
	//ADMUX  |= (1<<REFS0) | (1<<REFS1) ; // Select Vref = 1.1V internal since AREF pin of ATmega328P is connected to capacitor
	ADMUX  |= (1<<REFS0);                 // Select Vref = AVcc= 5V internal since AREF pin of ATmega328P is connected to capacitor
	
	// if AREF pin is connected directly to 5V, comment out the above line
	ADMUX  |= channel_to_read;          // Select the ADC channel to convert
	
	ADCSRA |= (1<<ADEN)  | (1<<ADSC);                // Enable ADC and Start the conversion
	
	while( !(ADCSRA & (1<<ADIF)) );                  // Wait for conversion to finish
	
	// When ADIF not Set,	[(ADCSRA & (1<<ADIF) = 0]
	//	So NOT[(ADCSRA & (1<<ADIF) ] = 1, While(loop is true),Stays there

	
	// When ADIF is Set,	[(ADCSRA & (1<<ADIF) = 1]
	//	So NOT[(ADCSRA & (1<<ADIF) ] = 0, While(loop is false),Exits and goes to next instruction
	
	
	
	ADC_result   =  ADC;      // "ADC" variable (AVR-GCC defined variable) Contains the whole 10 bit ADC result,
	// stored in the 16 bit ADC_10bit_Result Variable  (unsigned int)
	
	ADCSRA |= (1<<ADIF);            // Clear ADIF,ADIF is cleared by writing a 1 to ADSCRA (According to ATmega328p datasheet)
	
	
	
	//ADCSRA &= ~(1<<ADEN);
	
	return ADC_result;
}

void transmit_adc_value(uint16_t adc_10bit_value, int chan)
{
	//splitting the value to individual digits 1000's  100's  10's  1's   position
	// we will use 1023(decimal) as an eg        1       0      2     3
	// eg adc_value = 1023 decimal
	
	//char * data[4]; 
	
	uint16_t  temp = 0; //Temporary Storage (16bit)
	
	uint8_t   Ones_Position = 0;		// 8 bit variables to be send using usart
	uint8_t   Tens_Position = 0;
	uint8_t   Hundreths_Position =0;
	uint8_t   Thousands_Position =0;
	
	temp = read_adc_channel(adc_10bit_value);
	
	Ones_Position      = temp % 10; // Eg 1023 % 10  -> Remainder -> 3
	temp = temp/10;                 // Eg temp = 1023/10 ->102
	Tens_Position      = temp % 10; // Eg 102  % 10  -> Remainder -> 2
	temp = temp/10;                 // Eg temp = 102/10 ->10
	Hundreths_Position = temp % 10;  // Eg 10   % 10  -> Remainder -> 0
	Thousands_Position = temp / 10;  // Eg 10   / 10  -> Quotient  -> 1
	
	//ASCII Conversion
	Ones_Position += 0x30;
	Tens_Position += 0x30;
	Hundreths_Position += 0x30;
	Thousands_Position += 0x30;
	
	//Sending the 4 digit number to PC using USART
	/*
	USART_Transmit(Thousands_Position);
	USART_Transmit(Hundreths_Position);
	USART_Transmit(Tens_Position);
	USART_Transmit(Ones_Position);
	*/
	
	if(chan == 0){
		data[0] = Thousands_Position;
		data[1] = Hundreths_Position;
		data[2] = Tens_Position;
		data[3] = Ones_Position;
	}
	
	else if(chan == 1){
		data[5] = Thousands_Position;
		data[6] = Hundreths_Position;
		data[7] = Tens_Position;
		data[8] = Ones_Position;
	}
	else if(chan == 2){
		data[10] = Thousands_Position;
		data[11] = Hundreths_Position;
		data[12] = Tens_Position;
		data[13] = Ones_Position;
	}
	
	
	USART_send_str(data);
	//return data;
}

void ADC_to_UART(uint8_t adc_channel)
{
	uint16_t test = read_adc_channel(adc_channel);
	
	uint8_t high = test >> 8;
	uint8_t low = test & 0xFF;
	
	USART_Transmit(high);
	USART_Transmit(low);
}


int main (void)
{
	/* set board io port */
	board_init();
	USART_Init(MYUBRR);
	
	//DDRC |= (1 << DDC0);	// PC0 output
	
	DDRD |= (1 << DDD4);	// PD4 output LED
	
	char temp[4];

	while(1)
	{
		_delay_ms(500);
		//PORTC &= ~(1 << PC0);	// PC0 low
		PORTD &= ~(1 << PD4);
	
		_delay_ms(500);
		//PORTC |= (1 << PC0);	// PC0 high
		PORTD |= (1 << PD4);
		
		
		//USART_send_str(data2);
		
	/*	//temp = read_adc_channel(ADC_CHANNEL_0);
		//memcpy(temp, read_adc_channel(ADC_CHANNEL_0), sizeof(temp));
		transmit_adc_value(ADC_CHANNEL_0, temp);

		ADC_out[0] = temp[0];
		ADC_out[1] = temp[1];
		ADC_out[2] = temp[2];
		ADC_out[3] = temp[3];
		
		//temp = read_adc_channel(ADC_CHANNEL_1);
		//strcpy(temp, read_adc_channel((ADC_CHANNEL_1)));

		ADC_out[5] = temp[0];
		ADC_out[6] = temp[1];
		ADC_out[7] = temp[2];
		ADC_out[8] = temp[3];
		
		//temp = read_adc_channel(ADC_CHANNEL_2);		
		//strcpy(temp, read_adc_channel((ADC_CHANNEL_2)));

		ADC_out[10] = temp[0];
		ADC_out[11] = temp[1];
		ADC_out[12] = temp[2];
		ADC_out[13] = temp[3];
		*/
		transmit_adc_value(ADC_CHANNEL_0, 0);
		transmit_adc_value(ADC_CHANNEL_1, 1);
		transmit_adc_value(ADC_CHANNEL_2, 2);

		//USART_send_str(data);
					
	}
}
/*
	while(1)
	{
		_delay_ms(2000);
		//PORTC &= ~(1 << PC0);	// PC0 low
		PORTD &= ~(1 << PD4);
		
		_delay_ms(2000);
		//PORTC |= (1 << PC0);	// PC0 high
		PORTD |= (1 << PD4);	
		
		transmit_adc_value(read_adc_channel(ADC_CHANNEL_0));
		USART_Transmit('\n');										// WORKING PART 3/10/22
	}*/	


/*

int main (void)
{
	// set board io port 
	board_init();
	USART_Init(MYUBRR);
	
	//DDRC |= (1 << DDC0);	// PC0 output
	
	DDRD |= (1 << DDD4);	// PD4 output


	while(1)
	{
		_delay_ms(2000);
		//PORTC &= ~(1 << PC0);	// PC0 low
		PORTD &= ~(1 << PD4);
		
		_delay_ms(2000);
		//PORTC |= (1 << PC0);	// PC0 high
		PORTD |= (1 << PD4);	
		
		i = 0;
			
		while(data2[i] != 0)
		{
			USART_Transmit(data2[i]);
			i++;                             // increment counter           
		}
		USART_Transmit('\n');
		
		/*
		for (i=0; i<11; i++)
		{
			USART_Transmit(data2[i]);
		}
		USART_Transmit('\n');*/
		//USART_Transmit(data2);
	//}	
	/*
	bool button_state;
	while(1){
		button_state = ioport_get_pin_level(GPIO_PUSH_BUTTON_0);
		if(button_state){
			LED_Off(LED0);
		}else{
			LED_On(LED0);
		}
	}	*/
//}

