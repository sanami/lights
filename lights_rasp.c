// gcc -o lights lights_rasp.c -lpigpio
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pigpio.h>
#include "lights.h"

bool is_random_mode = true;
unsigned long total_count = 0;
Line line1;
Line line2;

int modes[] = {MODE_OPPOSITE_BLINK, MODE_SEQUENCE_BLINK, MODE_ALL_BLINK, MODE_ALL_ON};

void setup() {
	line1.mode = MODE_OPPOSITE_BLINK;
	// line1.mode = MODE_OPPOSITE_SWITCH;
	// line1.mode = MODE_SEQUENCE_BLINK;
	// line1.mode = MODE_ALL_BLINK;
	line2.mode = MODE_ALL_BLINK;

	init_line_settings(&line1);
	init_line_settings(&line2);
}

void loop() {
	update_line(&line1);
	update_line(&line2);

	gpioPWM(17, line1.led1);
	gpioPWM(27, line1.led2);
	gpioPWM(4, line2.led1);

	++total_count;

	if (is_random_mode && total_count % 1000 == 0) {
		line1.mode = random_int(MODE_TYPE_COUNT);
		// line2.mode = random_int(MODE_TYPE_COUNT);

		init_line_settings(&line1);
		init_line_settings(&line2);
	}

	gpioSleep(PI_TIME_RELATIVE, 0, 20000);
}

void done() {
	gpioPWM(17, 0);
	gpioPWM(27, 0);
	gpioPWM(4, 0);
}

int main() {
	if (gpioInitialise() < 0) {
		printf("!gpioInitialise\n");
		return -1;
	}

	setup();

	while(true) {
		loop();
	}

	done();
	gpioTerminate();

	return 0;
}
