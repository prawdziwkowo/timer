/*
 * memory.c
 *
 * Created: 18.03.2018 13:23:11
 *  Author: gpraw
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>

#include "memory.h"

EEMEM uint16_t last_selected = DEFAULT_MEMORY_VALUE;

EEMEM uint16_t memory_times[MEMORY_SIZE];

uint16_t get_last_selected_time()
{
	uint16_t value = eeprom_read_word(&last_selected);
	if (value == 0xFFFF) {
		return DEFAULT_MEMORY_VALUE;
	}
	return value;
}

void set_last_selected_time (uint16_t seconds)
{
	eeprom_write_word(&last_selected, seconds);
}

uint16_t get_memory_time_by_key(uint8_t key)
{
	uint16_t value = eeprom_read_word(&memory_times[key]);
	if (value == 0xFFFF) {
		return DEFAULT_MEMORY_VALUE;
	}
	return value;
}

void set_memory_time_by_key(uint8_t key, uint16_t seconds)
{
	eeprom_write_word(&memory_times[key], seconds);
}

uint16_t* get_all_memory_times()
{
	static uint16_t ret[MEMORY_SIZE];
	uint8_t i;
	for (i = 0; i < MEMORY_SIZE; i++) {
		ret[i] = get_memory_time_by_key(i);
	}
	return ret;
}
