/*
 * Joystick.cpp
 *
 *  Created on: 16/lug/2014
 *      Author: ii40253
 */

#include "Joystick.h"
#include <SerialDebug.h>

Joystick::Joystick() {
	lastDebounce = 0;
}

void Joystick::begin(uint8_t x, uint8_t y, uint8_t button, uint8_t debounce) {
	this->xPin = x;
	this->yPin = y;
	this->buttonPin = button;
	this->debounce = debounce;
	pinMode(x, INPUT);
	pinMode(y, INPUT);
	pinMode(button, INPUT_PULLUP);
	buttonRef, buttonState = HIGH;
	this->calibrate();
}

void Joystick::calibrate() {
	xRef = analogRead(xPin);
	yRef = analogRead(yPin);

	DEBUG("calibrate", xRef, yRef, buttonRef);
}

int16_t Joystick::x() {
	return analogRead(xPin) - xRef;
}

int16_t Joystick::y() {
	return analogRead(yPin) - yRef;
}

bool Joystick::button() {
	uint8_t reading = digitalRead(buttonPin);
	DEBUG(reading, buttonState);
	if (reading != buttonState) {
		// reset the debouncing timer
		lastDebounce = millis();
	}

	if ((millis() - lastDebounce) > debounce) {
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:
		if (reading != buttonState) {
			buttonState = reading;
		}
	}
	return buttonState != buttonRef;
}
