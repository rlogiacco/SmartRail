#include "SmartRail.h"

#include <SerialDebug.h>
#include <VoltageReference.h>
#include <AccelStepper.h>
#include <LiquidCrystal.h>
#include <Flash.h>

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

#include "i18n/messages.h"
#include "i18n/messages_it.h"

uint8_t index = 0;
char* command;
bool commandComplete = false;
float acceleration = 100.0;
uint16_t xCalibrate, yCalibrate;

#define FUNCTIONS_SIZE 2
volatile uint8_t functionIndex = 0;
volatile bool clicked = true;

inline void welcome() {
	display(welcome_1, 0, LEFT);
	display(welcome_2, 1, RIGHT);
	for (int i = 0; i < 3; i++) {
		delay(300);
		lcd.noDisplay();
		delay(200);
		lcd.display();
	}
}

void calibrateJoystick() {
	delay(50);
	xCalibrate = analogRead(X_AXIS_PIN);
	delay(50);
	yCalibrate = analogRead(Y_AXIS_PIN);
}

void click() {
	functionIndex = (functionIndex + 1) % FUNCTIONS_SIZE;
	clicked = true;
}

void setup() {
	Serial.begin(9600);
	pinMode(X_AXIS_PIN, INPUT);
	pinMode(Y_AXIS_PIN, INPUT);
	pinMode(BUTTON_PIN, INPUT);
	stepper.setMaxSpeed(2000.0);
	stepper.setSpeed(500.0);
	stepper.setAcceleration(100.0);
	lcd.begin(16, 2);
	welcome();
	calibrateJoystick();
	attachInterrupt(0, click, CHANGE);
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
	if (clicked) {
		lcd.clear();
		display(manual_mode, 0);
	}
	int16_t x = analogRead(X_AXIS_PIN) - xCalibrate;
	if (x > 50) {
		//DEBUG("LEFT");
		stepper.move(10);
		stepper.setMaxSpeed(1000);
		stepper.setSpeed(abs(x));
	} else if (x < -50) {
		//DEBUG("RIGHT");
		stepper.move(-10);
		stepper.setMaxSpeed(1000);
		stepper.setSpeed(abs(x));
	}

	int16_t y = analogRead(Y_AXIS_PIN) - yCalibrate;
	if (y > 20) {
		acceleration += 10.0;
		stepper.setAcceleration(acceleration);
	} else if (y < -20) {
		acceleration -= 10.0;
		stepper.setAcceleration(acceleration);
	}
	stepper.runSpeedToPosition();
}

void stepperDemo() {
	display(demo_mode, 0);
	display(const_speed_mode, 1);
	stepper.moveTo(4000);
	stepper.setMaxSpeed(500);
	stepper.setSpeed(500);
	DEBUG("cw 1");
	while(stepper.distanceToGo() != 0)
		stepper.runSpeedToPosition();
	delay(1000);

	stepper.moveTo(0);
	stepper.setMaxSpeed(500);
	stepper.setSpeed(500);
	DEBUG("ccw 1");
	while(stepper.distanceToGo() != 0)
		stepper.runSpeedToPosition();
	delay(1000);


	display(smooth_mode, 1);
	stepper.setMaxSpeed(1000);
	stepper.setAcceleration(100.0);
	stepper.setCurrentPosition(0);

	DEBUG("cw 2");
	stepper.runToNewPosition(4000);
	delay(1000);

	DEBUG("ccw 2");
	stepper.runToNewPosition(0);
	delay(1000);
}

void (*functions[])() = {manual, stepperDemo};
void loop() {
	functions[functionIndex]();
	clicked = false;
}

