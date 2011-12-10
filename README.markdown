Rotary Dial
===========

Arduino library to read numbers from a rotary dial (or otherwise using pulse dialing; see [Wikipedia: Rotary dial](http://en.wikipedia.org/wiki/Rotary_dial).

![front of a rotary telephone dial](http://www.markfickett.com/umbrella/images/111105rotarydialfront-sm.jpg "Rotary Dial")

This implementation is for the North American system, where [1, 9] pulses correspond to the numbers [1, 9], and 0 is represented by 10 pulses. This library was written for use with the dial [demonstrated here](http://commons.wikimedia.org/wiki/File:Rotary_Dial,_Dialing_Back_with_LEDs.ogv).

Connection & Circuit
--------------------

On the back of the dial are two connections relevant for this implementation.  One (the 'ready' switch) is normally open (NO), and is closed whenever the rotor is not at rest (specifically, as soon as the user draws back the rotor, and until it finishes returning). The other (the 'pulse' switch) is normally closed (NC), and is opened briefly for each pulse (roughly 10 - 20 Hz).

The expected circuit is:

	     Rotary Dial                              Arduino
	                      /---------------------- readyPin
	  /- ready switch (NO) -- pull-up resistor -- VCC
	 /-- pulse switch (NC) -- pull-up resistor -/
	 \                    \---------------------- pulsePin
	  \------------------------------------------ GND

The expected sequence is:

	readyPin	pulsePin	state
	HIGH		n/a		default (waiting)
	LOW		LOW		ready to dial / for first pulse
	LOW		HIGH		pulse received (number = 1)
	LOW		LOW		ready for next pulse
	LOW		HIGH		pulse received (number = 2)
	LOW		...		(repeat)
	HIGH		n/a		rotation complete, count recorded

There is 15ms allowed for debounce, which is the implementation's only
constraint on pulse speed.

See Also
--------

See also: [USB output from the Arduino](http://www.arduino.cc/playground/Main/InterfacingWithHardware#USB), including [presenting the Arduino as a keyboard by installing new firmware](http://hunt.net.nz/users/darran/?tag=keyboard).

