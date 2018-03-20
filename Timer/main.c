/*
Klawiatura na pinach PD2-PD7, PB0, PB1 (2-9 arduino)
Wyúwietlacz na i2c
Przekaünik na pinie PC3 (A3 arduinbo)
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>



#include "lib/4x4Keyboard.h"
#include "lib/uart.h"
#include "lib/draw.h"
#include "lib/memory.h"



#define UART_BAUD_RATE      9600



volatile uint16_t seconds;
volatile uint8_t clocks;
volatile uint8_t redraw = 1;
volatile uint8_t screen = SCREEN_START;

volatile uint8_t selected_sign = 0;
volatile uint8_t prev_sign;


struct displayTime entered_time;

void run_action();

void inline timer0_init()
{
	//overflow 250hz
	TCCR0B |= (1<<CS02); //preskaler 256
	OCR0B = 0xFA;
	TIMSK0 |= (1<<TOIE0); //przerwanie ov
}

void inline init_pins()
{
	DDRC |= (1<<3);
	PORTC |= (1<<3);
}

void enable_counting()
{
	PORTC &= ~(1<<3);
}

void disable_counting()
{
	PORTC |= (1<<3);
}

uint8_t counting_enabled()
{
	return !(PINC & (1<<3));
}

int main(void)
{
	initDisplay();
	init_4x4_kbr ();
	timer0_init();
	init_pins();
	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei();
	
	seconds = get_last_selected_time();
	
    while (1) 
    {
		run_action();
		if (redraw) {
			draw(screen, seconds, entered_time);	
			redraw = 0;
			selected_sign = 0;
		}
    }
}

void run_action_screen_counting()
{
	switch(selected_sign) {
		case 12: //#
			disable_counting();
			screen = SCREEN_PAUSED;
			redraw = 1;
			break;
	}
}

void run_action_screen_choosed_time()
{
	switch(selected_sign) {
		case 12: //#
			enable_counting();
			screen = SCREEN_COUNTING;
			redraw = 1;
			break;
		case 4://*
			screen = SCREEN_MEMORY_SAVE;
			redraw = 1;
			break;
	}
}

struct displayTime get_empty_time_struct()
{
	struct displayTime ret;
	ret.hours[0] = ret.minutesH[0] = ret.minutesL[0] = ret.secondsH[0] = ret.secondsL[0] = '_';
	ret.hours[1] = ret.minutesH[1] = ret.minutesL[1] = ret.secondsH[1] = ret.secondsL[1] = '\0';
	return ret;
}

void run_action_screen_choosing_time()
{
	static uint8_t digits_count = 0;
	static uint16_t new_seconds = 0;
	signed char digit;
	digit = map_key_to_digit(selected_sign);
	
	if (selected_sign == 12) { //#
		screen = SCREEN_START;
		new_seconds = 0;
		digits_count = 0;
		redraw = 1;
	}
	
	if (digit >= 0) {
		screen = SCREEN_CHOOSING_TIME;
		redraw = 1;
		if (!digits_count) {
			entered_time = get_empty_time_struct();
		}
		
		digits_count ++;
		
		switch(digits_count) {
			case 1:
				itoa(digit, entered_time.hours, 10);
				new_seconds += digit * 3600;
				break;
			case 2:
				if (digit < 6) {
					itoa(digit, entered_time.minutesH, 10);
					new_seconds += digit * 600;
				} else {
					digits_count--;
				}
				break;
			case 3:
				itoa(digit, entered_time.minutesL, 10);
				new_seconds += digit * 60;
				break;
			case 4:
				if (digit < 6) {
					itoa(digit, entered_time.secondsH, 10);
					new_seconds += digit * 10;
				} else {
					digits_count--;
				}
				break;
			case 5:
				new_seconds += digit;
				redraw = 1;
				if (new_seconds > 0) {
					itoa(digit, entered_time.secondsL, 10);
					digits_count = 0;
					set_last_selected_time(new_seconds);
					seconds = new_seconds;
					screen = SCREEN_CHOOSED_TIME;
					new_seconds = 0;
				} else {
					digits_count--;
				}
				
				break;
		}
	}
}

void run_action_screen_start()
{
	signed char digit;
	
	switch(selected_sign) {
		case 12: //#
			enable_counting();
			screen = SCREEN_COUNTING;
			redraw = 1;
			break;
		case 4://*
			screen = SCREEN_MEMORY_SELECT;
			redraw = 1;
			break;
	}
	
	digit = map_key_to_digit(selected_sign);
	if (digit >= 0) {
		screen = SCREEN_CHOOSING_TIME;
		redraw = 1;
		run_action_screen_choosing_time();
	}
}




void run_action_screen_paused()
{
	switch(selected_sign) {
		case 12: //#
			enable_counting();
			screen = SCREEN_COUNTING;
			redraw = 1;
			break;
		case 4: //*
			seconds = get_last_selected_time();
			screen = SCREEN_START;
			redraw = 1;
			break;
	}
}

void run_action_screen_memory_select()
{
	if (selected_sign == 12) {//#
		screen = SCREEN_START;
		redraw = 1;
	} else if (selected_sign >= 13 && selected_sign <= 16) {
		seconds = get_memory_time_by_key(selected_sign - 13);
		set_last_selected_time(seconds);
		screen = SCREEN_START;
		redraw = 1;
	}
}

void run_action_screen_memory_save() 
{
	if (selected_sign == 12) {//#
		screen = SCREEN_START;
		redraw = 1;
	} else if (selected_sign >= 13 && selected_sign <= 16) {
		set_memory_time_by_key(selected_sign - 13, seconds);
		screen = SCREEN_START;
		redraw = 1;
	}
}

void run_action_screen_end() {
	switch(selected_sign) {
		case 4: //*
			seconds = get_last_selected_time();
			screen = SCREEN_START;
			redraw = 1;
			break;
	}
}

void run_action()
{
	switch (screen)
	{
		case SCREEN_START:
			run_action_screen_start();
			break;
		case SCREEN_COUNTING:
			run_action_screen_counting();
			break;
		case SCREEN_PAUSED:
			run_action_screen_paused();
			break;
		case SCREEN_MEMORY_SELECT:
			run_action_screen_memory_select();
			break;
		case SCREEN_END:
			run_action_screen_end();
			break;
		case SCREEN_CHOOSING_TIME:
			run_action_screen_choosing_time();
			break;
		case SCREEN_CHOOSED_TIME:
			run_action_screen_choosed_time();
			break;
		case SCREEN_MEMORY_SAVE:
			run_action_screen_memory_save();
			break;
	}
}



ISR(TIMER0_OVF_vect) 
{	
	uint8_t sign;
	if (counting_enabled()) {
		clocks ++;
		if (clocks > 250) {
			seconds --;
			clocks = 0;
			redraw = 1;
			if (seconds == 0) {
				disable_counting();
				screen = SCREEN_END;
			}
		}
	}
	
	sign = check_kbr();
	if (sign != prev_sign) {
		selected_sign = sign;
		prev_sign = sign;
	} else {
		selected_sign = 0;
	}
}
