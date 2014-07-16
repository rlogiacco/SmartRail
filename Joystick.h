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
	Joystick();
	void begin(uint8_t x, uint8_t y, uint8_t button = 255);
	void calibrate();
	int16_t x();
	int16_t y();
private:
	uint8_t _x, _y, _button;
	uint16_t _xRef, _yRef, _buttonRef;
};

#endif /* JOYSTICK_H_ */
