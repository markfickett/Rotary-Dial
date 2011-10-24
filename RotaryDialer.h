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
 *
 * There is 15ms allowed for debounce, which is the implementation's only
 * constraint on pulse speed.
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
		unsigned long lastStateChangeMillis;

		/**
		 * Change state, but only if enough time has elapsed since
		 * the last state change (to protect from noise).
		 */
		bool changeStateIfDebounced(enum State newState);

		/**
		 * To be called when ready returns HIGH (when the rotor returns
		 * to its rest position); save the number, if valid.
		 */
		void completeDial();
	public:
		/**
		 * Create a new RotaryDialer listening on the given pins.
		 * @param readyPin connected to a NO (HIGH) switch on the rotor
		 *	which is closed (LOW) during dialing
		 * @param pulsePin connected to a NC (LOW) switch on the rotor
		 *	which is opened (HIGH) during each pulse
		 */
		RotaryDialer(int readyPin, int pulsePin);

		/**
		 * Initialize the pins; digital read pins, held HIGH.
		 */
		void setup();

		/**
		 * Check the pins and update state (in or out of a pulse,
		 * dialing complete, etc). This must be called at least as
		 * pulses; assuming 10 pulses per second, every 50ms.
		 */
		bool update();

		/**
		 * @return whether a new number has been dialed since the last
		 *	getNextNumber call
		 */
		bool hasNextNumber();

		/**
		 * Get the most recently dialed number. After this is called,
		 * hasNextNumber will return false until a new number is dialed.
		 * @return the most recently dialed number, and clear
		 *	hasNextNumber
		 */
		int getNextNumber();
};

