/*
 * Joystick.cpp
 *
 *  Created on: 16/lug/2014
 *      Author: ii40253
 */

#include "Joystick.h"
#define SERIAL_DEBUG false
#include <SerialDebug.h>

Joystick::Joystick(uint8_t x, uint8_t y, uint8_t button, uint8_t debounce) {
	lastDebounce = 0;
	this->xPin = x;
	this->yPin = y;
	this->buttonPin = button;
	this->debounce = debounce;
	this->xRef = 511;
	this->yRef = 511;
	this->buttonRef = HIGH;
	this->buttonState = HIGH;
	this->lastState = HIGH;
	pinMode(x, INPUT);
	pinMode(y, INPUT);
	pinMode(button, INPUT_PULLUP);
}

void Joystick::calibrate() {
	xRef = analogRead(xPin);
	yRef = analogRead(yPin);
	buttonRef = digitalRead(buttonPin);
	buttonState = buttonRef;
	lastState = buttonRef;
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
	if (reading != lastState) {
		// reset the debouncing timer
		lastDebounce = millis();
	}

	if ((millis() - lastDebounce) > debounce) {
		DEBUG("debounce", millis() - lastDebounce, reading, buttonState);
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:
		if (reading != buttonState) {
			DEBUG("changing button state", buttonState, reading);
			buttonState = reading;
		}
	}
	lastState = reading;
	return buttonState != buttonRef;
}
