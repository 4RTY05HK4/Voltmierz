/*
	Created: 30.05.2020 13:05:06
	Author : Jakub Nowak
	Biblioteka do wy�wietlania napi�cia zmierzonego za pomoc� wbudowanego w mikrokontroler Atmega32 przetwornika AC.
	-------------------------------------------------------!!!UWAGA!!!----------------------------------------------------
	|                             MAKSYMALNE NAPI�CIE JAKIE MO�E ZOSTAC ZMIERZONE TO 2.55V                               |
	----------------------------------------------------------------------------------------------------------------------
	Wy�wietlacz nale�y pod��czy� do kontrolera poprzez interfejs I2C. Porty SCL i SDA nale�y pod��czy� na porty kontrolera PC0 i PC1.
	SCL => PC0
	SDA => PC1
	Do wyj�� analogowych przetwornika AC nale�y pod��czy� potencjometry. U�yte wej�cia to PA1 i PA2.
	Po odpowiednim pod��czeniu mo�na w g��wnym programie wywo�a� funkcje demo(). W�wczas odczyt z pierwszego kana�u wy�wietlony zostanie na g�rnej linii
	wy�wietlacza z dok�adno�ci� 0.16V, a odczyt z drugiego kana�u wy�wietlony zostanie na drugiej linii wy�wietlacza z dok�adno�ci� 0.03V;

*/

#define RS 0
#define RW 1
#define E 2
#define LED 3

uint8_t bar_1[]={0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10}; //znak w�asny pojedy�czej belki
uint8_t bar_2[]={0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18}; //znak w�asny podw�jnej belki
uint8_t bar_3[]={0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c}; //znak w�asny potr�jnej belki
uint8_t bar_4[]={0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e}; //znak w�asny poczw�rnej belki
uint8_t bar_5[]={0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f}; //znak w�asny pe�nego prostok�ta

void TWI_init()
{
	TWBR = 16;
	TWCR = (1<<TWEA);
}

void TWI_start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}


void TWI_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while ((TWCR & (1<<TWSTO)));

}

void TWI_write(uint8_t bajt)
{
	TWDR = bajt;
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));
}

void LCD_write(uint8_t data, uint8_t rs)
{
	uint8_t co = 0;

	co |= (rs<<RS)|(1<<E)|(1<<LED)|(data & 0xf0);
	TWI_write(co);
	co &= ~(1<<E);
	TWI_write(co);
	co = 0;
	co |= (rs<<RS)|(1<<E)|(1<<LED)|((data<<4) & 0xf0);
	TWI_write(co);
	co &= ~(1<<E);
	TWI_write(co);

}

void LCD_bars(uint8_t adres, uint8_t *tab)
{
	for (uint8_t i=0;i<8;i++)
	{
		LCD_write(0x40+adres*8+i,0);
		LCD_write(tab[i],1);
	}
}

void LCD_init()
{
	TWI_start();
	TWI_write(0x4e);
	LCD_write(0x33, 0);
	LCD_write(0x32, 0);
	LCD_write(0x28, 0);
	LCD_write(0x06, 0);
	LCD_write(0x0c, 0);
	//dodanie w�asnych znak�w przed czyszczeniem ekranu
	LCD_bars(1,bar_1);
	LCD_bars(2,bar_2);
	LCD_bars(3,bar_3);
	LCD_bars(4,bar_4);
	LCD_bars(5,bar_5);
	LCD_write(0x01, 0);
	TWI_stop();
	_delay_ms(2);
}

void LCD_text(char *napis, uint8_t adres) 
{
	uint8_t i=0;
	TWI_start();
	TWI_write(0x4e);
	LCD_write(adres, 0);
	while(napis[i]!='\0')
	{
		if (napis[i]=='1') napis[i]=1;
		if (napis[i]=='2') napis[i]=2;
		if (napis[i]=='3') napis[i]=3;
		if (napis[i]=='4') napis[i]=4;
		if (napis[i]=='5') napis[i]=5;
		LCD_write(napis[i++], 1);
		
	}
	TWI_stop();
}

void ADC_init(void)
{
	ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<MUX0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2);
}

uint16_t ADC_CH_1() //funkcja mierz�ca napi�cie na wej�ciu pierwszym (PA1), zwracaj�ca warto�� napi�cia do zmiennej
{
	ADMUX = (1<<REFS1)|(1<<REFS0)|(1<<MUX0)|(0<<MUX1);
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	uint16_t wynik = ADC;
	wynik = (wynik>>2);
	return wynik;
}

uint16_t ADC_CH_2() //funkcja mierz�ca napi�cie na wej�ciu drugim (PA2), zwracaj�ca warto�� napi�cia do zmiennej
{
	ADMUX = (1<<REFS1)|(1<<REFS0)|(0<<MUX0)|(1<<MUX1);
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	uint16_t wynik = ADC;
	wynik = (wynik>>2);
	return wynik;
}

void LCD_V_disp(uint16_t wynik, uint8_t adres) //funkcja wy�wietlaj�ca prostok�t na ka�de 0.16V, w programie g��wnym nale�y jej przekaza� zmierzone napi�cie oraz adres linii wy�wietlacza
{
	uint8_t wart=0;
	wart = wynik / 16;
	if(wart == 0) 
	{
	LCD_text(" ",adres);
	}
	else
	{
		for(uint8_t i=0; i<=wart; i++)
		{
			LCD_text("5",adres);
			adres++;
		}
	}
	for(uint8_t i=wart; i<=16; i++)
	{
		LCD_text(" ",adres);
		adres++;
	}
}

void LCD_V_prec_disp(uint16_t wynik, uint8_t adres) //funkcja wy�wietlaj�ca belk� na ka�de 0.03V, , w programie g��wnym nale�y jej przekaza� zmierzone napi�cie oraz adres linii wy�wietlacza
{
	uint8_t wart = 0;
	wart = wynik/3;
	uint8_t iter = wart/5;	
	uint8_t rest = wart%5;

	if(wart == 0)
	{
		LCD_text(" ",adres);
	}
	else
	{
		if(rest == 1 && iter <= 1) LCD_text("1",adres);
		else if(rest == 2 && iter <= 1) LCD_text("2",adres);
		else if(rest == 3 && iter <= 1) LCD_text("3",adres);
		else if(rest == 4 && iter <= 1) LCD_text("4",adres);
		for(uint8_t i = 1; i <= iter; i++)
		{
			LCD_text("5",adres);
			adres++;
		}
	}
	if(rest == 1) LCD_text("1",adres);
	else if(rest == 2) LCD_text("2",adres);
	else if(rest == 3) LCD_text("3",adres);
	else if(rest == 4) LCD_text("4",adres);
	adres++;
	for(uint8_t i=iter; i<=16; i++)
	{
		LCD_text(" ",adres);
		adres++;
	}
}

void demo()
{
    TWI_init(); 
    LCD_init(); 
    uint16_t wynik;
    ADC_init(); 
    while (1)
    {
	    wynik = ADC_CH_1();
	    LCD_V_disp(wynik,0x80); 
	    wynik = ADC_CH_2(); 
	    LCD_V_prec_disp(wynik,0xc0); 
    }
}
