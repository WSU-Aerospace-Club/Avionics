
#include "BMP180.h"
#include <Arduino.h>
#include <wire.h>
#include "Parameters.h"
	BMP180::BMP180(Parameters* vars){
		overSample = OVER_SAMPLE_ULTRA;
		runState = 0;
		pressureCycles = 10;
		seaLevelPressure = 101325;
		pullCalData();
		/*
		Serial.print("AC1: ");
		Serial.println(ac1);
		Serial.print("AC2: ");
		Serial.println(ac2);
		Serial.print("AC3: ");
		Serial.println(ac3);
		Serial.print("AC4: ");
		Serial.println(ac4);
		Serial.print("AC5: ");
		Serial.println(ac5);
		Serial.print("AC6: ");
		Serial.println(ac6);
		Serial.print("B1: ");
		Serial.println(b1);
		Serial.print("B2: ");
		Serial.println(b2);
		Serial.print("MB: ");
		Serial.println(mb);
		Serial.print("MC: ");
		Serial.println(mc);
		Serial.print("MD: ");
		Serial.println(md);
		*/
		startTemp();
		delay(100);
		pullTemp();
		delay(100);
		startPressure();
		delay(100);
		pullAltitude();
		vars->homeAlt = altitude;
		return;
	}
	BMP180::~BMP180(){
	};
	void BMP180::runBarometer(){
		/*
		start temp, get temp, start pressure, get altitude
		*/
		switch (runState){
			case 0: // start temp
				startTemp();
				runState = 1;
				break;
			case 1: // get temp
				if (isDataReady()){
					pullTemp();
					runState = 2;
					runBarometer();
				}
				break;
			case 2: // start pressure
				startPressure();
				runState = 3;
				break;
			case 3: // get altitude
				if (isDataReady()){
					pullAltitude();
					if (pressureCycles > 0){
						runState = 2;
						pressureCycles--;
					}
					else{
						runState = 0;
						pressureCycles = 10;
					}
					runBarometer();
				}
				break;
		}
	}
	byte BMP180::getID(){
		return BMP180_ID;
	};
	void BMP180::startTemp(){
		pushBytes(READ_UNCP_TEMP + (overSample << 6), (uint8_t)1, BMP180_REG_CTR_MSR);
	};
	void BMP180::startPressure(){
		pushBytes(READ_UNCP_PRSR + (overSample << 6), (uint8_t)1, BMP180_REG_CTR_MSR);
	};
	long BMP180::pullTemp(){
		byte MSB;
		byte LSB;
		pullBytes(&MSB,(uint8_t)1,BMP180_REG_MSB);
		pullBytes(&LSB,(uint8_t)1, BMP180_REG_LSB);
		long UT = (long)(MSB << 8) | (long)LSB;
		//Serial.print("UT: ");
		//Serial.println(UT);
		long X1 = (UT - ac6)*ac5 / 0x8000;
		long X2 = mc*(0x800) / (X1 + md);
		b5 = X1 + X2;
		temperature = (b5 + 8) / 0x10;
		return temperature;
	};
	long BMP180::pullPressure(){
		byte MSB;
		byte LSB;
		byte XSB;
		pullBytes(&MSB, (uint8_t)1, BMP180_REG_MSB);
		pullBytes(&LSB, (uint8_t)1, BMP180_REG_LSB);
		pullBytes(&XSB, (uint8_t)1, BMP180_REG_XSB);
		long UP = (((unsigned long)MSB << 16) + ((unsigned long)LSB << 8) + (unsigned long)XSB) >> (8 - overSample);
		//Serial.print("UP: ");
		//Serial.println(UP);
		long B6 = b5-4000;
		long X1 = (long)b2*(B6*B6/0x1000)/0x800;
		long X2 = (long)ac2*B6/0x800;
		long X3 = X1 + X2;
		long B3 = ((((long)ac1 * 4 + X3) << overSample) + 2) / 4;
		X1 = (long)ac3*B6/0x2000;
		X2 = ((long)b1*(B6*B6/0x1000))/0x10000;
		X3 = ((X1+X2)+2)/4;
		unsigned long B4 = (unsigned long)ac4*(unsigned long)(X3 + 0x8000) / 0x8000;
		unsigned long B7 = ((unsigned long)UP-B3)*(0xC350>>overSample);
		if (B7 < 0x80000000){
			pressure = B7 * 2 / B4;
		}
		else{
			pressure = B7 / B4 * 2;
		}
		X1 = (pressure/0x100)*(pressure/0x100);
		X1 = (X1 * 0xBDE) / 0x10000;
		X2 = (-0x1CBD * pressure) / 0x10000;
		pressure = pressure + (X1 + X2 + 0xECF) / 0x10;
		return pressure;
	};
	float BMP180::pullAltitude(){
		altitude = 44330 * (1.0 - pow((float)pullPressure()/(float)seaLevelPressure, (float)(1.0 / 5.255)));
		return altitude;
	};
	bool BMP180::testComs(){
		byte i;
		pullBytes(&i, (uint8_t)1, BMP180_REG_ID);
		if (i == BMP180_ID){
			return true;
		}
		else{
			return false;
		}
	};
	void BMP180::setOverSample(byte OS){
		overSample = OS;
	};
	void BMP180::softReset(){
		pushBytes(SOFT_RESET, (uint8_t)1, BMP180_REG_SFT_RST);
		return;
	};
	bool BMP180::pullCalData(){
		uint8_t data[2];
		pullBytes(data, (uint8_t)2, BMP180_REG_AC1);
		ac1 = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_AC2);
		ac2 = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_AC3);
		ac3 = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_AC4);
		ac4 = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_AC5);
		ac5 = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_AC6);
		ac6 = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_B1);
		b1 = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_B2);
		b2 = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_MB);
		mb = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_MC);
		mc = ((int16_t)data[0] << 8) + (int16_t)data[1];
		pullBytes(data, (uint8_t)2, BMP180_REG_MD);
		md = ((int16_t)data[0] << 8) + (int16_t)data[1];
	};
	void BMP180::pullBytes(byte* data, uint8_t length, uint8_t adr){
		uint8_t i;
		Wire.beginTransmission(BMP180_ID);
		Wire.write(adr);
		Wire.endTransmission();
		Wire.requestFrom(BMP180_ID, length);
		while (Wire.available() != length);
		for (i = 0; i < length; i++){
			data[i] = Wire.read();
			//Serial.println(data[i]);
			//Serial.print("--> ");
			//Serial.print(i);
			//Serial.print(": ");
			//Serial.println(adr + i,HEX);
		}
		return;
	};
	void BMP180::pushBytes(byte data, uint8_t length, uint8_t adr){
		byte *ptr = &data;
		Wire.beginTransmission(BMP180_ID);
		Wire.write(adr);
		Wire.write(ptr, length);
		Wire.endTransmission();
		return;
	};
	bool BMP180::isDataReady(){
		byte i;
		pullBytes(&i, (uint8_t)1, BMP180_REG_CTR_MSR);
		if (! ( (i >> 5) ^ (overSample << 1) ) ){
			return true;
		}
		else{
			return false;
		}
	};