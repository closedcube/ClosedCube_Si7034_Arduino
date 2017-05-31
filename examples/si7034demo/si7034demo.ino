/**************************************************************************************

This is example for ClosedCube HDC1080 Humidity and Temperature Sensor breakout booard

Initial Date: 31-May-2017

Hardware connections for Arduino Uno:
VDD to 3.3V DC
SCL to A5
SDA to A4
GND to common ground

Written by AA for ClosedCube

MIT License

**************************************************************************************/

#include "ClosedCube_Si7034.h"

ClosedCube_Si7034 si7034;

void setup()
{
	Serial.begin(9600);
	Serial.println("ClosedCube Si7034 Arduino Test");
	
	si7034.begin(0x70);
	
	initSensor();
}

void loop()
{
	printResult("[F]", si7034.fastMeasurement());
	printResult("[N]", si7034.fastMeasurement());
	
	delay(250);
}

void initSensor() {
	Serial.print("Querying device...");
	if (si7034.queryDevice())
		Serial.println("OK.");
	else
		Serial.println("ERROR: Cannot query device.");

	Serial.print("Firmware Revision: 0x");
	Serial.println(si7034.readFirmwareRevision(), HEX);

	Si7034_SerialNumber srn = si7034.readSerialNumber();
	Serial.print("Serial #: 0x");
	Serial.print(srn.snb, HEX);
	Serial.println(srn.snb, HEX);
}

void printResult(const char* modeText, Si7034_Result result) {
	Serial.print(modeText);
	Serial.print("  ");
	if (result.error == 0x00) 
	{
		Serial.print("T: ");
		Serial.print(result.t);
		Serial.print("C  RH: ");
		Serial.print(result.rh);
		Serial.print("%");
	}
	else 
	{
		Serial.print("ERROR: Code = 0x");
		Serial.print(result.error, HEX);
	}

	Serial.println();
}