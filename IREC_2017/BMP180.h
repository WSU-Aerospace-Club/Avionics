#ifndef _BMP180
#define _BMP180
#include <wire.h>
#include <Arduino.h>
#include "Parameters.h"

	class BMP180{
	public:
		//variables
		long pressure;
		long temperature;
		long seaLevelPressure;
		float altitude;
		byte runState;
		byte pressureCycles;
		//methods
		void runBarometer();
		byte getID();
		void startTemp();
		void startPressure();
		long pullTemp();
		long pullPressure();
		float pullAltitude();
		bool isDataReady();
		bool testComs();
		void setOverSample(byte OS);
		void softReset();
		//constructor/destructor
		BMP180(Parameters* vars);
		~BMP180();
		//constants
		const byte OVER_SAMPLE_LOW = 0x00;
		const byte OVER_SAMPLE_STD = 0x01;
		const byte OVER_SAMPLE_HIGH = 0x02;
		const byte OVER_SAMPLE_ULTRA = 0x03;
	private:
		// methods
		bool pullCalData();
		void pushBytes(byte data, uint8_t length, uint8_t adr);
		void pullBytes(byte* data, uint8_t length, uint8_t adr);
		// variables
		int16_t ac1, ac2, ac3, b1, b2, b5, mb, mc, md;
		uint16_t ac4, ac5, ac6;
		byte overSample; // default OVERSAMPLE_STD
		// commands
		const byte READ_UNCP_TEMP = 0x2E;
		const byte READ_UNCP_PRSR = 0x34; // add (oversample<<6) when using
		const byte SOFT_RESET = 0xB6;
		// Address constants
		const uint8_t BMP180_ID = 0x77;
		const byte BMP180_REG_MSB = 0xF6;
		const byte BMP180_REG_LSB = 0xF7;
		const byte BMP180_REG_XSB = 0xF8;
		const byte BMP180_REG_CTR_MSR = 0xF4;
		const byte BMP180_REG_SFT_RST = 0xE0;
		const byte BMP180_REG_ID = 0xD0;
		const byte BMP180_REG_CAL0 = 0xAA; // calibration data; CAL0 to CAL21, 0xAA to 0xBF
		const byte BMP180_REG_AC1 = 0xAA;
		const byte BMP180_REG_AC2 = 0xAC;
		const byte BMP180_REG_AC3 = 0xAE;
		const byte BMP180_REG_AC4 = 0xB0;
		const byte BMP180_REG_AC5 = 0xB2;
		const byte BMP180_REG_AC6 = 0xB4;
		const byte BMP180_REG_B1 = 0xB6;
		const byte BMP180_REG_B2 = 0xB8;
		const byte BMP180_REG_MB = 0xBA;
		const byte BMP180_REG_MC = 0xBC;
		const byte BMP180_REG_MD = 0xBE;
	};
#endif