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
    TWI_init(); // wywo³anie funkcji inicjalizuj¹cych interfejs I2C
    LCD_init(); // wywo³anie funkcji inicjalizuj¹cych wyœwietlacz LCD za poœrednictwem interfejsu I2C
	uint16_t wynik;
	ADC_init(); // inicjalizacja przetwornika AC

    while (1) 
    {	
		wynik = ADC_CH_1(); // zwrócenie zmierzonej wartoœci na kanale PA1 do zmiennej wynik
		LCD_V_disp(wynik,0x80); // wyœwietlenie napiêcia z mniejsz¹ podzia³k¹ (16 stanów) na wyœwietlaczu LCD

		wynik = ADC_CH_2(); // zwrócenie zmierzonej wartoœci na kanale PA2 do zmiennej wynik
		LCD_V_prec_disp(wynik,0xc0); // wyœwietlenie napiêcia z wiêksz¹ podzia³k¹ (80 stanów) na wyœwietlaczu LCD
		
    }
}

