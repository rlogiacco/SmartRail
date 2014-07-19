/*
 * Joystick.cpp
 *
 *  Created on: 16/lug/2014
 *      Author: ii40253
 */

#include "Joystick.h"

Joystick::Joystick() { }

void Joystick::begin(uint8_t x, uint8_t y, uint8_t button) {
	this->_x = x;
	this->_y = y;
	this->_button = button;
	this->calibrate();
	pinMode(x, INPUT);
	pinMode(y, INPUT);
	pinMode(button, INPUT);
}

void Joystick::calibrate() {
	_xRef = analogRead(_x);
	_yRef = analogRead(_y);
	_buttonRef = digitalRead(_button);
}

int16_t Joystick::x() {
	return analogRead(_x) - _xRef;
}

int16_t Joystick::y() {
	return analogRead(_y) - _yRef;
}
