#ifndef _L3GD20TR
#define _L3GD20TR
#include <Arduino.h>

class MEMS_Gyro{
public:
// User Functions
	void runGyro();
// Output
	float x, y, z;
	float roll, pitch, yaw;
	float scale;
	float offset_x, offset_y, offset_z;
	unsigned long timeX, timeY, timeZ;
// registers
	byte who_am_I;
	struct _CTRL_REG1{
		byte Yen : 1;
		byte Xen : 1;
		byte Zen : 1;
		byte PD  : 1;
		byte BW  : 2;
		byte DR  : 2;
	} reg1;
	struct _CTRL_REG2{
		byte HPCF : 4;
		byte HPM  : 2;
		byte PAD  : 2;
	} reg2;
	struct _CTRL_REG3{
		byte I2_Empty  : 1;
		byte I2_ORun   : 1;
		byte I2_WTM    : 1;
		byte I2_DRDY   : 1;
		byte PP_OD     : 1;
		byte H_Lactive : 1;
		byte I1_Boot   : 1;
		byte I1_int1   : 1;
	} reg3;
	struct _CTRL_REG4{
		byte SIM : 1;
		byte PAD : 3;
		byte FS  : 2;
		byte BLE : 1;
		byte BDU : 1;
	} reg4;
	struct _CTRL_REG5{
		byte Out_Sel  : 2;
		byte INT1_Sel : 2;
		byte HPen     : 1;
		byte PAD      : 1;
		byte FIFO_EN  : 1;
		byte BOOT     : 1;
	} reg5;
	byte reference;
	int8_t temperature;
	struct _STATUS{
		byte XDA   : 1;
		byte YDA   : 1;
		byte ZDA   : 1;
		byte ZYXDA : 1;
		byte XOR   : 1;
		byte YOR   : 1;
		byte ZOR   : 1;
		byte ZYXOR : 1;
	} status;
	typedef struct _OUT_XYZ{
		byte low;
		byte high;
	} out_XYZ;
	union _RATE_X{
		out_XYZ out_x;
		int16_t x;
	} rate_x;
	union _RATE_Y{
		out_XYZ out_y;
		int16_t y;
	} rate_y;
	union _RATE_Z{
		out_XYZ out_z;
		int16_t z;
	} rate_z;
	struct _FIFO_CTRL_REG{
		byte WTM : 5;
		byte FM  : 3;
	}fifo_ctrl_reg;
	struct _FIFO_SRC_REG{
		byte FSS   : 5;
		byte EMPTY : 1;
		byte OVRN  : 1;
		byte WTM   : 1;
	} fifo_src_reg;
	struct _INT_CFG{
		byte XLIE   : 1;
		byte XHIE   : 1;
		byte YLIE   : 1;
		byte YHIE   : 1;
		byte ZLIE   : 1;
		byte ZHIE   : 1;
		byte LIR    : 1;
		byte AND_OR : 1;
	} int1_cfg;
	struct _INT_SRC{
		byte XL  : 1;
		byte XH  : 1;
		byte YL  : 1;
		byte YH  : 1;
		byte ZL  : 1;
		byte ZH  : 1;
		byte IA  : 1;
		byte PAD : 1;
	} int1_src;
	typedef struct _INT_THS{
		byte low  : 8;
		byte high : 7;
		byte PAD  : 1;
	} int_ths;
	union{
		int_ths reg;
		uint16_t threshold;
	}
	int_ths_X, int_ths_Y, int_ths_Z;
	struct _INT_DURATION{
		byte D    : 7;
		byte WAIT : 1;
	}int_duration;
// constructor/ destructor
	MEMS_Gyro();
	~MEMS_Gyro();
private:
// private methods
	void pushBytes(uint8_t adr, uint8_t* data, uint8_t size);
	void pullBytes(uint8_t adr, uint8_t* data, uint8_t size);
// I2C address
	static constexpr byte L3GD20TR_ID = 0x6A;
// memory addresses
	static constexpr byte WHO_AM_I      = 0x0F;
	static constexpr byte CTRL_REG1     = 0x20;
	static constexpr byte CTRL_REG2     = 0x21;
	static constexpr byte CTRL_REG3     = 0x22;
	static constexpr byte CTRL_REG4     = 0x23;
	static constexpr byte CTRL_REG5     = 0x24;
	static constexpr byte REFERENCE     = 0x25;
	static constexpr byte OUT_TEMP      = 0x26;
	static constexpr byte STATUS_REG    = 0x27;
	static constexpr byte OUT_X_L       = 0x28;
	static constexpr byte OUT_X_H       = 0x29;
	static constexpr byte OUT_Y_L       = 0x2A;
	static constexpr byte OUT_Y_H       = 0x2B;
	static constexpr byte OUT_Z_L       = 0x2C;
	static constexpr byte OUT_Z_H       = 0x2D;
	static constexpr byte FIFO_CTRL_REG = 0x2E;
	static constexpr byte FIFO_SRC_REG  = 0x2F;
	static constexpr byte INT1_CFG      = 0x30;
	static constexpr byte INT1_SRC      = 0x31;
	static constexpr byte INT1_THS_XH   = 0x32;
	static constexpr byte INT1_THS_XL   = 0x33;
	static constexpr byte INT1_THS_YH   = 0x34;
	static constexpr byte INT1_THS_YL   = 0x35;
	static constexpr byte INT1_THS_ZH   = 0x36;
	static constexpr byte INT1_THS_ZL   = 0x37;
	static constexpr byte INT1_DURATION = 0x38;
};

#endif