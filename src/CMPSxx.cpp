#include "CMPSxx.h"

#include <Wire.h>

#include "tools-log.h"

bool CMPS12::begin()
{
	if(!TwoWireDevice::begin())
	{
		// ERROR("TwoWireDevice.begin() failed.");
		return false;
	};
	
	// read sw version
	uint8_t version = readreg8(REG_COMMAND);
	if(version != 5)
	{
		// ERROR("CMPS11 device did not reply correctly to version request. Reply to version =  %d", version);
		return false;
	};
	
	return true;
};

bool CMPS14::begin()
{
	if(!TwoWireDevice::begin())
	{
		// ERROR("TwoWireDevice.begin() failed.");
		return false;
	};
	
	// read sw version
	uint8_t version = readreg8(REG_COMMAND);
	if(version != 7)
	{
		// ERROR("CMPS14 device did not reply correctly to version request. Reply to version =  %d", version);
		return false;
	};
	
	return true;
};

uint8_t CMPS12::getCalStatRaw()
{
	return readreg8(REG_CALSTAT);// & 0x03;
};

CMPS12::calstat_t CMPS12::getCalStat()
{
	uint8_t cal = getCalStatRaw();
	if(cal == 0xFF) return CAL_HIGH;
	uint8_t cal_s = cal >> 6;
	// uint8_t cal_g = (cal >> 4) & 0x03;
	// uint8_t cal_a = (cal >> 2) & 0x03;
	uint8_t cal_m = cal & 0x03;
	if((cal_s == 0) | (cal_m == 0)) return CAL_LOW;
	return CAL_MED;
};

CMPS14::calstat_t CMPS14::getCalStat()
{
	uint8_t cal = readreg8(REG_CALSTAT);
	if(cal & 0x3F) return CAL_HIGH;
	uint8_t cal_s = cal >> 6;
	uint8_t cal_g = (cal >> 4) & 0x03;
	uint8_t cal_a = (cal >> 2) & 0x03;
	uint8_t cal_m = cal & 0x03;
	if((cal_g == 0) | (cal_a == 0) | (cal_m == 0)) return CAL_LOW;
	return CAL_MED;
};

float CMPS12::getBearing()
{
	return readreg16_ML(REG_BEARING16) / 10.0;
};

float CMPS14::getBearing()
{
	return readreg16_ML(REG_BEARING16) / 10.0;
};

float CMPS12::getPitch()
{
	return (int8_t) readreg8(REG_PITCH90);
};

float CMPS14::getPitch()
{
	return (int16_t) readreg16_ML(REG_PITCH16) / 10.0;
};

float CMPS12::getRoll()
{
	return (int8_t) readreg8(REG_ROLL90);
};

float CMPS14::getRoll()
{
	return (int16_t) readreg16_ML(REG_ROLL16) / 10.0;
};

int8_t CMPS12::getTemp()
{
	return (int8_t) readreg8(REG_TEMP);
};

uint8_t send_sequence(TwoWire& wire, const char* sequence)
{
	wire.beginTransmission(*sequence);
	sequence++;
	wire.write(0x00 /* REG_COMMAND */);
	while(*sequence != 0x00)
	{
		wire.write(*sequence);
		delay(20);
		wire.read();
		sequence++;
	};
	return wire.endTransmission();
};

bool CMPS12::storeCal()
{
	const char seq[] = {_i2caddr, 0xF0, 0xF5, 0xF6, 0x00};
	_last_error = send_sequence(_wire, seq);
	return _last_error == 0;
};

bool CMPS14::storeCal()
{
	const char seq[] = {_i2caddr, 0xF0, 0xF5, 0xF6, 0x00};
	_last_error = send_sequence(_wire, seq);
	return _last_error == 0;
};

bool CMPS12::eraseCal()
{
	const char seq[] = {_i2caddr, 0xE0, 0xE5, 0xE2, 0x00};
	_last_error = send_sequence(_wire, seq);
	return _last_error == 0;
};

bool CMPS14::eraseCal()
{
	const char seq[] = {_i2caddr, 0xE0, 0xE5, 0xE2, 0x00};
	_last_error = send_sequence(_wire, seq);
	return _last_error == 0;
};

bool CMPS14::calConfig(bool periodic_save, bool gyrocal, bool accelcal, bool magcal)
{
	uint8_t settings = 0x80;
	if(periodic_save)
		settings |= 0x10;
	if(gyrocal)
		settings |= 0x04;
	if(accelcal)
		settings |= 0x02;
	if(magcal)
		settings |= 0x01;

	const char seq[] = {_i2caddr, 0x98, 0x95, 0x99, settings};
	_last_error = send_sequence(_wire, seq);
	return _last_error = 0;
};

bool send_setAddress(TwoWire& wire, uint8_t _i2caddr, uint8_t newaddress)
{
	switch(newaddress)
	{
		case (0xC0 >> 1):
		case (0xC2 >> 1):
		case (0xC4 >> 1):
		case (0xC6 >> 1):
		case (0xC8 >> 1):
		case (0xCA >> 1):
		case (0xCC >> 1):
		case (0xCE >> 1):
			newaddress <<= 1;
		case 0xC0:
		case 0xC2:
		case 0xC4:
		case 0xC6:
		case 0xC8:
		case 0xCA:
		case 0xCC:
		case 0xCE:
			break;
		default:
			// ERROR("Invalid new address for CMPS12: 0x%x", address);
			return false;	
	};

	const char seq[] = {_i2caddr, 0xA0, 0xAA, 0xA5, newaddress, 0x00};
	return send_sequence(wire, seq);
};

bool CMPS12::setAddress(uint8_t newaddress)
{
	return 0 == (_last_error = send_setAddress(_wire, _i2caddr, newaddress));
};

bool CMPS14::setAddress(uint8_t newaddress)
{
	return 0 == (_last_error = send_setAddress(_wire, _i2caddr, newaddress));
};
