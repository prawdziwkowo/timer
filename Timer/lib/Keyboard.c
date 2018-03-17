#include <avr/io.h>
#include "Keyboard.h"

//Inicjalizacja obs³ugi klawiarury.
void init_4x4_kbr (void)
{
	//Wiersze jako wyjœcia, kolumny jako wejœcia:
	cbi(DDR_K1,K1);
	cbi(DDR_K2,K2);
	cbi(DDR_K3,K3);
	cbi(DDR_K4,K4);
	sbi(DDR_W1,W1);
	sbi(DDR_W2,W2);
	sbi(DDR_W3,W3);
	sbi(DDR_W4,W4);
	
	//Podci¹ganie dla wejœæ:
	sbi(PORT_K1,K1);
	sbi(PORT_K2,K2);
	sbi(PORT_K3,K3);
	sbi(PORT_K4,K4);
}

// Aktywacja wiersza.
void active_row (char x)
{
	sbi(PORT_W1,W1);
	sbi(PORT_W2,W2);
	sbi(PORT_W3,W3);
	sbi(PORT_W4,W4);
	switch (x)
	{
		case 0:
		cbi(PORT_W1,W1);
		break;
		case 1:
		cbi(PORT_W2,W2);
		break;
		case 2:
		cbi(PORT_W3,W3);
		break;
		case 3:
		cbi(PORT_W4,W4);
		break;
	}
}

// Sprawdzenie przycisków.
uint8_t check_kbr (void)
{
	char n = 0;
	do
	{
		active_row (n);
		//TODO: zamiast delay zrobiæ jednefo nopa i zobaczyæ czy dzia³a
		asm volatile ("nop"); //synchronizator, trzeba odczekac jeden tak zegara pomiedzy wpisaniem danym do PORTx a przeczytaniem z PINx
		if (~PIN_K1 & (1 << K1)) return (1+n*4);
		if (~PIN_K2 & (1 << K2)) return (2+n*4);
		if (~PIN_K3 & (1 << K3)) return (3+n*4);
		if (~PIN_K4 & (1 << K4)) return (4+n*4);
	} while(n++ < 3);
	return (0);
}