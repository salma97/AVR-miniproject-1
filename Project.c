#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

int seconds = 0, minutes = 0, hours = 0;

ISR (TIMER1_COMPA_vect)
{
	seconds++;

	if(seconds == 60)
	{
		seconds = 0;
		minutes++;
	}

	if(minutes == 60)
	{
		minutes = 0;
		hours++;
	}

	if(hours == 24)
	{
		hours = 0;
	}

}

ISR (INT0_vect)
{
	PORTC = 0x00;
	seconds = 0;
	minutes = 0;
	hours = 0;
}

void Timer1_Init (void)
{
	SREG |= (1<<7);
	TCCR1A |= (1<<FOC1A);
	TCCR1B |= (1<<WGM12) | (1<<CS10) | (1<<CS12);
	TIMSK |= (1<<OCIE1A);
	TCNT1 = 0;
	OCR1A = 1000;
}

void INT0_Init (void)
{
	SREG |= (1<<7);
	GICR |= (1<<INT0);
	MCUCR |= (1<<ISC01);
}

void Decoder (int number)
{
	switch(number)
	{
	case(1):
		PORTC = (PORTC & 0x00) | (seconds%10);
		break;
	case(2):
	    PORTC = (PORTC & 0x00) | (seconds/10);
	    break;
	case(4):
		PORTC = (PORTC & 0x00) | (minutes%10);
		break;
	case(8):
		PORTC = (PORTC & 0x00) | (minutes/10);
		break;
	case(16):
		PORTC = (PORTC & 0x00) | (hours%10);
		break;
	case(32):
		PORTC = (PORTC & 0x00) | (hours/10);
		break;
	}
}

int main (void)
{
	Timer1_Init();
	INT0_Init();

	DDRC = 0x0F;
	PORTC = 0x00;

	DDRA = 0xFF;
	PORTA = 0x01;

	DDRD &= (~(1<<PD2));
	PORTD |= (1<<PD2);

	while(1)
	{
		if (PORTA == 64)
		{
			PORTA = 0x01;
		}
		else
		{
			PORTA = (PORTA<<1);
		}

		Decoder(PORTA);
		_delay_ms(1);
	}

}
