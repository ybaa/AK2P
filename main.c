/*
 * GccApplication5.c
 *
 * Created: 22.04.2016 00:36:04
 * Author : Martyna;
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

//----------------------------zmienne------------------------------------------------------------------------;
uint8_t hour = 0, sec = 0, min = 0;
uint8_t wasThere = 0, wasThereMin = 0, helpHour = 0, setAlarm = 0;
uint8_t secA = 0, minA = 512, hourA = 512, blink = 1, isSet = 0, ringing = 0;		//A-alarm;
int hoursTab[] = {0,1,2,3,4,5,6,7,64,65,66,67,68,69,70,71,128,129,130,131,132,133,134,135};


void setAlarmM(){
	
	while(!(bit_is_set(PINB, PINB4))){
	if(blink == 1){
		minA = 0;
		PORTC = 0;
		PORTB = 0b0111000;
		PORTD = 0b00111111;
		_delay_ms(500);
	}
	blink = 0;
	
	secA = 0;						// do czasu az wcisniety jest przycisk, sekudny sie zatrzymuja, a minuty rosna co sekunda;
	minA++;
	if(minA == 60)
	minA = 0;
	PORTC = minA;
	sec++;
	_delay_ms(962);
	}
	
	isSet = 1;
	
	
	setAlarm = 0;

}

void setAlarmH(){
	while(!(bit_is_set(PINB, PINB5))){
		if(blink == 1){
			hourA = 0;
			PORTC = 0;
			PORTB = 0b0111000;
			PORTD = 0b00111111;
			_delay_ms(500);
		}
		blink = 0;
		sec++;
		secA = 0;						// do czasu az wcisniety jest przycisk, sekudny sie zatrzymuja, a minuty rosna co sekunda;
		hourA++;
		if(hourA == 24)
		hourA = 0;
		PORTC = hourA;
		_delay_ms(962);
	}
	
	isSet = 1;
	min++;		//naprawa przestawien, ktorych narazie nie moge znalexc;
	hour--;
	setAlarm = 0;
}

void ring(){
	PORTD = 0b10000000;
	//sec++;
	_delay_ms(962);
	//ringing = 1;
	if(!(bit_is_set(PINB, PINB4))){		//alarm wylacza sie guzikiem od minut;
		PORTD = min;
		setAlarm = 0;
		ringing = 0;
		min--;
		_delay_ms(962);
	}
}



int main(void){
	
//------------------------porty, piny, ddry------------------------------------------------------------------;
DDRC = 0xFF;					// wszystkie piny sš outputami -> diody;
PORTC = 0x00;

DDRD = 0xFF;					//wszystkie piny sš outputami -> diody;t
PORTD = 0x00;

DDRB = 0b11000111;				//2 piny to przyciski, wiec sa ustawione na inputy;
PORTB = 0b00111000;
	
	
	
    _delay_ms(3000);
	
	
    while (1) {
		
		//--------------------------------petla dla sekund------------------------------------------------------------------;
		for ( sec = 0; sec < 60; sec ++){
			PORTC = sec;
			PORTD = min;
			
			
			
			
			if(!(bit_is_set(PINB, PINB3))){
				
				
				setAlarm = 1;
				//PORTD = 0b00100001;	//tu by³ test czy tu wgl wchodzi, wchodzi wiec niech nie miga tymi diodami 100x
				PORTD = 0b00111111;
				_delay_ms(3000);	//niech poczeka z 2 sekundy zeby kliknaac nastpeny guzik;
				//PORTD = min;
			}
			
			
			//---------------------------------ustawianie minut-------------------------------------------------------------;
			//while(!(PINB & 0x10)){
			while(!(bit_is_set(PINB, PINB4))){
				
				while(setAlarm == 1){
					setAlarmM();
					ringing = 1;
					if(isSet == 1){
						PORTB = hoursTab[hour];
						min--;
						PORTD = min;
						PORTC = 0;
						blink = 1;
						break;
					}
				}
				
				
				
				if(wasThereMin == 0){
					PORTC = 255;
					PORTD = 0;
					_delay_ms(500);
				}
				wasThereMin = 1;
				PORTB = 0b00010000;				//atmega nie ogarnia bez tego przy probie ustawiania godzin po minutach gdzie ma outpty a gdzie inputy...;
				sec = 0;						// do czasu az wcisniety jest przycisk, sekudny sie zatrzymuja, a minuty rosna co sekunda;
				min++;							
				if(min == 60)
					min = 0;
				PORTC = min;
				_delay_ms(962);
			}
			
			if(wasThereMin == 1){
				PORTD = min;
				wasThereMin = 0;
			}
			
			PORTB = hoursTab[hour];				//po ustawieniu mu w petli wyzej gdzie ma outputy trzeba wrocic do ustawionej godziny;
			
			
			
			
			
		
			//--------------------------------ustawianie godzin--------------------------------------------------------------;
		
		
				while(!(bit_is_set(PINB, PINB5))){			//warunek dziala -> sukces;
					
					while(setAlarm == 1){
						setAlarmH();
						ringing = 1;
						if(isSet == 1){
							PORTB = hoursTab[hour];
							min--;
							PORTD = min;
							PORTC = 0;
							blink = 1;
							break;
						}
					}
					
					
					if(wasThere == 0){
						PORTC = 255;						//niech zamiga wszystkimi diodami, jako ze jest w trybie ustawiania godzin;
						_delay_ms(500);
						PORTB = 0b00110000;					//trzeba to ustawic, zeby atmega ogarnela, ze gdzie ma outputy a gdzie inputy;
					}
					
					wasThere = 1;							//godziny beda sie pogladowo ustawiac na sekundach, a nastepnie przenosic w miejsce diod dla godzin;
				sec = 0;
				hour++;
				if(hour == 24)
				hour = 0;
				PORTC = hour;
				_delay_ms(962);
				
				
			}
		
			if(wasThere == 1){								//porzadek, przeniesienie wyswietlania godizny na poprawne miejsce;
			PORTB = hoursTab[hour];
			wasThere = 0;
			}
			
			
			while(min == minA && ringing == 1 && hour == hourA){
				ring();
			}
			
			
			_delay_ms(962);
		}
		
		
		//inkrementuje godzine, kiedy ma 60min;
		min++;
		if(min == 60){
			hour++;
			min = 0;
			sec = 0;
			PORTB = hoursTab[hour];
		}
		
		//polnoc, a wiec wyzerowanie;
		if(hoursTab[hour] > 135){
			min = 0;
			hour = 0;
			sec = 0;
			PORTB = hoursTab[hour];
		}
		
		PORTD = min;

	
    }

}