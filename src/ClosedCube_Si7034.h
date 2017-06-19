
/*

Arduino library for Silabs Si7034 Digital I2C Humidity and Temperature Sensor
Written by AA for ClosedCube
---

The MIT License (MIT)

Copyright (c) 2017 ClosedCube Limited

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/


#ifndef _CLOSEDCUBE_SI7034_h
#define _CLOSEDCUBE_SI7034_h

#include <Arduino.h>

typedef union {
	byte rawData[8];
	struct {
		long sna;
		long snb;
	};	
} Si7034_SerialNumber;


struct Si7034_Result {
	float t;
	float rh;
	uint8_t error;
};

class ClosedCube_Si7034
{

 public:
	void begin(uint8_t address);
	uint16_t readFirmwareRevision(); // 0x10 = Firware version 1.0
	bool queryDevice();

	Si7034_SerialNumber readSerialNumber();

	Si7034_Result fastMeasurement();
	Si7034_Result normalMeasurement();
	
	Si7034_Heater readHeater();
	void heatOn();
	void heatOff();
 private:
	 uint8_t _address;
	 
	 Si7034_Result noHoldModeMeasurement(uint16_t hexCode);
	 float calculateT(uint16_t rawValue);
	 float calculateRH(uint16_t rawValue);

};


#endif

