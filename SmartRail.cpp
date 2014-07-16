#include "SmartRail.h"

#include <SerialDebug.h>
#include <VoltageReference.h>
#include <AccelStepper.h>

#define STEP_INT_PIN 0
#define STEP_1_PIN 0
#define STEP_2_PIN 0
#define STEP_3_PIN 0
#define STEP_4_PIN 0

#define X_AXIS_PIN 0
#define Y_AXIS_PIN 0
#define BUTTON_PIN 0

AccelStepper stepper = AccelStepper(STEP_INT_PIN, STEP_1_PIN, STEP_2_PIN, STEP_3_PIN, STEP_4_PIN);
VoltageReference vRef = VoltageReference();

uint8_t index = 0;
char* command;
bool commandComplete = false;
float acceleration = 100.0;

void setup() {
	pinMode(X_AXIS_PIN, INPUT_PULLUP);
	pinMode(Y_AXIS_PIN, INPUT_PULLUP);
	pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void serialEvent() {
	while (Serial.available() && commandComplete == false) {
		char inChar = Serial.read();
		command[index++] = inChar;
		if (inChar == '\n') {
			index = 0;
			commandComplete = true;
		}
	}
}

void manual() {
	int x = analogRead(X_AXIS_PIN);
	stepper.setSpeed(abs(x));
	if (x > 0)
		stepper.move(1);
	if (x < 0)
		stepper.move(-1);
	if (x != 0)
		DEBUG("speed", stepper.speed());
}

void loop() {
	int y = analogRead(Y_AXIS_PIN);
	if (y > 0)
		stepper.setAcceleration(acceleration + 10.0);
	if (y < 0)
		stepper.setAcceleration(acceleration - 10.0);
	if (y != 0)
		DEBUG("acceleration", acceleration);
}
