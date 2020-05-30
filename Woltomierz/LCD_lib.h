#define RS 0
#define RW 1
#define E 2
#define LED 3

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

void LCD_init()
{
	TWI_start();
	TWI_write(0x4e);
	LCD_write(0x33, 0);
	LCD_write(0x32, 0);
	LCD_write(0x28, 0);
	LCD_write(0x06, 0);
	LCD_write(0x0c, 0);
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
		LCD_write(napis[i++], 1);
		
	}
	TWI_stop();
}



//void TWI_init();
//
//void TWI_start();
//
//void TWI_stop(void);
//
//void TWI_write(uint8_t bajt);
//
//void LCD_write(uint8_t data, uint8_t rs);
//
//void LCD_init();
//
//void LCD_text(char *napis, uint8_t adres);