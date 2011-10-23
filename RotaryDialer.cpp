#include "RotaryDialer.h"

#include "WProgram.h"

#define NO_NUMBER -1

RotaryDialer::RotaryDialer(int readyPin, int pulsePin) :
	pinReady(readyPin), pinPulse(pulsePin), hasCompletedNumber(false),
	state(WAITING)
{ }

void RotaryDialer::setup() {
	pinMode(pinReady, INPUT);
	pinMode(pinPulse, INPUT);
	digitalWrite(pinReady, HIGH);
	digitalWrite(pinPulse, HIGH);

	Serial.println("waiting for first dial");
}

void RotaryDialer::completeDial() {
	state = WAITING;
	Serial.println("dial complete");
	if (number >= 0 && number <= 10) {
		if (number == 10) {
			number = 0;
		}
		Serial.print("got good number ");
		Serial.println(number);
		hasCompletedNumber = true;
	}
	Serial.println("waiting for next dial");
}

bool RotaryDialer::update() {
	int readyStatus = digitalRead(pinReady);
	int pulseStatus = digitalRead(pinPulse);

	switch(state) {
		case WAITING:
			if (readyStatus == LOW) {
				Serial.println("listening for pulses");
				hasCompletedNumber = false;
				number = 0;
				state = LISTENING_NOPULSE;
			}
			break;
		case LISTENING_NOPULSE:
			if (readyStatus == HIGH) {
				completeDial();
			} else if (pulseStatus == HIGH) {
				Serial.println("into pulse");
				state = LISTENING_PULSE;
			}
			break;
		case LISTENING_PULSE:
			if (readyStatus == HIGH) {
				completeDial();
			} else if (pulseStatus == LOW) {
				Serial.print("out of pulse, number=");
				number++;
				Serial.println(number);
				state = LISTENING_NOPULSE;
			}
			break;
	}
	return hasCompletedNumber;
}

bool RotaryDialer::hasNextNumber() {
	return hasCompletedNumber;
}

int RotaryDialer::getNextNumber() {
	if (hasCompletedNumber) {
		hasCompletedNumber = false;
		return number;
	} else {
		return NO_NUMBER;
	}
}

