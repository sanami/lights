#ifndef LIGHTS_H
#define LIGHTS_H

#define MIN_LED 1
#define MAX_LED 40
#define MODE_TYPE_COUNT 5

typedef enum ModeType {
	MODE_OPPOSITE_BLINK, MODE_OPPOSITE_SWITCH, MODE_SEQUENCE_BLINK, MODE_ALL_BLINK, MODE_ALL_ON
} ModeType;

typedef enum ActionType {
	ACTION_INC, ACTION_DEC
} ActionType;

typedef struct Line {
	int mode;
	float led1;
	float led2;
	int count1;
	int count2;
	int action;
	float step;
	float *current_led;
} Line;

int random_int(int max) {
#ifdef ARDUINO
	return random(max);
#else
	return rand() % max;
#endif
}

void init_line(Line *line, ActionType action) {
	line->action = action;

	switch (line->mode) {
		case MODE_OPPOSITE_BLINK:
		case MODE_OPPOSITE_SWITCH:
			if (action == ACTION_INC) {
				line->led1 = MIN_LED;
				line->led2 = MAX_LED;
			} else {
				line->led1 = MAX_LED;
				line->led2 = MIN_LED;
			}
			line->count1 = 0;
			break;

		case MODE_ALL_BLINK:
			if (action == ACTION_INC) {
				line->led1 = MIN_LED;
				line->led2 = MIN_LED;
			} else {
				line->led1 = MAX_LED;
				line->led2 = MAX_LED;
			}
			break;

		case MODE_SEQUENCE_BLINK:
			line->led1 = 0;
			line->led2 = 0;

			line->current_led = (line->count1 == 0) ? &line->led1 : &line->led2;

			if (action == ACTION_INC) {
				*line->current_led = MIN_LED;
			} else {
				*line->current_led = MAX_LED;
			}
			break;

		case MODE_ALL_ON:
			line->led1 = MAX_LED;
			line->led2 = MAX_LED;
	}
}

void update_line(Line *line) {
	switch (line->mode) {
		case MODE_OPPOSITE_BLINK:
			if (line->action == ACTION_INC) {
				line->led1 += line->step;
				line->led2 -= line->step;

				if (line->led1 > MAX_LED || line->led2 < MIN_LED) {
					init_line(line, ACTION_DEC);
				}
			} else {
				line->led1 -= line->step;
				line->led2 += line->step;

				if (line->led1 < MIN_LED || line->led2 > MAX_LED) {
					init_line(line, ACTION_INC);
				}
			}
			break;

		case MODE_ALL_BLINK:
			if (line->action == ACTION_INC) {
				line->led1 += line->step;
				line->led2 += line->step;

				if (line->led1 > MAX_LED || line->led2 > MAX_LED) {
					init_line(line, ACTION_DEC);
				}
			} else {
				line->led1 -= line->step;
				line->led2 -= line->step;

				if (line->led1 < MIN_LED || line->led2 < MIN_LED) {
					init_line(line, ACTION_INC);
				}
			}
			break;

		case MODE_OPPOSITE_SWITCH:
			line->count1 += line->step;
			if (line->count1 > MAX_LED) {
				init_line(line, line->action == ACTION_INC ? ACTION_DEC : ACTION_INC);
			}
			break;

		case MODE_SEQUENCE_BLINK:
			if (line->action == ACTION_INC) {
				*line->current_led += line->step;

				if (*line->current_led > MAX_LED) {
					init_line(line, ACTION_DEC);
				}
			} else {
				*line->current_led -= line->step;

				if (*line->current_led < MIN_LED) {
					line->count1 = (line->count1 == 0) ? 1 : 0;
					init_line(line, ACTION_INC);
				}
			}
			break;

		case MODE_ALL_ON:
			break;
	}
}

void change_line_settings(Line *line) {
	switch (line->mode) {
		case MODE_OPPOSITE_BLINK:
		case MODE_OPPOSITE_SWITCH:
		case MODE_SEQUENCE_BLINK:
			line->step = 1 + random_int(5);
			break;
		case MODE_ALL_BLINK:
			line->step = 0.1 + random_int(5) / 5.0;
			break;
		case MODE_ALL_ON:
			break;
	}
}

void init_line_settings(Line *line) {
	line->count1 = 0;
	change_line_settings(line);
	init_line(line, ACTION_INC);
}

#endif
