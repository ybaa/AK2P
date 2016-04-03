/*
 * GccApplication1.c
 *
 * Created: 31.03.2016 16:01:36
 * Author : Martyna
 */ 

#include <avr/io.h>
#include <util/delay.h>

void mk_delay_ms(uint16_t ms);
uint8_t hour=0;
int hoursTab[] = {0,1,2,3,4,5,6,7,64,65,66,67,68,69,70,71,128,129,130,131,132,133,134,135};
	
int main(void)
{
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRD = 0xFF;
	PORTD = 0x00;
	//DDRB = 0xFF; //obsluga guzika nie dzialala, bo kazdy pin byl ustawiony na output
	DDRB = 0b11001111; 
	PORTB = 0x00;
	PORTB |= (1<<PB4);
	PORTB |= (1<<PB5);

	uint8_t sec = 0, min = 0;//hour = 0;
	

    /* Replace with your application code */
    while (1) {
		
		PORTB = hoursTab[hour];
		PORTD = min;
		
		for(sec = 0; sec < 10; sec++){

			PORTC = sec;
				
			if(!(PINB & (1<<PB5)))
			{
				sec=0;
				hour++;
				PORTB = hoursTab[hour];
				PORTC = sec;
			}
			
			if(!(PINB & (1<<PB4)))
			{
				sec=0;
				min++;
				PORTD = min;
				PORTC = sec;
			}
			
			_delay_ms(1000);
			//mk_delay_ms(1000);
			 		
		}
		
		min++;
		if(min == 10){
			hour++;
			PORTB = hour;
			min = 0;
		}
		
	
		if(hoursTab[hour] > 135){
			sec = 0; 
			min = 0;
			hour = 0;
		}
		
		
    }
}



/*void mk_delay_ms(uint16_t ms)
{
	while(ms=-500)
	{
		_delay_ms(500);
		
		if(!(PINB & (1<<PB4)))
		{
			hour++;
			PORTB = hoursTab[hour];
		}
	}
}*/
