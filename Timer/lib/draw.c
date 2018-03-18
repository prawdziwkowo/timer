/*
 * draw.c
 *
 * Created: 17.03.2018 19:45:12
 *  Author: gpraw
 */

#include "draw.h"
#include "U8gLib/u8g.h"
#include "memory.h"

#include <avr/io.h>
#include <stdlib.h>

u8g_t u8g;

const char PROGMEM text_load[] = "[*]load";
const char PROGMEM text_save[] = "[*]save";
const char PROGMEM text_start[] = "[#]start";
const char PROGMEM text_stop[] = "[#]stop";
const char PROGMEM text_exit[] = "[#]exit";
const char PROGMEM text_reset[] = "[*]reset";
const char PROGMEM text_memory_select[] = "[A-D]sel";
const char PROGMEM text_empty[] = "";
const char PROGMEM text_entered_time[] = "Entered time:";

//TODO: trzeba dodaæ obs³uge typów wyœwietlanego ekranu (main, memory i tp)
//TODO: dodaæ obs³ugê aktualnie przycisniêtego przycisku (chyba ???)
void draw(uint8_t screen, uint16_t seconds, struct displayTime time)
{
	u8g_FirstPage(&u8g);
	do
	{
		if (screen == SCREEN_START || screen == SCREEN_COUNTING || screen == SCREEN_PAUSED || screen == SCREEN_END || screen == SCREEN_CHOOSED_TIME) {
			drawTime(secondsToDisplay(seconds));
		}
		
		if (screen == SCREEN_MEMORY_SAVE || screen == SCREEN_MEMORY_SELECT) {
			drawMemoryScreen();
		}
		
		if (screen == SCREEN_MEMORY_SAVE) {
			drawEnteredTime(seconds);
		}
		
		if (screen == SCREEN_CHOOSING_TIME) {
			drawTime(time);
		}
		
		drawMainMenuByScreen(screen);
	} while ( u8g_NextPage(&u8g) );
}


void initDisplay()
{
	u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_NONE|U8G_I2C_OPT_FAST);
	u8g_FirstPage(&u8g);
	do
	{		
	} while ( u8g_NextPage(&u8g) );
}

void drawEnteredTime(uint16_t seconds)
{
	u8g_SetFont(&u8g, SMALL_FONT);
	char buffer[15];
	strcpy_P(buffer, text_entered_time);
	u8g_DrawStr(&u8g, 2, 38, buffer);
	
	char * str;
	str = secondsToChar(seconds);
	u8g_DrawStr(&u8g, 83, 38, str);
	free(str);
	
	
	u8g_DrawFrame(&u8g, 0, 28, 128, 14);
}

void drawMemoryScreen()
{
	uint16_t *times = get_all_memory_times();
	uint8_t i;
	char letter[2];
	letter[1] = '\0';
	
	char * str;
	
	u8g_SetFont(&u8g, SMALL_FONT);
	for (i = 0; i < MEMORY_SIZE; i++) {
		switch (i) {
			case 0:
				letter[0] = 'A';
				break;
			case 1:
				letter[0] = 'B';
				break;
			case 2:
				letter[0] = 'C';
				break;
			case 3:
				letter[0] = 'D';
				break;
		}
		u8g_DrawStr(&u8g, (i % 2) * 68, (i / 2 + 1) * 12, letter);
		str = secondsToChar(times[i]);
		u8g_DrawStr(&u8g, (i % 2) * 68 + 15, (i / 2 + 1) * 12, str);
		free(str);
	}
}

void drawTime(struct displayTime time)
{
	u8g_SetFont(&u8g, BIG_FONT);
	u8g_DrawStr(&u8g, 10, 25, time.hours);
	u8g_DrawStr(&u8g, 10 + DIGIT_TIME_HEIGHT, 22, ":");
	u8g_DrawStr(&u8g, 10 + DIGIT_TIME_HEIGHT * 2, 25, time.minutesH);
	u8g_DrawStr(&u8g, 10 + DIGIT_TIME_HEIGHT * 3, 25, time.minutesL);
	u8g_DrawStr(&u8g, 10 + DIGIT_TIME_HEIGHT * 4, 22, ":");
	u8g_DrawStr(&u8g, 10 + DIGIT_TIME_HEIGHT * 5, 25, time.secondsH);
	u8g_DrawStr(&u8g, 10 + DIGIT_TIME_HEIGHT * 6, 25, time.secondsL);

} 

void drawMainMenuByScreen(uint8_t screen)
{
	switch(screen) {
		case SCREEN_START:
			drawMainMenu(text_load, text_start);
			break;
		case SCREEN_COUNTING:
			drawMainMenu(text_empty, text_stop);
			break;
		case SCREEN_PAUSED:
			drawMainMenu(text_reset, text_start);
			break;
		case SCREEN_END:
			drawMainMenu(text_reset, text_empty);
			break;		
		case SCREEN_MEMORY_SAVE:
			drawMainMenu(text_memory_select, text_exit);
			break;
		case SCREEN_MEMORY_SELECT:
			drawMainMenu(text_memory_select, text_exit);
			break;
		case SCREEN_CHOOSED_TIME:
			drawMainMenu(text_save, text_start);
			break;
	}
}

void drawMainMenu(const char* text1, const char *text2)
{
	char buffer[10];
	
	u8g_SetFont(&u8g, SMALL_FONT);
	
	strcpy_P(buffer, text1);
	if (buffer[0]) {
		u8g_DrawFrame(&u8g, 0, 46, 62, 18);
		u8g_DrawStr(&u8g, 3, 58, buffer);
	}
	
	strcpy_P(buffer, text2);
	if (buffer[0]) {
		u8g_DrawFrame(&u8g, 66, 46, 62, 18);
		u8g_DrawStr(&u8g, 69, 58, buffer);
	}
	
}


struct displayTime secondsToDisplay(uint16_t seconds)
{	
	struct displayTime ret;
	
	itoa(seconds / 3600, ret.hours, 10);
	
	itoa(((seconds % 3600) / 60) / 10, ret.minutesH, 10);
	
	itoa(((seconds % 3600) / 60) % 10, ret.minutesL, 10);
	
	itoa((seconds % 60) / 10, ret.secondsH, 10);
	
	itoa((seconds % 60) % 10, ret.secondsL, 10);
	
	return ret;
}

char * secondsToChar(uint16_t seconds) 
{

	struct displayTime time = secondsToDisplay(seconds);

	char *str = (char *) malloc(sizeof(char) * 8);
	str[0] = time.hours[0];
	str[1] = ':';
	str[2] = time.minutesH[0];
	str[3] = time.minutesL[0];
	str[4] = ':';
	str[5] = time.secondsH[0];
	str[6] = time.secondsL[0];
	str[7] = '\0';

	return str;
}