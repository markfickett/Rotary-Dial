#pragma once

/**
 * Read numbers from a rotary dial (or otherwise using pulse dialing; see
 * http://en.wikipedia.org/wiki/Rotary_dial ). This implementation is for the
 * North American system, where [1, 9] pulses correspond to the numbers [1, 9],
 * and 0 is represented by 10 pulses.
 *
 * On the back of the dial are two connections relevant for this implementation.
 * One (the 'ready' switch) is normally open (NO), and is closed whenever the
 * rotor is not at rest (specifically, as soon as the user draws back the rotor,
 * and until it finishes returning). The other (the 'pulse' switch) is normally
 * closed (NC), and is opened briefly for each pulse (roughly 10 - 20 Hz).
 *
 * The expected circuit is:
 *	     Rotary Dial                              Arduino
 *	                      /---------------------- readyPin
 *	  /- ready switch (NO) -- pull-up resistor -- VCC
 *	 /-- pulse switch (NC) -- pull-up resistor -/
 *	 \                    \---------------------- pulsePin
 *	  \------------------------------------------ GND
 *
 * The expected sequence is:
 *	readyPin	pulsePin	state
 *	HIGH		n/a		default (waiting)
 *	LOW		LOW		ready to dial / for first pulse
 *	LOW		HIGH		pulse received (number = 1)
 *	LOW		LOW		ready for next pulse
 *	LOW		HIGH		pulse received (number = 2)
 *	LOW		...		(repeat)
 *	HIGH		n/a		rotation complete, count recorded
 */
class RotaryDialer {
	private:
		int pinReady;
		int pinPulse;
		bool hasCompletedNumber;
		int number;

		enum State {
			WAITING,
			LISTENING_NOPULSE,
			LISTENING_PULSE
		};
		enum State state;

		void completeDial();
	public:
		RotaryDialer(int readyPin, int pulsePin);
		void setup();
		bool update();
		bool hasNextNumber();
		int getNextNumber();
};

