/*
 * Step.cpp
 *
 *  Created on: 15/lug/2014
 *      Author: ii40253
 */

#include "Step.h"

Step::Step(uint16_t position, uint16_t delay, uint16_t speed, uint16_t acceleration) {
	this->position = position;
	this->delay = delay;
	this->speed = speed;
	this->acceleration = acceleration;
}

