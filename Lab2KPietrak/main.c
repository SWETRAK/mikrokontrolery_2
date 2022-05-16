/*
 * Lab2KPietrak.c
 *
 * Created: 28.04.2022 12:20:11
 * Author : Student_PL
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <time.h>


// Tablica kodow binarnych dla cyfr wyswietlanych na wyswietlaczach 7 segment
// Najstarszy bit odpowiada segmentowi a a najmlodszy kropce 
unsigned char znaki[10] = {
	0b11111100, // 0
	0b01100000, // 1
	0b11011010, // 2
	0b11110010, // 3
	0b01100110, // 4
	0b10110110, // 5
	0b10111110, // 6
	0b11100000, // 7
	0b11111110, // 8
	0b11110110, // 9
};

// Tablica kodow binarnych dla cyfr wyswietlanych na wyswietlaczach 7 segment
// Najstarszy bit odpowiada segmentowi a a najmlodszy kropce 
unsigned char znakiZajecia[4] = {
	0b11000110, // maly kwadrat w gurnej czesci wyswietlacza
	0b00111010, // maly kwadrat w dolnej czesci wyswietlacza 
	0b10011110, // litera E
	0b11110010, // Cyfra 3
};

void z1();
void z2();
void z3();
void z3a();


int main(void)
{
    /* Replace with your application code */
	
	DDRA = 0xFF; // Porta skonfigorowany jako wyjscie, sterowanie segmentami
	DDRB = 0x0F; // Portb skonfigurowany jako wyjscie na 4 mlodszych bitach, sterowanie ktory wyswietlacz jest aktywny
	srand(time(NULL));
	
    while (1) 
    {
		
		for(uint8_t i = 0; i<4; i++) { // Petla iterujaca przez wszystkie pozycje wyswietlacza 
			unsigned char znak = znakiZajecia[rand()%2]; // pseudo-losowe wybranie znaku do wyswietlenia z tablicy znakiZajecia
			for(uint8_t j = 0; j<250; j++) { // petla iterujaca 250 * 2ms = 0.5s 
				PORTB = ~_BV(i); // wybranie wyswietlacza o indeksie i
				PORTA = ~(znak); // wyswietlenie na wyswietlaczu wylosowanego znaku
				_delay_ms(2); // opuznienie w dzialaniu
			}
		}
		
    }
}

void z1() {
	
	PORTB = ~_BV(0); // wybranie wyswietlacza pierwszego od lewej 
	PORTA = ~(znaki[2]); // wyswietlenie cyfry 2, kod tej cyfry zawarty jest w tablicy znaki
	_delay_ms(2); // Odczekanie 2ms a nastepnie przelaczenie sie na kolejny wyswietlacz 
	
	PORTB = ~_BV(1); // wybranie wyswietlacza drogiego od lewej 
	PORTA = ~(znaki[0]); // wyswietlenie cyfry 0, kod tej cyfry zawarty jest w tablicy znaki
	_delay_ms(2); // Odczekanie 2ms a nastepnie przelaczenie sie na kolejny wyswietlacz 
	
	PORTB = ~_BV(2);  // wybranie wyswietlacza trzeciego od lewej 
	PORTA = ~(znaki[0]); // wyswietlenie cyfry 0, kod tej cyfry zawarty jest w tablicy znaki
	_delay_ms(2); // Odczekanie 2ms a nastepnie przelaczenie sie na kolejny wyswietlacz
	
	PORTB = ~_BV(3); // wybranie wyswietlacza czwartego od lewej 
	PORTA = ~(znaki[0]); // wyswietlenie cyfry 0, kod tej cyfry zawarty jest w tablicy znaki
	_delay_ms(2); // Odczekanie 2ms a nastepnie przelaczenie sie na kolejny wyswietlacz
	
}

