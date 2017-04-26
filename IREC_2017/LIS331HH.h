#ifndef _LIS331HH
#define _LIS331HH

#include <wire.h>
#include <Arduino.h>

class MEMS_Accel{
public:
// user functions
	void runAccel();
// output
	float x, y, z;
	unsigned long timeX, timeY, timeZ;
// register variables
	struct _REG1{
		byte Xenable   : 1;
		byte Yenable   : 1;
		byte Zenable   : 1;
		byte dataRate  : 2;
		byte powerMode : 3;
	} reg1;
	struct _REG2{
		byte HPCF  : 2;
		byte HPIN1 : 1;
		byte HPIN2 : 1;
		byte FDS   : 1;
		byte HPM   : 2;
		byte boot  : 1;
	} reg2;
	struct _REG3{
		byte I1_CFG  : 2;
		byte LIR1    : 1;
		byte I2_CFG  : 2;
		byte LIR2    : 1;
		byte PP_OD   : 1;
		byte IHL     : 1;
	} reg3;
	struct _REG4{
		byte SIM    : 1;
		byte ST     : 1;
		byte PAD    : 1;
		byte STsign : 1;
		byte FS     : 2;
		byte BLE    : 1;
		byte BDU    : 1;
	} reg4;
	struct _REG5{
		byte STW : 2;
		byte PAD : 6;
	} reg5;
	byte ref;
	struct _STATUS{
		byte ZYXOR : 1;
		byte ZOR   : 1;
		byte YOR   : 1;
		byte XOR   : 1;
		byte ZYXDA : 1;
		byte ZDA   : 1;
		byte YDA   : 1;
		byte XDA   : 1;
	} status;
	struct _OUT_XYZ{
		byte low;
		byte high;
	};
	union _ACCEL_X{
		struct _OUT_XYZ out_X;
		int16_t x;
	} accel_X;
	union _ACCEL_Y{
		struct _OUT_XYZ out_Y;
		int16_t y;
	} accel_Y;
	union _ACCEL_Z{
		struct _OUT_XYZ out_Z;
		int16_t z;
	} accel_Z;
	struct _INT_CFG{
		byte AOI : 1;
		byte _6D : 1;
		byte ZHIE : 1;
		byte ZLIE : 1;
		byte YHIE : 1;
		byte YLIE : 1;
		byte XHIE : 1;
		byte XLIE : 1;
	} 
	int1_CFG, int2_CFG;
	struct _INT_SOURCE{
		byte PAD : 1;
		byte IA : 1;
		byte ZH : 1;
		byte ZL : 1;
		byte YH : 1;
		byte YL : 1;
		byte XH : 1;
		byte XL : 1;
	} 
	int1_Source, int2_Source;
	struct _INT_THS{
		byte PAD : 1;
		byte THS6 : 1;
		byte THS5 : 1;
		byte THS4 : 1;
		byte THS3 : 1;
		byte THS2 : 1;
		byte THS1 : 1;
		byte THS0 : 1;
	} 
	int1_THS, int2_THS;
	struct _INT_DURATION{
		byte PAD : 1;
		byte duration : 7;
	} 
	int1_Duration, int2_Duration;
// constructor/destructor
	MEMS_Accel();
	~MEMS_Accel();
private:
// private methods
	void pullBytes(uint8_t adr, uint8_t* data, uint8_t size);
	void pushBytes(uint8_t adr, uint8_t* data, uint8_t size);
// I2C address
	static constexpr byte LIS331HH_ID = 0x18;
// Memory addresses
	static constexpr byte CTRL_REG1       = 0x20;
	static constexpr byte CTRL_REG2       = 0x21;
	static constexpr byte CTRL_REG3       = 0x22;
	static constexpr byte CTRL_REG4       = 0x23;
	static constexpr byte CTRL_REG5       = 0x24;
	static constexpr byte HP_FILTER_RESET = 0x25;
	static constexpr byte REFERENCE       = 0x26;
	static constexpr byte STATUS_REG      = 0x27;
	static constexpr byte OUT_X_L         = 0x28;
	static constexpr byte OUT_X_H         = 0x29;
	static constexpr byte OUT_Y_L         = 0x2A;
	static constexpr byte OUT_Y_H         = 0x2B;
	static constexpr byte OUT_Z_L         = 0x2C;
	static constexpr byte OUT_Z_H         = 0x2D;
	static constexpr byte INT1_CFG        = 0x30;
	static constexpr byte INT1_SOURCE     = 0x31;
	static constexpr byte INT1_THS        = 0x32;
	static constexpr byte INT1_DURATION   = 0x33;
	static constexpr byte INT2_CFG        = 0x34;
	static constexpr byte INT2_SOURCE     = 0x35;
	static constexpr byte INT2_THS        = 0x36;
	static constexpr byte INT2_DURATION   = 0x37;
};
#endif