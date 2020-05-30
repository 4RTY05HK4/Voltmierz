/*
 * Woltomierz.c
 *
 * Created: 30.05.2020 13:05:06
 * Author : Jakub
 */ 


#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "LCD_lib.h"

void ADC_init(void)
{
	ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<MUX0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2);
}

void BCD_ADC(uint16_t wynik, uint8_t *tab)
{
	tab[0] = wynik / 100;
	tab[1] = (wynik % 100) / 10;
	tab[2] = (wynik % 100) % 10;
}

void LCD_V_disp(uint16_t wynik, uint8_t adres)
{
	uint8_t wart=0;
	wart = wynik / 16;
	for(uint8_t i=0; i<=wart; i++)
	{
		LCD_text("0",adres);
		adres++;
	}
	for(uint8_t i=wart; i<=16; i++)
	{
		LCD_text(".",adres);
		adres++;
	}
}

int main(void)
{
    TWI_init();
    LCD_init();
	uint16_t wynik;
	uint8_t LCD[3];
	char value[] = "1,23V";
	ADC_init();
	//ADCSRA |= (1<<ADSC);
	//while (ADCSRA & (1<<ADSC));
	//wynik = ADC;
	//wynik = (wynik>>2);
	//BCD_ADC(wynik, LCD);
	//value[0] = LCD[0]+'0';
	//value[2] = LCD[1]+'0';
	//value[3] = LCD[2]+'0';
	//LCD_text(value,0x80);
	//////uint8_t adres = 0x80;
	//////for(uint8_t i=0; i<6; i++)
	//////{
		//////LCD_text("0",adres);
		//////adres++;
	//////}

    while (1) 
    {	
		ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<MUX0);
		ADCSRA |= (1<<ADSC);
		while (ADCSRA & (1<<ADSC));
		wynik = ADC;
		wynik = (wynik>>2);
		//BCD_ADC(wynik, LCD);
		//value[0] = LCD[0]+'0';
		//value[2] = LCD[1]+'0';
		//value[3] = LCD[2]+'0';
		//LCD_text(value,0x80);
		LCD_V_disp(wynik,0x80);
		ADMUX = (1<<REFS1)|(1<<REFS0)|(0<<MUX0)|(1<<MUX1);
		ADCSRA |= (1<<ADSC);
		while (ADCSRA & (1<<ADSC));
		wynik = ADC;
		wynik = (wynik>>2);
		//BCD_ADC(wynik, LCD);
		//value[0] = LCD[0]+'0';
		//value[2] = LCD[1]+'0';
		//value[3] = LCD[2]+'0';
		//LCD_text(value,0xc0);
		LCD_V_disp(wynik,0xc0);
    }
}

