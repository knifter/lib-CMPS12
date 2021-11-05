#include "CMPS12.h"
// #include "CMPS12_regs.h"

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
		// ERROR("CMPS device did not reply correctly to version request.");
		return false;
	};
	
	return true;
};

uint8_t CMPS12::getCalStatRaw()
{
	return readreg8(REG_CALSTAT);// & 0x03;
};

calstat_t CMPS12::getCalStat()
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

float CMPS12::getBearing()
{
	return readreg16_ML(REG_BEARING16) / 10.0;
};

float CMPS12::getPitch()
{
	return (int16_t) readreg16_ML(REG_PITCH16);
};

float CMPS12::getRoll()
{
	return (int8_t) readreg8(REG_ROLL);
};

int8_t CMPS12::getTemp()
{
	return (int8_t) readreg8(REG_TEMP);
};

bool CMPS12::storeCal()
{
	_wire.beginTransmission(_i2caddr);
	_wire.write(REG_COMMAND);
	_wire.write(0xF0);
	delay(20);
	_wire.write(0xF5);
	delay(20);
	_wire.write(0xF6);
	delay(20);
	_last_error = _wire.endTransmission();
	return _last_error == 0;
};

bool CMPS12::eraseCal()
{
	_wire.beginTransmission(_i2caddr);
	_wire.write(REG_COMMAND);
	_wire.write(0xE0);
	delay(20);
	_wire.write(0xE5);
	delay(20);
	_wire.write(0xE2);
	delay(20);
	_last_error = _wire.endTransmission();
	return _last_error == 0;
};

bool CMPS12::setAddress(uint8_t address)
{
	switch(address)
	{
		case (0xC0 >> 1):
		case (0xC2 >> 1):
		case (0xC4 >> 1):
		case (0xC6 >> 1):
		case (0xC8 >> 1):
		case (0xCA >> 1):
		case (0xCC >> 1):
		case (0xCE >> 1):
			address <<= 1;
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
	_wire.beginTransmission(_i2caddr);
	_wire.write(REG_COMMAND);
	_wire.write(0xA0);
	delay(20);
	_wire.write(0xAA);
	delay(20);
	_wire.write(0xA5);
	delay(20);
	_wire.write(address);
	_last_error = _wire.endTransmission();
	return _last_error == 0;
};
