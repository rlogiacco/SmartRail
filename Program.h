/*
 * Program.h
 *
 *  Created on: 15/lug/2014
 *      Author: ii40253
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Arduino.h"
#include "Step.h"

class Program {
public:
	Program();
	void add(Step* step);
	void save();
	void load();

private:
	Step* steps[16];
	uint8_t count;
};

#endif /* PROGRAM_H_ */
