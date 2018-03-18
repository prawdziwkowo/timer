/*
 * memory.h
 *
 * Created: 18.03.2018 13:22:58
 *  Author: gpraw
 */ 


#ifndef MEMORY_H_
#define MEMORY_H_

#include <avr/io.h>

#define MEMORY_SIZE 4
#define DEFAULT_MEMORY_VALUE 120


uint16_t get_last_selected_time();
void set_last_selected_time (uint16_t value);
uint16_t get_memory_time_by_key(uint8_t key);
void set_memory_time_by_key(uint8_t key, uint16_t value);

uint16_t* get_all_memory_times();

#endif /* MEMORY_H_ */