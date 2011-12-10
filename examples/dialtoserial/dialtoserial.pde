/**
 * Print each digit over Serial as it is dialed.
 * See the README for detailed documentation.
 */

#include <RotaryDialer.h>

#define PIN_READY	8
#define PIN_PULSE	9

RotaryDialer dialer = RotaryDialer(PIN_READY, PIN_PULSE);

void setup() {
	Serial.begin(28800);
	dialer.setup();
}

void loop() {
	if (dialer.update()) {
		Serial.println(dialer.getNextNumber());
	}
}
