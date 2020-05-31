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
    TWI_init(); // wywo�anie funkcji inicjalizuj�cych interfejs I2C
    LCD_init(); // wywo�anie funkcji inicjalizuj�cych wy�wietlacz LCD za po�rednictwem interfejsu I2C
	uint16_t wynik;
	ADC_init(); // inicjalizacja przetwornika AC

    while (1) 
    {	
		wynik = ADC_CH_1(); // zwr�cenie zmierzonej warto�ci na kanale PA1 do zmiennej wynik
		LCD_V_disp(wynik,0x80); // wy�wietlenie napi�cia z mniejsz� podzia�k� (16 stan�w) na wy�wietlaczu LCD

		wynik = ADC_CH_2(); // zwr�cenie zmierzonej warto�ci na kanale PA2 do zmiennej wynik
		LCD_V_prec_disp(wynik,0xc0); // wy�wietlenie napi�cia z wi�ksz� podzia�k� (80 stan�w) na wy�wietlaczu LCD
		
    }
}

