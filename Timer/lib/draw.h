/*
 * draw.h
 *
 * Created: 17.03.2018 19:44:58
 *  Author: gpraw
 */ 


#ifndef DRAW_H_
#define DRAW_H_

#include <avr/io.h>

#define DIGIT_TIME_HEIGHT 16
#define BIG_FONT u8g_font_fub20n
#define SMALL_FONT u8g_font_6x12r


struct displayTime {
	char hours[2];
	char minutesH[2];
	char minutesL[2];
	char secondsH[2];
	char secondsL[2];
} display;

struct displayTime secondsToDisplay(uint16_t seconds);
void draw(uint16_t seconds);
void initDisplay();
void drawTime(struct displayTime time);
void drawMainMenu(const char* text1, const char *text2);




#endif /* DRAW_H_ */