
//Funkcja check_kbr zwraca wartoœæ wciskanego przycisku klawiatury 4x4 1-16 lub 0 gdy ¿aden nie wybrany.


/*
--------------------
|  ||K1|K2|K3|K4|
====================
|W1|| 1| 2| 3| 4|
--------------------
|W2|| 5| 6| 7| 8|
--------------------
|W3|| 9|10|11|12|
--------------------
|W4||13|14|15|16|
--------------------
*/
 
//Definicja kolumn:
#define PORT_K1 PORTD
#define DDR_K1 DDRD
#define PIN_K1 PIND
#define K1 2
#define PORT_K2 PORTD
#define DDR_K2 DDRD
#define PIN_K2 PIND
#define K2 3
#define PORT_K3 PORTD
#define DDR_K3 DDRD
#define PIN_K3 PIND
#define K3 4
#define PORT_K4 PORTD
#define DDR_K4 DDRD
#define PIN_K4 PIND
#define K4 5

//Definicja wierszy:
#define PORT_W1 PORTD
#define DDR_W1 DDRD
#define W1 6
#define PORT_W2 PORTD
#define DDR_W2 DDRD
#define W2 7
#define PORT_W3 PORTB
#define DDR_W3 DDRB
#define W3 0
#define PORT_W4 PORTB
#define DDR_W4 DDRB
#define W4 1

// Pozosta?e definicje:
#define sbi(port, bit) (port) |=(1 << (bit)) 
#define cbi(port, bit) (port) &=~(1 << (bit))
 
//Inicjalizacja obs?ugi klawiarury.
void init_4x4_kbr (void);

// Aktywacja wiersza. 
void active_row (char x);

// Sprawdzenie przycisk¡w.
uint8_t check_kbr (void);

signed char map_key_to_digit(uint8_t key);
 
/*


int main(void)
{
	init_4x4_kbr ();
 	for(;;)
 	{
		print (check_kbr ());			
 	}
}
*/