#include "SmartRail.h"

#include <SerialDebug.h>
#include <VoltageReference.h>
#include <AccelStepper.h>
#include <LiquidCrystal.h>

#define STEP_1_PIN 6
#define STEP_2_PIN 5
#define STEP_3_PIN 4
#define STEP_4_PIN 3

#define X_AXIS_PIN A6
#define Y_AXIS_PIN A7
#define BUTTON_PIN 2

AccelStepper stepper = AccelStepper(AccelStepper::FULL4WIRE, STEP_1_PIN, STEP_3_PIN, STEP_2_PIN, STEP_4_PIN);
VoltageReference vRef = VoltageReference();
LiquidCrystal lcd = LiquidCrystal(12, 11, 10, 9, 8, 7);

uint8_t index = 0;
char* command;
bool commandComplete = false;
float acceleration = 100.0;

void setup() {
	Serial.begin(9600);
	pinMode(X_AXIS_PIN, INPUT);
	pinMode(Y_AXIS_PIN, INPUT);
	pinMode(BUTTON_PIN, INPUT);
	lcd.begin(16, 2);
	lcd.print("ciao mamma");
	stepper.setMaxSpeed(2000.0);
	stepper.setSpeed(500.0);
	stepper.setAcceleration(100.0);
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
	int16_t x = analogRead(X_AXIS_PIN) - 512;
	stepper.setSpeed(abs(x));
	if (x > 0)
		stepper.move(1);
	if (x < 0)
		stepper.move(-1);
	if (x != 0)
		DEBUG("speed", stepper.speed());

	int16_t y = analogRead(Y_AXIS_PIN) - 512;
	if (y > 0)
		stepper.setAcceleration(acceleration + 10.0);
	if (y < 0)
		stepper.setAcceleration(acceleration - 10.0);
	if (y != 0)
		DEBUG("acceleration", acceleration);
}

void stepperDemo() {
	stepper.runToNewPosition(2000);
	delay(1000);
	stepper.runToNewPosition(-2000);
	delay(1000);
	stepper.setSpeed(500);
	stepper.moveTo(2000);
	stepper.runSpeedToPosition();
	delay(1000);
	stepper.moveTo(-2000);
	stepper.runSpeedToPosition();
	delay(1000);
}

void loop() {
	lcd.setCursor(0, 1);
	// print the number of seconds since reset:
	lcd.print(millis() / 1000);
}

