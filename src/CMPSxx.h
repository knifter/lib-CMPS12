#ifndef __CMPSxx_H
#define __CMPSxx_H

#include <Arduino.h>
#include <TwoWireDevice.h>

// CONFIG
#define CMPSxx_ADDRESS_DEFAULT      (0xC0 >> 1)

class CMPS12 : public TwoWireDevice
{
	public:
		CMPS12(TwoWire& wire, const uint8_t addr = CMPSxx_ADDRESS_DEFAULT)  : TwoWireDevice(wire, addr) {};
		CMPS12(const uint8_t addr = CMPSxx_ADDRESS_DEFAULT) : TwoWireDevice(addr) {};
        ~CMPS12() {};

		bool begin();

		uint8_t getCalStatRaw();
		float getBearing();
		int8_t getTemp();
		float getPitch();
		float getRoll();

		bool storeCal();
		bool eraseCal();
		bool setAddress(uint8_t address);

		typedef enum {
			CAL_HIGH,
			CAL_MED,
			CAL_LOW
		} calstat_t;
		calstat_t getCalStat();

	private:
		typedef enum : uint8_t
		{
			REG_COMMAND = 0x00,
			// REG_BEARING255 = 0x01,
			REG_BEARING16 = 0x02,
			REG_PITCH90 = 0x04,
			REG_ROLL90 = 0x05,
			// REG_MAGNRAW_X = 0x06,
			// REG_MAGNRAW_Y = 0x08,
			// REG_MAGNRAW_Z = 0x0A,
			// REG_ACCRAW_X = 0x0C,
			// REG_ACCRAW_Y = 0x0E,
			// REG_ACCRAW_Z = 0x10,
			// REG_GYRORAW_X = 0x12,
			// REG_GYRORAW_Y = 0x14,
			// REG_GYRORAW_Z = 0x16,
			REG_TEMP = 0x18,
			// REG_BEARING_BOSCH = 0x1A,
			// REG_PITCH16 = 0x1C,
			REG_CALSTAT = 0x1E,
		} _cmps12_regs_t;

		CMPS12(const CMPS12&);
		CMPS12& operator=(const CMPS12&);
};

class CMPS14 : public TwoWireDevice
{
	public:
		CMPS14(TwoWire& wire, const uint8_t addr = CMPSxx_ADDRESS_DEFAULT)  : TwoWireDevice(wire, addr) {};
		CMPS14(const uint8_t addr = CMPSxx_ADDRESS_DEFAULT) : TwoWireDevice(addr) {};
        ~CMPS14() {};

		bool begin();

		float getBearing();
		float getPitch();
		float getRoll();

		typedef enum {
			CAL_HIGH,
			CAL_MED,
			CAL_LOW
		} calstat_t;
		calstat_t getCalStat();

		bool storeCal();
		bool eraseCal();
		bool calConfig(bool periodic_save, bool gyrocal, bool accelcal, bool magcal);
		bool setAddress(uint8_t address);

	private:
		typedef enum : uint8_t
		{
			REG_COMMAND = 0x00,
			// REG_BEARING255 = 0x01,
			REG_BEARING16 = 0x02,
			// REG_PITCH90 = 0x04,
			// REG_ROLL90 = 0x05,
			// REG_MAGN_X = 0x06,
			// REG_MAGN_Y = 0x08,
			// REG_MAGN_Z = 0x0A,
			// REG_LINACC_X = 0x0C,
			// REG_LINACC_Y = 0x0E,
			// REG_LINACC_Z = 0x10,
			// REG_GYRORAW_X = 0x12,
			// REG_GYRORAW_Y = 0x14,
			// REG_GYRORAW_Z = 0x16,
			REG_ROLL16 = 0x1C,
			REG_CALSTAT = 0x1E,
			// V5+ commands
			REG_PITCH16 = 0x1A,
			// REG_ACC16_X = 0x1F,
			// REG_ACC16_Y = 0x21,
			// REG_ACC16_Z = 0x23,
			// REG_GYRO16_X = 0x25,
			// REG_GYRO16_X = 0x27,
			// REG_GYRO16_X = 0x29,
		} _cmps14_regs_t;

};

#endif // __CMPS12_H