#include "SmartRail.h"
#include "Program.h"
#include "Joystick.h"

#include <SerialDebug.h>
#include <VoltageReference.h>
#include <AccelStepper.h>
#include <LiquidCrystal.h>

// stepper motor pins
#define STEP_1_PIN 6
#define STEP_2_PIN 5
#define STEP_3_PIN 4
#define STEP_4_PIN 3

// joystick pins
#define X_AXIS_PIN A6
#define Y_AXIS_PIN A7
#define BUTTON_PIN 2

// lcd pins
#define LCD_ENABLE_PIN 12
#define LCD_SELECT_PIN 11
#define LCD_DATA_1_PIN 10
#define LCD_DATA_2_PIN 9
#define LCD_DATA_3_PIN 8
#define LCD_DATA_4_PIN 7
#define DATA_5_PIN 5
#define STEP_4_PIN 3



AccelStepper stepper = AccelStepper(AccelStepper::FULL4WIRE, STEP_1_PIN, STEP_3_PIN, STEP_2_PIN, STEP_4_PIN);
VoltageReference vRef = VoltageReference();
LiquidCrystal lcd = LiquidCrystal(LCD_ENABLE_PIN, LCD_SELECT_PIN, LCD_DATA_1_PIN, LCD_DATA_2_PIN, LCD_DATA_3_PIN, LCD_DATA_4_PIN);
Joystick joystick = Joystick();

uint8_t index = 0;
char* command;
bool commandComplete = false;
float acceleration = 100.0;

Program programs[16];

void setup() {
	Serial.begin(9600);
	joystick.begin(A6, A7, 2);
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

