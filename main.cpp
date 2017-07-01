#include <Arduino.h>
#include "ProTrinketKeyboard.h"

#define AXISESX 8
#define AXISESY 8

struct C64
{
	byte gpio;
} C64[AXISESX] = {
		{0}, 
		{1},
		{3},
		{4},
		{5},
		{6},
		{8},
		{9}
};

struct SENDERS
{
	byte gpio;
} SENDERS[AXISESY] = {
		{A5},
		{A4},
		{A3},
		{A2},
		{A1},
		{A0},
		{12},
		{11}
};

unsigned long debouncer[64];
boolean laststate[64];
boolean state[64];

uint8_t keymap[192] = {
//	1, escape, left control, runstop, space, commodore, q, 2
	0x1E, 0x29, 0x2B, 0x29, 0x2C, 0xE0, 0x14, 0x1F,
//	3 W A LSHIFT Z S E 4
	0x20, 0x1A, 0x04, 0x02, 0x1D, 0x16, 0x08, 0x21,
//	5 R D X C F T 6
	0x22, 0x15, 0x07, 0x1B, 0x06, 0x09, 0x17, 0x23,
//	7 Y G V B H U 8
	0x24, 0x1C, 0x0A, 0x19, 0x05, 0x0B, 0x18, 0x25,
//	9 I J N M K O 0
	0x26, 0x0C, 0x0D, 0x11, 0x10, 0x0E, 0x12, 0x27,
//	+ P L , . : @ -
	86, 0x13, 0x0F, 0x36, 0x37, 0x33, 0x2F, 0x27,
//	£ * ; / RSHIFT = arrowup HOME
	0x31, 0x1F, 0x34, 0x24, 0xE5, 45, 0x52, 0x4A,
//	DEL RETURN ARROWSLEFTRIGHT ARROWSUPDOWN f1 f3 f5 f7
	0x2A, 0x28, 0x50, 0x52, 0x3A, 0x3C, 0x3E, 0x40
};

uint8_t keymapmodifiers[192] = {
//	1, escape, left control, runstop, space, commodore, q, 2
	0, 0, 0, 0, 0, 0, 0, 0,
//	3 W A LSHIFT Z S E 4
	0, 0, 0, 0, 0, 0, 0, 0,
//	5 R D X C F T 6
	0, 0, 0, 0, 0, 0, 0, 0,
//	7 Y G V B H U 8
	0, 0, 0, 0, 0, 0, 0, 0,
//	9 I J N M K O 0
	0, 0, 0, 0, 0, 0, 0, 0,
//	+ P L , . : @ -
	0, 0, 0, 0, 0, 0, 0, 0x02,
//	£ * ; / RSHIFT = arrowup HOME
	0x02, 0x40, 0, 0x02, 0, 0, 0, 0,
//	DEL RETURN ARROWSLEFTRIGHT ARROWSUPDOWN f1 f3 f5 f7
	0, 0, 0, 0, 0, 0, 0, 0
};



void setup() 
{
	for(byte y = 0; y < AXISESY; y++) {
		pinMode(SENDERS[y].gpio, INPUT);
	}

	for(byte x = 0; x < AXISESX; x++) {
		pinMode(C64[x].gpio, INPUT_PULLUP);
	}

	TrinketKeyboard.begin();

	// Turn on C64 red led
	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);
}

void loop() {
	TrinketKeyboard.poll(); // Must be called at least once every 10ms
	unsigned long t = millis(); // Prevent multiple calls to millis() in loop

	char mod = 0;

	// Check if C= is pressed for control, ctrl on c64 is remapped to tab
	pinMode(SENDERS[0].gpio, OUTPUT);
	digitalWrite(SENDERS[0].gpio, LOW);
	if(digitalRead(C64[5].gpio) == LOW) {
		mod = 0x01;
	}
	digitalWrite(SENDERS[0].gpio, HIGH);
	pinMode(SENDERS[0].gpio, INPUT);

	// Check if left shift is pressed for shift
	pinMode(SENDERS[1].gpio, OUTPUT);
	digitalWrite(SENDERS[1].gpio, LOW);
	if(digitalRead(C64[3].gpio) == LOW) {
		mod = 0x02;
	}
	digitalWrite(SENDERS[1].gpio, HIGH);
	pinMode(SENDERS[1].gpio, INPUT);

	// Check if right shift is pressed for alt gr
	pinMode(SENDERS[6].gpio, OUTPUT);
	digitalWrite(SENDERS[6].gpio, LOW);
	if(digitalRead(C64[4].gpio) == LOW) {
		mod = 0x40;
	}
	digitalWrite(SENDERS[6].gpio, HIGH);
	pinMode(SENDERS[6].gpio, INPUT);


	for(byte y = 0; y < AXISESY; y++) {
		pinMode(SENDERS[y].gpio, OUTPUT);
		digitalWrite(SENDERS[y].gpio, LOW);

		for(byte x = 0; x < AXISESX; x++) {
			boolean gpiostate = digitalRead(C64[x].gpio);

			if(gpiostate != laststate[(y*8)+x]) {
				debouncer[(y*8)+x] = t;
			}

			if(t - debouncer[(y*8)+x] > 10) {
				if(laststate[(y*8)+x] != state[(y*8)+x]) {
					state[(y*8)+x] = laststate[(y*8)+x];
					if(state[(y*8)+x]) {
						TrinketKeyboard.pressKey(0,0);
					} else {
						if(mod == 0) {
			                               	TrinketKeyboard.pressKey(keymapmodifiers[(y*8)+x], keymap[(y*8)+x]);
						} else {
			                               	TrinketKeyboard.pressKey(mod, keymap[(y*8)+x]);
						}
					}
				}
			}
			laststate[(y*8)+x] = gpiostate;
		}
		digitalWrite(SENDERS[y].gpio, HIGH);
		pinMode(SENDERS[y].gpio, INPUT);
	}
}