void z2() {
	
	unsigned char liczby[3]; // tablica przechowujaca kolejne cyfry jednosci, dziesiatki i setki 
	
	for(uint8_t i = 2; i<143; i+=7) { // generowanie liczb z zakresu od 2 do 142 wlacznie, co 7 
		uint8_t new = 0; // zmienna pomocnicza przechowujaca wartosc do wyswietlenia po przeksztalceniach 
		uint8_t setiszero = 0; // zmienna pomocnicza przechowujaca 0 gdy setki nie sa 0 lub 1 gdy setki sa 0
		if ((uint8_t)(i/100) == 0 ) { // sprawdzanie czy setki sa zerem 
			liczby[2] = 0b00000000; // kod zgaszonego wyswietlacza 
			setiszero = 1; // ustawienie zmiennej pomocniczej na 1 
		} else {
			liczby[2] = znaki[(uint8_t)(i/100)]; // kod cyfry odpowiadajacej cyfsze setek 
		}
		new = (uint8_t)(i%100); // obliczanie nowej cyfry, obcinanie setek i pozostawienie tylko dziesiatek i jednosci
		if (setiszero == 1 && (uint8_t)(new/10) == 0) { // jesli cyfra setek byla zerem i cyfra dziesiatek jest zerem
			liczby[1] = 0b00000000; // kod zgaszonego wyswietlacza 
		} else {
			liczby[1] = znaki[(uint8_t)(new/10)]; // kod cyfry odpowiadajacej cyfsze dziesiatek
		}
		new  = (uint8_t)(new%10); // obliczanie nowej cyfry, obcinanie dziesiatek i pozostawienie tylko jednosci
		liczby[0] = znaki[new]; // dodawanie do tabliczy kodu dla cyfry jednostek 
		
		for(uint8_t j = 0; j< 83; j++) { // tablica multipleksujaca wyswietlacz, 83 * 2ms * 3 = ok 500ms = 0,5s

				PORTB = ~_BV(3); // wybranie ostatniego wyswietlacza 
				PORTA = ~(liczby[0]); // wyswietlenie cyfry jednosci
				_delay_ms(2); // Odczekanie 2ms a nastepnie przelaczenie sie na kolejny wyswietlacz
				
				PORTB = ~_BV(2); // wybranie przedostatniego wyswietlacza 
				PORTA = ~(liczby[1]); // wyswietlenie cyfry dziesiatek
				_delay_ms(2); // Odczekanie 2ms a nastepnie przelaczenie sie na kolejny wyswietlacz
				
				PORTB = ~_BV(1); // wybranie drugiego od lewej wyswietlacza 
				PORTA = ~(liczby[2]); // wyswietlenie cyfry setek
				_delay_ms(2); // Odczekanie 2ms a nastepnie przelaczenie sie na kolejny wyswietlacz

		}
	}
}

void z3() {
	unsigned char symbole[4];
	for(uint8_t i = 0; i<4; i++) {
		symbole[i] = znakiZajecia[rand()%2];
	}
	
	for(uint8_t i = 0; i<62; i++) {
			PORTB = ~_BV(0);
			PORTA = ~(symbole[0]);
			_delay_ms(2);
			
			PORTB = ~_BV(1);
			PORTA = ~(symbole[1]);
			_delay_ms(2);
			
			PORTB = ~_BV(2);
			PORTA = ~(symbole[2]);
			_delay_ms(2);
			
			PORTB = ~_BV(3);
			PORTA = ~(symbole[3]);
			_delay_ms(2);
	}
}

void z3a() {

	for(uint8_t i = 0; i<4; i++) { // Petla iterujaca przez wszystkie pozycje wyswietlacza 
		unsigned char znak = znakiZajecia[rand()%2]; // pseudo-losowe wybranie znaku do wyswietlenia z tablicy znakiZajecia
		for(uint8_t j = 0; j<250; j++) { // petla iterujaca 250 * 2ms = 0.5s 
			PORTB = ~_BV(i); // wybranie wyswietlacza o indeksie i
			PORTA = ~(znak); // wyswietlenie na wyswietlaczu wylosowanego znaku
			_delay_ms(2); // opuznienie w dzialaniu
		}
	}
}