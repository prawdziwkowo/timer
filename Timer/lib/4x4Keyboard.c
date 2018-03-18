#include <avr/io.h>
#include "4x4Keyboard.h"

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
		asm volatile ("nop"); //synchronizator, trzeba odczekac jeden tak zegara pomiedzy wpisaniem danym do PORTx a przeczytaniem z PINx
		if (~PIN_K1 & (1 << K1)) return (1+n*4);
		if (~PIN_K2 & (1 << K2)) return (2+n*4);
		if (~PIN_K3 & (1 << K3)) return (3+n*4);
		if (~PIN_K4 & (1 << K4)) return (4+n*4);
	} while(n++ < 3);
	return (0);
}

signed char map_key_to_digit(uint8_t key) 
{
	switch (key) {
		case 1:
			return 1;
		case 5:
			return 2;
		case 9:
			return 3;
		case 2:
			return 4;
		case 6:
			return 5;
		case 10:
			return 6;
		case 3:
			return 7;
		case 7:
			return 8;
		case 11:
			return 9;
		case 8:
			return 0;
	}
	return -1;
}