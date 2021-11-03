#ifndef __CMPS12_H
#define __CMPS12_H

#include <Arduino.h>
#include <TwoWireDevice.h>

// CONFIG
#define CMPS12_ADDRESS_DEFAULT      (0xC0 >> 1)

typedef enum {
	CAL_HIGH,
	CAL_MED,
	CAL_LOW
} calstat_t;

/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with the CMPS12 compass/attitude sensor
*/
/**************************************************************************/
class CMPS12 : public TwoWireDevice 
{
	public:
		//constructors
		CMPS12(TwoWire& wire, const uint8_t addr = CMPS12_ADDRESS_DEFAULT)  : TwoWireDevice(wire, addr) {};
		CMPS12(const uint8_t addr = CMPS12_ADDRESS_DEFAULT) : TwoWireDevice(addr) {};
        ~CMPS12() {};

		bool begin();
		uint8_t getCalStatRaw();
		calstat_t getCalStat();
		float getBearing();
		float getPitch();
		float getRoll();
		int8_t getTemp();

		bool storeCal();
		bool eraseCal();
		bool setAddress(uint8_t address);

    protected:
		typedef enum : uint8_t
		{
			REG_COMMAND = 0,
			REG_BEARING255 = 1,
			REG_BEARING16 = 2,
			REG_PITCH = 4,
			REG_ROLL = 5,
			REG_TEMP = 25,
			REG_PITCH16 = 28,
			REG_CALSTAT = 30
		} _cmps12_regs_t;

	private:
		CMPS12(const CMPS12&);
		CMPS12& operator=(const CMPS12&);
};

#endif // __CMPS12_H