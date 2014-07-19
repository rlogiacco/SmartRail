#include "SmartRail.h"
#include "Program.h"
#include "Joystick.h"

#include <SerialDebug.h>
#include <VoltageReference.h>
#include <AccelStepper.h>
#include <LiquidCrystal.h>
#include <Flash.h>

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

#include "i18n/messages.h"
#include "i18n/messages_it.h"

uint8_t index = 0;
char* command;
bool commandComplete = false;
float acceleration = 100.0;
uint16_t xCalibrate, yCalibrate;

#define FUNCTIONS_SIZE 2
uint8_t functionIndex = 0;
bool clicked = true;

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

void click() {
	functionIndex = (functionIndex + 1) % FUNCTIONS_SIZE;
	clicked = true;
}

Program programs[16];

void setup() {
	SERIAL_DEBUG_SETUP(9600);
	joystick.begin(X_AXIS_PIN, Y_AXIS_PIN, BUTTON_PIN);
	joystick.calibrate();
	stepper.setMaxSpeed(2000.0);
	stepper.setSpeed(500.0);
	stepper.setAcceleration(100.0);
	lcd.begin(16, 2);
	welcome();
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
	lcd.clear();
	display(manual_mode, 0);
	while (!joystick.button()) {
		int16_t x = joystick.x();
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

		int16_t y = joystick.y();
		if (y > 20) {
			acceleration += 10.0;
			stepper.setAcceleration(acceleration);
		} else if (y < -20) {
			acceleration -= 10.0;
			stepper.setAcceleration(acceleration);
		}
		stepper.runSpeedToPosition();
	}
}

void stepperDemo() {
	display(demo_mode, 0);
	display(const_speed_mode, 1);
	stepper.moveTo(4000);
	stepper.setMaxSpeed(500);
	stepper.setSpeed(500);
	DEBUG("cw 1");
	while (stepper.distanceToGo() != 0)
		stepper.runSpeedToPosition();
	delay(1000);

	stepper.moveTo(0);
	stepper.setMaxSpeed(500);
	stepper.setSpeed(500);
	DEBUG("ccw 1");
	while (stepper.distanceToGo() != 0)
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

	click();
}

void (*functions[])() = {manual, stepperDemo};
void loop() {
	if (joystick.button()) {
		DEBUG("joystick clicked");
		click();
	}
	DEBUG("function", functionIndex);
	functions[functionIndex]();
	clicked = false;
}

