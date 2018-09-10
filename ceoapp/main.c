/*
 * ceoapp.c
 *
 * Created: 31.8.2018. 23.33.52
 * Author : User23
 */ 
#define  SMP 1
#define  F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "LCD/lcd.h"
int count;//brojaè za interrupt 16000000/1024=15625-1=15624/256=61
void Timer1Init(void);
void Timer0Init(void);
void KontrolnaLampica(void);
void SetovanjePortovaZaESC(void);
void ESCFunkcija(void);
void MerenjeIspisFrekvencijePolling(void);



int main(void)
{
	sei();
	SetovanjePortovaZaESC();
	Timer0Init();
	Timer1Init();
	_delay_ms(1000);
	while (1)
	{
		ESCFunkcija();
	
	}
	

}

void Timer1Init()//INICIJALIZACIJA TAJMERA 1
{
	TCCR1A  = 0;//NORMALNI MOD
	//TCCR1B |= 1<<CS10 | 1<<CS11 | 1<<CS12 ;//T1 CLOCK RISING EDGE
	TCCR1B &= 1<<ICES1;
	TCCR1B |= 1<<CS11 | 1<<CS12;//T1 CLOCK FALLING EDGE
	TIMSK |= 1<<TOIE1;//OMOGUÆI OVERFLOW INTERUPT
	TIFR |= 1<<TOV1; //FLAG ZA OVERFLOW NITERRUPT
	TCNT1=0;
	
}

void KontrolnaLampica()
{
	DDRD |= 1<<DDD7; //OUTPUT PORT ZA KONTROLNU LAMPICU
	PORTD ^= 1<<PORTD7;
}
void SetovanjePortovaZaESC()
{
	
	PORTA =(1<<PORTA0) | (1<<PORTA1) | (1<<PORTA2) | (1<<PORTA3); // 0b00011110;//setovanje pullup-ova na prva èetiri pina porta A
	DDRA = (1<<DDA4) | (1<<DDA5) | (1<<DDA6); //0b11100000;//setovanje i/o pinova na portu A
	
}
void ESCFunkcija()
{
	  if (!(PINA &(1<<PINA3)))//ESC ON
	  {
		    //debouncing
		    _delay_ms(50);
		    while(!(PINA &(1<<PINA3)));
			_delay_ms(40);
		    PORTA |= 1<<PORTA4;
		    PORTD ^= 1<<PORTD7;
		
	  }
	 else if(!(PINA &(1<<PINA2)))//RPM +
	 {
		 //debouncing
		 _delay_ms(50);
		 while(!(PINA &(1<<PINA2)));
		 PORTD ^= 1<<PORTD7;
		 PORTA |=1<<PORTA5;
		 _delay_ms(200);
		 PORTA &= ~(1<<PORTA5);
		 
	 }
	 else if(!(PINA &(1<<PINA1)))//RPM -
	 {
		 //debouncing
		 _delay_ms(50);
		 while(!(PINA &(1<<PINA1)));
		  PORTD ^= 1<<PORTD7;
		 PORTA |=1<<PORTA6;
		 _delay_ms(200);
		 PORTA &= ~(1<<PORTA6);
		 
	 }
	 else if(!(PINA &(1<<PINA0)))//STOP I ESC OFF
	 {
		 //debouncing
		 _delay_ms(30);
		 while(!(PINA &(1<<PINA0)));
		 _delay_ms(40);

		 int i;
		 for (i = 0; i < 50; i++)
		 {
			 PORTD ^= 1<<PORTD7;
			 PORTA |=1<<PORTA6;
			 _delay_ms(50);
			 PORTA &= ~(1<<PORTA6);
			 _delay_ms(50);
			 
		 }
		 PORTA &=~(1<<PORTA4);
 	 }
	
	 
}
void MerenjeIspisFrekvencijePolling()
{
	volatile int i;
	volatile   unsigned int freq;
	const char buffer[11];
	volatile unsigned int rpm;
		_delay_ms(1000/SMP);
		i=TCNT1;
		freq=i*SMP;
		rpm=freq*5;
		utoa(rpm, buffer,10);
		TCNT1=0;
		lcd_init(LCD_DISP_ON);
		lcd_clrscr();             
		lcd_gotoxy(0,0);          
		lcd_puts("Broj obrtaja ");  
		lcd_gotoxy(0,1);
		lcd_puts(buffer);
		lcd_home();
	
}
void Timer0Init()
{
	TCCR0 |= 1<<CS02 | 1<<CS00;//1024 od preskalera
	TIMSK |= 1<<TOIE0;//OMOGUÆI OVERFLOW INTERUPT
	TIFR  |= 1<<TOV0; //FLAG ZA OVERFLOWINTERUPT
	TCNT0=0;
}
ISR(TIMER0_OVF_vect)
{
	count++;
	if (count==61)
	{
		volatile   unsigned int freq;
		const char buffer[11];
		volatile unsigned int rpm;
		
		freq=TCNT1;
		rpm=freq*5;
		utoa(rpm, buffer,10);
		TCNT1=0;
		lcd_init(LCD_DISP_ON);
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts("Broj obrtaja ");
		lcd_gotoxy(0,1);
		lcd_puts(buffer);
		lcd_home();
		count=0;
	}
	
}

void t1_init(void){
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	TIFR = 1<<TOV1;
	TIMSK = 1<<TOIE1;
}

void t2_init(void){
	TCCR2 = 0;
	//TCCR2B = 0;
	TCNT2 = 0;
	TIFR |= 1<<TOV2;
	TIMSK |= 1<<TOIE2;
}




