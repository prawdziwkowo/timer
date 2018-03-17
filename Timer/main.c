#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>


#include "lib/Keyboard.h"
#include "lib/uart.h"
#include "lib/draw.h"



#define UART_BAUD_RATE      9600



volatile uint16_t seconds;
volatile uint8_t clocks;
volatile uint8_t redraw;


void inline timer0_init()
{
	//overflow 250hz
	TCCR0B |= (1<<CS02); //preskaler 256
	OCR0B = 0xFA;
	TIMSK0 |= (1<<TOIE0); //przerwanie ov
}


int main(void)
{
    /* Replace with your application code */
	initDisplay();
	init_4x4_kbr ();
	timer0_init();
	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei();
	//draw();
	
    while (1) 
    {
		if (redraw) {
			draw(seconds);
			redraw = 0;
		}
		//sign = check_kbr();
		//itoa( sign, buffer, 10);
		//uart_puts(buffer);
		//_delay_ms(100);
    }
}



ISR(TIMER0_OVF_vect) 
{
	char buffer[10];
	clocks ++;
	if (clocks > 250) {
		seconds ++;
		clocks = 0;
		redraw = 1;
		itoa( seconds, buffer, 10);
		uart_puts(buffer);
		uart_puts(" ");
	}
}
