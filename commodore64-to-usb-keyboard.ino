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

byte RESTORE_S = 13;
byte RESTORE_R = 10;

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

	// Manage Restore Key
	pinMode(RESTORE_S, INPUT);
	pinMode(RESTORE_R, INPUT_PULLUP);


	TrinketKeyboard.begin();
}

void loop() {
	TrinketKeyboard.poll(); // Must be called at least once every 10ms
	unsigned long t = millis(); // Prevent multiple calls to millis() in loop

	char mod = 0;
	// Check if Restore is pressed
	mod += checkForModifier(RESTORE_S, RESTORE_R, 0);
	// Check if RunStop is pressed for alt
	mod += checkForModifier(SENDERS[0].gpio, C64[3].gpio, 0x04);
	// Check if C= is pressed for control, ctrl on c64 is remapped to tab
	mod += checkForModifier(SENDERS[0].gpio, C64[5].gpio, 0x01);
	// Check if left shift is pressed for shift
	mod += checkForModifier(SENDERS[1].gpio, C64[3].gpio, 0x02);
	// Check if right shift is pressed for alt gr
	mod += checkForModifier(SENDERS[6].gpio, C64[4].gpio, 0x40);

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
  			                    TrinketKeyboard.pressKey(mod, keymap[(y*8)+x]);
					}
				}
			}
			laststate[(y*8)+x] = gpiostate;
		}
		digitalWrite(SENDERS[y].gpio, HIGH);
		pinMode(SENDERS[y].gpio, INPUT);
	}
}

char checkForModifier(byte sender, byte receiver, char modifier) {
	char mod = 0;
	pinMode(sender, OUTPUT);
	digitalWrite(sender, LOW);
	if(digitalRead(receiver) == LOW) {
		mod = modifier;
	}
	digitalWrite(sender, HIGH);
	pinMode(sender, INPUT);
	return mod;
}
