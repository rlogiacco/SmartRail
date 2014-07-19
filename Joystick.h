/*
 * Joystick.h
 *
 *  Created on: 16/lug/2014
 *      Author: ii40253
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <Arduino.h>

class Joystick {
public:
	enum State {
		NONE,
		CLICK,
		LONG_CLICK,
		DOUBLE_CLICK
	};
	Joystick();
	void begin(uint8_t x, uint8_t y, uint8_t button = 255, uint8_t debounce = 50);
	void calibrate();
	int16_t x();
	int16_t y();
	bool button();
private:
	uint8_t xPin, yPin, buttonPin;
	uint16_t xRef, yRef;
	bool buttonState, buttonRef;
	uint16_t lastDebounce, lastClick;
	uint8_t debounce, duration, interval;
};

#endif /* JOYSTICK_H_ */
