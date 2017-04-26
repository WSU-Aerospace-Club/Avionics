#ifndef _EM_506
#define _EM_506
#include <WString.h>
#include <stdint.h>
#include "WSU_exceptions.h"

class EM506{
public:
// user structures
	typedef struct _Time{
		short int year;
		short int month;
		short int day;
		short int hour;
		short int min;
		short int second;
		int milSecond;
		float UTC;
	} Time;
	typedef struct _Loc{
		int32_t latitude;
		int32_t longitude;
		int32_t altitude;
	} Loc;
	typedef struct _ActiveSat{
		short int* satList;
		short int satCount;
		short int mode1;
		short int mode2;
		float PDOP;
		float HDOP;
		float VDOP;
	} ActiveSat;
	typedef struct _SatData{
		short int satID;
		short int elevation;
		int azimuth;
		short int SNR;
	} SatData;
	typedef struct _CourseSpeed{
		float course1;
		char refCourse1;
		float course2;
		char refCourse2;
		float speed1;
		char unitSpeed1;
		float speed2;
		char unitSpeed2;
		short int mode;
	} CourseSpeed;
// user methods
	void runGPS();
	Time getTime();
	Loc getLoc();
	CourseSpeed getCourseSpeed();
	bool getStatus();
	ActiveSat getActiveSat();
	SatData* getSatData(); // pull activeSat to get size first
	void hotStartXY(Time* time, Loc* loc);
	void hotStartXYZ(Time* time, Loc* loc);
	void setPortA();
	void setPortB();
	void setQuery(uint8_t mode, uint8_t rate, uint8_t msg);
	void setDatum();
	void setDebug(bool onOff);
// constructor/destructor
	EM506();
	~EM506();
private:
// GPS message structures
	typedef struct _GGA{
		float UTC;
		float latitude;
		char NS;
		float longitude;
		char EW;
		short int fixStatus;
		short int numSats;
		float HDOP;
		float altitude;
		char unitsAlt;
		float geoidSep;
		char unitsGeoid;
		int ageDGPS;
		short int DGPSstation;
	} GGA;
	typedef struct _GLL{
		float latitude;
		char NS;
		float longitude;
		char EW;
		float UTC;
		char status;
		char mode;
	} GLL;
	typedef struct _GSA{
		char mode1;
		uint8_t mode2;
		uint16_t sat1;
		uint16_t sat2;
		uint16_t sat3;
		uint16_t sat4;
		uint16_t sat5;
		uint16_t sat6;
		uint16_t sat7;
		uint16_t sat8;
		uint16_t sat9;
		uint16_t sat10;
		uint16_t sat11;
		uint16_t sat12;
		float PDOP;
		float HDOP;
		float VDOP;
	} GSA;
	typedef struct _GSV{
	} GSV;
	typedef struct _RMC{
		float UTC;
		char status;
		float latitude;
		char NS;
		float longitude;
		char EW;
		float SOG;
		float COG;
		uint32_t date;
		float magVar;
		char magEW;
		char mode;
	} RMC;
	typedef struct _VTG{
		float course1;
		char ref1;
		float course2;
		char ref2;
		float speed1;
		char units1;
		float speed2;
		char units2;
		char mode;
	} VTG;
	typedef struct _ZDA{
		uint32_t UTC;
		uint8_t day;
		uint8_t month;
		uint16_t year;
		uint8_t localHour;
		uint8_t localMin;
	} ZDA;
// last messages
	GGA GGAmsg;
	GLL GLLmsg;
	GSA GSAmsg;
	GSV GSVmsg;
	RMC RMCmsg;
	VTG VTGmsg;
	ZDA ZDAmsg;
// private methods
	void processGGA(String* str);
	void processGLL(String* str);
	void processGSA(String* str);
	void processGSV(String* str);
	void processRMC(String* str);
	void processVTG(String* str);
	void processZDA(String* str);
	uint8_t checksum(String* str);
};
#endif
