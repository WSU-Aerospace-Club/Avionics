//#include "WSU_exceptions.h"
#include "EM_506.h"
#include <Arduino.h>
#include <WString.h>

// Constructor/ deconstructor

EM506::EM506(){
	Serial4.begin(4800);
	setQuery(0, 1, 0);
	setQuery(0, 1, 1);
	setQuery(0, 1, 2);
	setQuery(0, 0, 3);
	setQuery(0, 1, 4);
	setQuery(0, 1, 5);
	setQuery(0, 0, 6);
	setQuery(0, 1, 8);
}
EM506::~EM506(){}

// Public methods

EM506::Time EM506::getTime(){
	Time time;
	time.day = ZDAmsg.day;
	time.hour = ZDAmsg.UTC/10000;
	time.milSecond = (RMCmsg.UTC-(uint32_t)RMCmsg.UTC)*1000;
	time.min = (ZDAmsg.UTC%10000)/100;
	time.month = ZDAmsg.month;
	time.second = ZDAmsg.UTC%100;
	time.UTC = ZDAmsg.UTC;
	time.year = ZDAmsg.year;
	return time;
}
EM506::Loc EM506::getLoc(){
	Loc loc;
	if (GGAmsg.NS == 'N'){
		loc.latitude = (fmod(GGAmsg.latitude, 100) / 60 + (uint32_t)GGAmsg.latitude / 100) * 10000000;
	}
	else{
		loc.latitude = (-fmod(GGAmsg.latitude, 100) / 60 - (uint32_t)GGAmsg.latitude / 100) * 10000000;
	}
	if (GGAmsg.EW == 'E'){
		loc.longitude = (fmod(GGAmsg.longitude, 100) / 60 + (uint32_t)GGAmsg.longitude / 100) * 10000000;
	}
	else{
		loc.longitude = (-fmod(GGAmsg.longitude, 100) / 60 - (uint32_t)GGAmsg.longitude / 100) * 10000000;

	}
	loc.altitude = GGAmsg.altitude * 1000;
	return loc;
}
EM506::CourseSpeed EM506::getCourseSpeed(){
	CourseSpeed csp;
	csp.course1 = VTGmsg.course1;
	csp.course2 = VTGmsg.course2;
	csp.mode = VTGmsg.mode;
	csp.refCourse1 = VTGmsg.ref1;
	csp.refCourse2 = VTGmsg.ref2;
	csp.speed1 = VTGmsg.speed1;
	csp.speed2 = VTGmsg.speed2;
	csp.unitSpeed1 = VTGmsg.units1;
	csp.unitSpeed2 = VTGmsg.units2;
	return csp;
}
EM506::ActiveSat EM506::getActiveSat(){
	ActiveSat sat;
	sat.HDOP = GSAmsg.HDOP;
	sat.mode1 = GSAmsg.mode1;
	sat.mode2 = GSAmsg.mode2;
	sat.PDOP = GSAmsg.PDOP;
	sat.satCount = GGAmsg.numSats;
	sat.satList = NULL;
	sat.VDOP = GSAmsg.VDOP;
	return sat;
}

// commands 

void EM506::hotStartXY(EM506::Time* time, EM506::Loc* loc){

}
void EM506::hotStartXYZ(EM506::Time* time, EM506::Loc* loc){

}
void EM506::setPortA(){

}
void EM506::setPortB(){

}
void EM506::setQuery(uint8_t mode, uint8_t rate, uint8_t msg){
	// ?PSRF103,<msg [0-6, 8]>,<mode [0 = set rate, 1 = query]>,<rate [1-255 Hz]>,<cksumEnable [1 = on, 0 = off]>*CKSUM<CR><LF>
	// two digits per entry?
	String str = String("PSRF103,") + msg + ',' + mode + ',' + rate + ',' + 1;
	str = '$' + str + '*' + String(checksum(&str), HEX) + (char)0x0D + (char)0x0A;
	Serial4.print(str);
	//Serial.println(str);
}
void EM506::setDatum(){

}
void EM506::runGPS(){
	while (Serial4.available()){
		String str = Serial4.readStringUntil('\n');
		//Serial.println(str);
		String subStr = str.substring(0, str.indexOf(','));
		if (0 == strcmp(subStr.c_str(), "$GPGGA")){
			processGGA(&str);
			continue;
		}
		if (0 == strcmp(subStr.c_str(), "$GPGLL")){
			processGLL(&str);
			continue;
		}
		if (0 == strcmp(subStr.c_str(), "$GPGSA")){
			processGSA(&str);
			continue;
		}
		if (0 == strcmp(subStr.c_str(), "$GPGSV")){
			// processGSV(&str);
			continue;
		}
		if (0 == strcmp(subStr.c_str(), "$GPRMC")){
			processRMC(&str);
			continue;
		}
		if (0 == strcmp(subStr.c_str(), "$GPVTG")){
			processVTG(&str);
			continue;
		}
		if (0 == strcmp(subStr.c_str(), "$GPZDA")){
			processZDA(&str);
			continue;
		}
	}
}

