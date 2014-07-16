/*
 * Step.h
 *
 *  Created on: 15/lug/2014
 *      Author: ii40253
 */

#ifndef STEP_H_
#define STEP_H_

#include "Arduino.h"

class Step {
public:
	Step(uint16_t position, uint16_t delay, uint16_t speed, uint16_t acceleration);
private:
	uint16_t position;
	uint16_t delay;
	uint16_t speed;
	uint16_t acceleration;
};

#endif /* STEP_H_ */
