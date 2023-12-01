#include <Arduino.h>
#include "lights.h"

boolean button_pressed = false;
boolean is_random_mode = true;
unsigned long total_count = 0;
Line line1;
Line line2;

int modes[] = {MODE_OPPOSITE_BLINK, MODE_SEQUENCE_BLINK, MODE_ALL_BLINK, MODE_ALL_ON};

void debug() {
	Serial.print(total_count);
	Serial.print(" ");
	Serial.print(line1.mode);
	Serial.print(" led1=");
	Serial.print(line1.led1);
	Serial.print(" led2=");
	Serial.print(line1.led2);
	Serial.print(" step=");
	Serial.print(line1.step);
	Serial.println();
}

void setup() {
	Serial.begin(9600);

//    pinMode(7, INPUT); // button
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);

	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);

	line1.mode = MODE_OPPOSITE_BLINK;
//     line1.mode = MODE_OPPOSITE_SWITCH;
//     line1.mode = MODE_SEQUENCE_BLINK;
//     line1.mode = MODE_ALL_BLINK;
	// line1.mode = MODE_ALL_ON;
	init_line_settings(&line1);
}

void loop() {
	update_line(&line1);
	update_line(&line2);

	analogWrite(5, line1.led1);
	analogWrite(6, line1.led2);

	analogWrite(9, line2.led1);
	analogWrite(10, line2.led2);

	++total_count;

	if (is_random_mode && total_count % 1000 == 0) {
		line1.mode = random(MODE_TYPE_COUNT);
		line2.mode = random(MODE_TYPE_COUNT);

		init_line_settings(&line1);
		init_line_settings(&line2);
	}

	// Switch mode
//    if (digitalRead(7)) {
//        if (!button_pressed) {
//            line1.mode = (line1.mode + 1) % MODE_TYPE_COUNT;
//            init_line_settings(&line1);
//
//            button_pressed = true;
//        }
//
//    } else if (button_pressed) {
//        button_pressed = false;
//    }

	// Debug
//    if (total_count % 100 == 0) {
//         debug();
//    }

	delay(50);
}