// Private methods

void EM506::processGGA(String* str){
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.UTC = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.latitude = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.NS = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.longitude = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.EW = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.fixStatus = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.numSats = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.HDOP = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.altitude = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.unitsAlt = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.geoidSep = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.unitsGeoid = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.ageDGPS = str->substring(0, str->indexOf(',')).toInt();;
	str->remove(0, str->indexOf(',') + 1);
	GGAmsg.DGPSstation = str->substring(0, str->indexOf('*')).toInt();
	/*
	Serial1.print('\n');
	Serial1.println(GGAmsg.UTC);
	Serial1.print(GGAmsg.latitude);
	Serial1.println(GGAmsg.NS);
	Serial1.print(GGAmsg.longitude);
	Serial1.println(GGAmsg.EW);
	Serial1.println(GGAmsg.fixStatus);
	Serial1.println(GGAmsg.numSats);
	Serial1.println(GGAmsg.HDOP);
	Serial1.print(GGAmsg.altitude);
	Serial1.println(GGAmsg.unitsAlt);
	Serial1.println(GGAmsg.geoidSep);
	Serial1.println(GGAmsg.unitsGeoid);
	Serial1.println(GGAmsg.ageDGPS);
	Serial1.println(GGAmsg.DGPSstation);
	Serial1.print('\n');
	*/
}
void EM506::processGLL(String* str){
	str->remove(0, str->indexOf(',') + 1);
	GLLmsg.latitude = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GLLmsg.NS = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	GLLmsg.longitude = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GLLmsg.EW = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	GLLmsg.UTC = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GLLmsg.status = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	GLLmsg.mode = str->substring(0, str->indexOf('*')).charAt(0);
	/*
	Serial1.print('\n');
	Serial1.print(GLLmsg.latitude);
	Serial1.println(GLLmsg.NS);
	Serial1.print(GLLmsg.longitude);
	Serial1.println(GLLmsg.EW);
	Serial1.println(GLLmsg.UTC);
	Serial1.println(GLLmsg.status);
	Serial1.println(GLLmsg.mode);
	Serial1.print('\n');
	*/
}
void EM506::processGSA(String* str){
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.mode1 = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.mode2 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat1 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat2 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat3 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat4 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat5 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat6 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat7 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat8 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat9 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat10 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat11 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.sat12 = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.PDOP = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.HDOP = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	GSAmsg.VDOP = str->substring(0, str->indexOf('*')).toFloat();
	/*
	Serial1.print('\n');
	Serial1.println(GSAmsg.mode1);
	Serial1.println(GSAmsg.mode2);
	Serial1.println(GSAmsg.sat1);
	Serial1.println(GSAmsg.sat2);
	Serial1.println(GSAmsg.sat3);
	Serial1.println(GSAmsg.sat4);
	Serial1.println(GSAmsg.sat5);
	Serial1.println(GSAmsg.sat6);
	Serial1.println(GSAmsg.sat7);
	Serial1.println(GSAmsg.sat8);
	Serial1.println(GSAmsg.sat9);
	Serial1.println(GSAmsg.sat10);
	Serial1.println(GSAmsg.sat11);
	Serial1.println(GSAmsg.sat12);
	Serial1.println(GSAmsg.PDOP);
	Serial1.println(GSAmsg.HDOP);
	Serial1.println(GSAmsg.VDOP);
	Serial1.print('\n');
	*/
}
void EM506::processGSV(String* str){

}
void EM506::processRMC(String* str){
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.UTC = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.status = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.latitude = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.NS = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.longitude = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.EW = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.SOG = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.COG = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.date = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.magVar = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.magEW = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	RMCmsg.mode = str->substring(0, str->indexOf('*')).charAt(0);
}
void EM506::processVTG(String* str){
	str->remove(0, str->indexOf(',') + 1);
	VTGmsg.course1 = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	VTGmsg.ref1 = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	VTGmsg.course2 = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	VTGmsg.ref2 = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	VTGmsg.speed1 = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	VTGmsg.units1 = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	VTGmsg.speed2 = str->substring(0, str->indexOf(',')).toFloat();
	str->remove(0, str->indexOf(',') + 1);
	VTGmsg.units2 = str->substring(0, str->indexOf(',')).charAt(0);
	str->remove(0, str->indexOf(',') + 1);
	VTGmsg.mode = str->substring(0, str->indexOf('*')).charAt(0);
}
void EM506::processZDA(String* str){
	str->remove(0, str->indexOf(',') + 1);
	ZDAmsg.UTC = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	ZDAmsg.day = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	ZDAmsg.month = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	ZDAmsg.year = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	ZDAmsg.localHour = str->substring(0, str->indexOf(',')).toInt();
	str->remove(0, str->indexOf(',') + 1);
	ZDAmsg.localMin = str->substring(0, str->indexOf('*')).toInt();
}
uint8_t EM506::checksum(String* str){
	uint8_t cks = str->charAt(0);
	for (int i = 1; i < str->length();i++){
		cks = cks ^ str->charAt(i);
	}
	return cks;
}