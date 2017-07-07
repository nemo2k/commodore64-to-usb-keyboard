#include <Arduino.h>
#include "ProTrinketKeyboard.h"
#include "commodore64-keys.h"

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

	// Check if RunStop is pressed for alt
	pinMode(SENDERS[0].gpio, OUTPUT);
	digitalWrite(SENDERS[0].gpio, LOW);
	if(digitalRead(C64[3].gpio) == LOW) {
		mod += 0x04;
	}
	digitalWrite(SENDERS[0].gpio, HIGH);
	pinMode(SENDERS[0].gpio, INPUT);

	// Check if C= is pressed for control, ctrl on c64 is remapped to tab
	pinMode(SENDERS[0].gpio, OUTPUT);
	digitalWrite(SENDERS[0].gpio, LOW);
	if(digitalRead(C64[5].gpio) == LOW) {
		mod += 0x01;
	}
	digitalWrite(SENDERS[0].gpio, HIGH);
	pinMode(SENDERS[0].gpio, INPUT);

	// Check if left shift is pressed for shift
	pinMode(SENDERS[1].gpio, OUTPUT);
	digitalWrite(SENDERS[1].gpio, LOW);
	if(digitalRead(C64[3].gpio) == LOW) {
		mod += 0x02;
	}
	digitalWrite(SENDERS[1].gpio, HIGH);
	pinMode(SENDERS[1].gpio, INPUT);

	// Check if right shift is pressed for alt gr
	pinMode(SENDERS[6].gpio, OUTPUT);
	digitalWrite(SENDERS[6].gpio, LOW);
	if(digitalRead(C64[4].gpio) == LOW) {
		mod += 0x40;
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
