#include <RotaryDialer.h>

#define PIN_READY	8
#define PIN_PULSE	9

RotaryDialer dialer = RotaryDialer(8, 9);

void setup() {
	Serial.begin(28800);
	dialer.setup();
}

void loop() {
	if (dialer.update()) {
		Serial.println(dialer.getNextNumber());
	}
}
