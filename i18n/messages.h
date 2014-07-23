/*
 * messages.h
 *
 *  Created on: 18/lug/2014
 *      Author: rlogiacco
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <avr/pgmspace.h>

char buffer[16];

enum Align {
	CENTER, LEFT, RIGHT
};

#define LCD_SYMBOLS_SIZE 4
enum Symbol {
	SELECT, PLAY, STOP, PAUSE
};

uint8_t LCD_SYMBOLS[4][8] = {
		{B00001,B00011,B00111,B01111,B00111,B00011,B00001},
		{B10000,B11000,B11100,B11110,B11100,B11000,B10000},
		{B00000,B11111,B11111,B11111,B11111,B11111,B00000},
		{B00000,B10001,B10001,B10001,B10001,B10001,B00000}
};

inline void display(const PROGMEM char *str, uint8_t line, Align align = CENTER) {
	for (int i = 0; i < 16; i++) {
		lcd.setCursor(i, line);
		lcd.print(' ');
		buffer[i] = '\0';
	}
	strncpy_P(buffer, str, strlen_P(str));
	switch (align) {
		case LEFT:
			lcd.setCursor(0, line);
			break;
		case RIGHT:
			lcd.setCursor(16 - strlen_P(str), line);
			break;
		case CENTER:
			lcd.setCursor((16 - strlen_P(str)) / 2, line);
			break;
	}
	lcd.print(buffer);
}

inline void clear(uint8_t line) {
	for (int i = 0; i < 16; i++) {
		lcd.setCursor(i, line);
		lcd.print(' ');
	}
}
#endif /* MESSAGES_H_ */
