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

#endif /* MESSAGES_H_ */