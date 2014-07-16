/*
 * Program.cpp
 *
 *  Created on: 15/lug/2014
 *      Author: ii40253
 */

#include "Program.h"

Program::Program() {
	this->count = 0;
}

void Program::add(Step* step) {
	this->steps[this->count++] = step;
}

void Program::save() {

}
