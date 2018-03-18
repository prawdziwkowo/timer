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
#define BIG_FONT u8g_font_fub20r
#define SMALL_FONT u8g_font_6x12r

//ekran startowy
#define SCREEN_START 0
//odliczanie
#define SCREEN_COUNTING 1
//przerwane odliczanie
#define SCREEN_PAUSED 2
//koniec odliczania
#define SCREEN_END 3
//wybor czasu z pamiêci
#define SCREEN_MEMORY_SELECT 4
//zapis czasu w pamiêci
#define SCREEN_MEMORY_SAVE 5
//wpisywanie czasu
#define SCREEN_CHOOSING_TIME 6
//czas wpisany
#define SCREEN_CHOOSED_TIME 7

struct displayTime {
	char hours[2];
	char minutesH[2];
	char minutesL[2];
	char secondsH[2];
	char secondsL[2];
} display;


void draw(uint8_t screen, uint16_t seconds, struct displayTime time);
void initDisplay();
void drawEnteredTime(uint16_t seconds);
void drawTime(struct displayTime time);
void drawMainMenuByScreen(uint8_t screen);
void drawMainMenu(const char* text1, const char *text2);
void drawMemoryScreen();

struct displayTime secondsToDisplay(uint16_t seconds);
char * secondsToChar(uint16_t seconds);




#endif /* DRAW_H_ */