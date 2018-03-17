/*
 * draw.c
 *
 * Created: 17.03.2018 19:45:12
 *  Author: gpraw
 */

#include "draw.h"
#include "U8gLib/u8g.h"

#include <avr/io.h>
#include <stdlib.h>

u8g_t u8g;

const char PROGMEM text_memory[] = "[*]memory";
const char PROGMEM text_save[] = "[*]save";
const char PROGMEM text_start[] = "[#]start";
const char PROGMEM text_stop[] = "[#]stop";
const char PROGMEM text_exit[] = "[#]exit";
const char PROGMEM text_reset[] = "[#]reset";
const char PROGMEM text_memory_select[] = "[A-D]sel";
const char PROGMEM text_empty[] = "";


//TODO: trzeba dodaæ obs³uge typów wyœwietlanego ekranu (main, memory i tp)
//TODO: dodaæ obs³ugê aktualnie przycisniêtego przycisku (chyba ???)
void draw(uint16_t seconds)
{
	u8g_FirstPage(&u8g);
	do
	{
		drawTime(secondsToDisplay(seconds));
		drawMainMenu(text_memory, text_empty);
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
