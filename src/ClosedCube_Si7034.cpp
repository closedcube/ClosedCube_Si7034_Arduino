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

#include <Wire.h>
#include "ClosedCube_Si7034.h"

void ClosedCube_Si7034::begin(uint8_t address)
{
	_address = address;
	Wire.begin();
}

Si7034_Result ClosedCube_Si7034::fastMeasurement() {
	return noHoldModeMeasurement(0x609C);
}

Si7034_Result ClosedCube_Si7034::normalMeasurement() {
	return noHoldModeMeasurement(0x7866);
}

Si7034_Result ClosedCube_Si7034::noHoldModeMeasurement(uint16_t hexCode) {
	Si7034_Result result;
	result.error = 0xFF;

	Wire.beginTransmission(_address);
	Wire.write(hexCode >> 8);
	Wire.write(hexCode);
	Wire.endTransmission(false);

	delay(15);

	uint8_t	buf[6];
	Wire.requestFrom(_address, (uint8_t)6);
	if (Wire.readBytes(buf, (int8_t)6) == 6)
	{
		result.t = calculateT(buf[0] << 8 | buf[1]);
		result.rh = calculateRH(buf[3] << 8 | buf[3]);
		result.error = 0x00;
	}
	else {
		result.error = 0x11;
	}

	return result;
}

float ClosedCube_Si7034::calculateT(uint16_t rawValue) {
	return -45 + 175 * (rawValue / pow(2,16));
}

float ClosedCube_Si7034::calculateRH(uint16_t rawValue) {
	return 100 * (rawValue / pow(2, 16));
}


bool ClosedCube_Si7034::queryDevice()
{
	bool result = false;

	Wire.beginTransmission(_address);
	Wire.write(0xEF);
	Wire.write(0xC8);
	Wire.endTransmission();

	delay(25);

	uint8_t	buf[3];
	Wire.requestFrom(_address, (uint8_t)3);
		
	if (Wire.readBytes(buf, (int8_t)3) == 3) {
		if (buf[0] == 0x00 && buf[1] == 0x47 && buf[2] == 0x2B) {
			result = true;
		}
	}
	return result;
}

uint16_t ClosedCube_Si7034::readFirmwareRevision()
{
	Wire.beginTransmission(_address);
	Wire.write(0x84);
	Wire.write(0xF1);
	Wire.endTransmission(false);

	Wire.requestFrom(_address, (uint8_t)1);
	return Wire.read();
}


Si7034_SerialNumber ClosedCube_Si7034::readSerialNumber() {
	byte sna[8];
	byte snb[6];

	Wire.beginTransmission(_address);
	Wire.write(0xFA);
	Wire.write(0x0F);
	Wire.endTransmission(false);

	delay(10);

	Wire.requestFrom(_address, (uint8_t)8);
	Wire.readBytes(sna, (uint8_t)8);

	Wire.beginTransmission(_address);
	Wire.write(0xFC);
	Wire.write(0xC9);
	Wire.endTransmission(false);

	delay(10);

	Wire.requestFrom(_address, (uint8_t)6);	
	Wire.readBytes(snb, (uint8_t)6);

	Si7034_SerialNumber result;

	result.rawData[0] = sna[0];
	result.rawData[1] = sna[2];
	result.rawData[2] = sna[4];
	result.rawData[3] = sna[6];
	
	result.rawData[4] = snb[0];
	result.rawData[5] = snb[1];
	result.rawData[6] = snb[3];
	result.rawData[7] = snb[4];

	return result;
}