/*
 * Woltomierz.c
 *
 * Created: 30.05.2020 13:05:06
 * Author : Jakub Nowak
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "LCD_lib.h"

int main(void)
{
    TWI_init(); // wywołanie funkcji inicjalizujących interfejs I2C
    LCD_init(); // wywołanie funkcji inicjalizujących wyświetlacz LCD za pośrednictwem interfejsu I2C
	uint16_t wynik;
	ADC_init(); // inicjalizacja przetwornika AC

    while (1) 
    {	
		wynik = ADC_CH_1(); // zwrócenie zmierzonej wartości na kanale PA1 do zmiennej wynik
		LCD_V_disp(wynik,0x80); // wyświetlenie napięcia z mniejszą podziałką (16 stanów) na wyświetlaczu LCD

		wynik = ADC_CH_2(); // zwrócenie zmierzonej wartości na kanale PA2 do zmiennej wynik
		LCD_V_prec_disp(wynik,0xc0); // wyświetlenie napięcia z większą podziałką (80 stanów) na wyświetlaczu LCD
		
    }
}

