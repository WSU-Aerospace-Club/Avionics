#include "L3GD20TR.h"
#include <Wire.h>
#include <Arduino.h>

MEMS_Gyro::MEMS_Gyro(){
	reg1.BW = 0;
	reg1.DR = 0;
	reg1.PD = 1;
	reg1.Xen = 1;
	reg1.Yen = 1;
	reg1.Zen = 1;
	reg2.HPCF = 0;
	reg2.HPM = 0;
	reg2.PAD = 0;
	reg3.H_Lactive = 0;
	reg3.I1_Boot = 0;
	reg3.I1_int1 = 0;
	reg3.I2_DRDY = 0;
	reg3.I2_Empty = 0;
	reg3.I2_ORun = 0;
	reg3.I2_WTM = 0;
	reg3.PP_OD = 0;
	reg4.BDU = 0;
	reg4.BLE = 0;
	reg4.FS = 0;
	reg4.PAD = 0;
	reg4.SIM = 0;
	reg5.BOOT = 0;
	reg5.FIFO_EN = 0;
	reg5.HPen = 0;
	reg5.INT1_Sel = 0;
	reg5.Out_Sel = 0;
	reg5.PAD = 0;
	reference = 0;
	fifo_ctrl_reg.FM = 0;
	fifo_ctrl_reg.WTM = 0;
	int1_cfg.AND_OR = 0;
	int1_cfg.LIR = 0;
	int1_cfg.XHIE = 0;
	int1_cfg.XLIE = 0;
	int1_cfg.YHIE = 0;
	int1_cfg.YLIE = 0;
	int1_cfg.ZHIE = 0;
	int1_cfg.ZLIE = 0;
	int_ths_X.threshold = 0;
	int_ths_X.reg.PAD = 0;
	int_ths_Y.threshold = 0;
	int_ths_Y.reg.PAD = 0;
	int_ths_Z.threshold = 0;
	int_ths_Z.reg.PAD = 0;
	int_duration.D = 0;
	int_duration.WAIT = 0;
	offset_x = .982674;
	offset_y = .82173;
	offset_z = 0;
	roll = 0;
	pitch = 0;
	yaw = 0;
	pushBytes(CTRL_REG1, (uint8_t*)&reg1, 1);
	pushBytes(CTRL_REG2, (uint8_t*)&reg2, 1);
	pushBytes(CTRL_REG3, (uint8_t*)&reg3, 1);
	pushBytes(CTRL_REG4, (uint8_t*)&reg4, 1);
	pushBytes(CTRL_REG5, (uint8_t*)&reg5, 1);
	pushBytes(REFERENCE, &reference, 1);
	pushBytes(FIFO_CTRL_REG, (uint8_t*)&fifo_ctrl_reg, 1);
	pushBytes(INT1_CFG, (uint8_t*)&int1_cfg, 1);
	pushBytes(INT1_THS_XH, (uint8_t*)&int_ths_X, 1);
	pushBytes(INT1_THS_XL, (uint8_t*)(&int_ths_X+1), 1);
	pushBytes(INT1_THS_YH, (uint8_t*)&int_ths_Y, 1);
	pushBytes(INT1_THS_YL, (uint8_t*)(&int_ths_Y + 1), 1);
	pushBytes(INT1_THS_ZH, (uint8_t*)&int_ths_Z, 1);
	pushBytes(INT1_THS_ZL, (uint8_t*)(&int_ths_Z + 1), 1);
	pushBytes(INT1_DURATION, (uint8_t*)&int_duration, 1);
	switch (reg4.FS){
		case 0:
			scale = 250;
			break;
		case 1:
			scale = 500;
			break;
		default:
			scale = 2000;
	};

	pullBytes(OUT_X_L, &rate_x.out_x.low, 1);
	pullBytes(OUT_X_H, &rate_x.out_x.high, 1);
	x = scale/0x8000 * rate_x.x - offset_x;
	timeX = micros();
	pullBytes(OUT_Y_L, &rate_y.out_y.low, 1);
	pullBytes(OUT_Y_H, &rate_y.out_y.high, 1);
	y = scale / 0x8000 * rate_y.y- offset_y;
	timeY = micros();
	pullBytes(OUT_Z_L, &rate_z.out_z.low, 1);
	pullBytes(OUT_Z_H, &rate_z.out_z.high, 1);
	z = scale / 0x8000 * rate_z.z - offset_z;
	timeZ = micros();
	pullBytes(OUT_TEMP, (uint8_t*)&temperature, 1);
}
MEMS_Gyro::~MEMS_Gyro(){

}
void MEMS_Gyro::pullBytes(uint8_t adr, uint8_t* data, uint8_t size){
	uint8_t i;
	Wire.beginTransmission(L3GD20TR_ID);
	Wire.write(adr);
	Wire.endTransmission();
	Wire.requestFrom(L3GD20TR_ID, size);
	Wire.available();
	while (Wire.available() != size);
	for (i = 0; i < size; i++){
		data[i] = Wire.read();
	}
}
void MEMS_Gyro::pushBytes(uint8_t adr, uint8_t* data, uint8_t size){
	Wire.beginTransmission(L3GD20TR_ID);
	Wire.write(adr);
	Wire.write(data, size);
	Wire.endTransmission();
}
void MEMS_Gyro::runGyro(){
	float x1, y1, z1;
	unsigned long timeX1, timeY1, timeZ1;

	pullBytes(OUT_X_L, &rate_x.out_x.low, 1);
	pullBytes(OUT_X_H, &rate_x.out_x.high, 1);
	timeX1 = micros();
	x1 = scale / 0x8000 * rate_x.x - offset_x;
	pullBytes(OUT_Y_L, &rate_y.out_y.low, 1);
	pullBytes(OUT_Y_H, &rate_y.out_y.high, 1);
	timeY1 = micros();
	y1 = scale / 0x8000 * rate_y.y - offset_y;
	pullBytes(OUT_Z_L, &rate_z.out_z.low, 1);
	pullBytes(OUT_Z_H, &rate_z.out_z.high, 1);
	timeZ1 = micros();
	z1 = scale / 0x8000 * rate_z.z - offset_z;

	float dt = timeX1 - timeX;
	roll = (x1 + x)*.00000000872664625*dt + roll;
	dt = timeY1 - timeY;
	pitch = (y1 + y)*.00000000872664625*dt + pitch;
	dt = timeZ1 - timeZ;
	yaw = (z1 + z)*.00000000872664625*dt + yaw;

	x = x1;
	y = y1;
	z = z1;
	timeX = timeX1;
	timeY = timeY1;
	timeZ = timeZ1;
}