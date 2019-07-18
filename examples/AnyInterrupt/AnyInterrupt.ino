/***************
 *
 * AnyInterrupt - external interupt sample
 *
 * A0 ~ A5 switch detected
 *
 * release site: https://github.com/askn37/PCIntVect/
 * maintainer: askn https://twitter.com/askn37
 *
 */

#include <Arduino.h>
#include <pcintvect.h>

void setup (void) {
	Serial.begin(9600);
	Serial.println(F("Startup"));
	pinMode(A0, INPUT_PULLUP);
	pinMode(A1, INPUT_PULLUP);
	pinMode(A2, INPUT_PULLUP);
	pinMode(A3, INPUT_PULLUP);
	pinMode(A4, INPUT_PULLUP);
	pinMode(A5, INPUT_PULLUP);
	attachPCInterrupt(A0, attachVect);
	attachPCInterrupt(A1, attachVect);
	attachPCInterrupt(A2, attachVect);
	attachPCInterrupt(A3, attachVect);
	attachPCInterrupt(A4, attachVect);
	attachPCInterrupt(A5, attachVect);
}

volatile uint8_t attachRegister = 0;
volatile uint8_t attachToggle = 0;
void attachVect (void) {
	attachToggle = ~attachToggle;
	attachRegister = *portInputRegister(digitalPinToPort(A0));
}

void loop (void) {
	static uint8_t beforeToggle = 0;
	if (beforeToggle != attachToggle) {
		beforeToggle = ~beforeToggle;
		uint8_t reg = attachRegister;
		Serial.print(" A0:");
		Serial.print(reg & digitalPinToBitMask(A0) ? "HIGH" : "LOW ");
		Serial.print(" A1:");
		Serial.print(reg & digitalPinToBitMask(A1) ? "HIGH" : "LOW ");
		Serial.print(" A2:");
		Serial.print(reg & digitalPinToBitMask(A2) ? "HIGH" : "LOW ");
		Serial.print(" A3:");
		Serial.print(reg & digitalPinToBitMask(A3) ? "HIGH" : "LOW ");
		Serial.print(" A4:");
		Serial.print(reg & digitalPinToBitMask(A4) ? "HIGH" : "LOW ");
		Serial.print(" A5:");
		Serial.print(reg & digitalPinToBitMask(A5) ? "HIGH" : "LOW ");
		Serial.println();
	}
}

// end of code