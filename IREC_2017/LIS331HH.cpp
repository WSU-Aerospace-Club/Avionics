#include "LIS331HH.h"
#include <wire.h>
#include <Arduino.h>

MEMS_Accel::MEMS_Accel(){
	reg1.dataRate = 3;
	reg1.powerMode = 1;
	reg1.Xenable = 1;
	reg1.Yenable = 1;
	reg1.Zenable = 1;
	reg2.boot = 0;
	reg2.FDS = 0;
	reg2.HPCF = 0;
	reg2.HPIN1 = 0;
	reg2.HPIN2 = 0;
	reg2.HPM = 0;
	reg3.I1_CFG = 0;
	reg3.I2_CFG = 0;
	reg3.IHL = 0;
	reg3.LIR1 = 0;
	reg3.LIR2 = 0;
	reg3.PP_OD = 0;
	reg4.BDU = 0;
	reg4.BLE = 0;
	reg4.FS = 0;
	reg4.PAD = 0;
	reg4.SIM = 0;
	reg4.ST = 0;
	reg4.STsign = 0;
	reg5.PAD = 0;
	reg5.STW = 0;
	
	pushBytes(CTRL_REG1, (uint8_t*)&reg1, 1);
	pushBytes(CTRL_REG2, (uint8_t*)&reg2, 1);
	pushBytes(CTRL_REG3, (uint8_t*)&reg3, 1);
	pushBytes(CTRL_REG4, (uint8_t*)&reg4, 1);
	pushBytes(CTRL_REG5, (uint8_t*)&reg5, 1);

	pullBytes(OUT_X_L, &accel_X.out_X.low,1);
	pullBytes(OUT_X_H, &accel_X.out_X.high, 1);
	pullBytes(OUT_Y_L, &accel_Y.out_Y.low, 1);
	pullBytes(OUT_Y_H, &accel_Y.out_Y.high, 1);
	pullBytes(OUT_Z_L, &accel_Z.out_Z.low, 1);
	pullBytes(OUT_Z_H, &accel_Z.out_Z.high, 1);
}
MEMS_Accel::~MEMS_Accel(){

}
void MEMS_Accel::runAccel(){
	pullBytes(OUT_X_L, &accel_X.out_X.low, 1);
	pullBytes(OUT_X_H, &accel_X.out_X.high, 1);
	pullBytes(OUT_Y_L, &accel_Y.out_Y.low, 1);
	pullBytes(OUT_Y_H, &accel_Y.out_Y.high, 1);
	pullBytes(OUT_Z_L, &accel_Z.out_Z.low, 1);
	pullBytes(OUT_Z_H, &accel_Z.out_Z.high, 1);
}
void MEMS_Accel::pullBytes(uint8_t adr, uint8_t* data, uint8_t size){
	uint8_t i;
	Wire.beginTransmission(LIS331HH_ID);
	Wire.write(adr);
	Wire.endTransmission();
	Wire.requestFrom(LIS331HH_ID, size);
	Wire.available();
	while (Wire.available() != size);
	for (i = 0; i < size; i++){
		data[i] = Wire.read();
		//Serial.println(data[i]);
		//Serial.print("--> ");
		//Serial.print(i);
		//Serial.print(": ");
		//Serial.println(adr + i,HEX);
	}
}
void MEMS_Accel::pushBytes(uint8_t adr, uint8_t* data, uint8_t size){
	Wire.beginTransmission(LIS331HH_ID);
	Wire.write(adr);
	Wire.write(data, size);
	Wire.endTransmission();
}