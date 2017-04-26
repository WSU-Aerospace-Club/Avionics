#include "DataLogger.h"
#include "Parameters.h"
#include <SD.h>

Parameters::param_element* DataLogger::readParam(uint16_t* count){
	File filePtr = SD.open(paramName, FILE_READ);
	if (!filePtr){
		//Serial1.println("SD_PARAM_OPEN_READ_FAIL");
	}
	else{
		uint16_t i = 0;
		while (filePtr.available()){
			filePtr.readStringUntil(';');
			i++;
		}
		i=i-1;
		//i = 30;
		filePtr.seek(0);
		//Parameters::param_element element[30];
		Parameters::param_element* first_element;
		Parameters::param_element* element;
		first_element = new Parameters::param_element;
		element = first_element;
		for (uint16_t j = 0; j < i; j++){	
			String str = filePtr.readStringUntil(';');
			//str.substring(0,(uint32_t)remove_if(&str, &str + str.length(), isspace)-(uint32_t)&str);
			strcpy(element->ID, str.substring(0, str.indexOf(',')).c_str());
			//Serial1.println(element->ID);
			element->ID[17] = NULL;
			str.remove(0, str.indexOf(',')+1);
			element->type = str.substring(0, str.indexOf(',')).toInt();
			//Serial1.println(element->type);
			str.remove(0, str.indexOf(',')+1);
			element->value = str.substring(0, str.indexOf(';')).toFloat();
			//Serial1.println(element->value);
			filePtr.read();
			element->nxt = new Parameters::param_element;
			element = element->nxt;
			element->nxt = NULL;
		}
		*count = i;
		return first_element;
	}
	return NULL;
}
void DataLogger::writeParam(Parameters* pars){
	Parameters::_param_element* ptr = pars->parameter_list;
	SD.remove(paramName);
	File filePtr = SD.open(paramName, FILE_WRITE);
	if (!filePtr){
		//Serial1.println("SD_PARAM_OPEN_WRITE_FAIL");
	}
	else{
		for (uint16_t i = 0; i < pars->getCount(); i++){
			filePtr.print(ptr->ID);
			filePtr.print(", ");
			filePtr.print(ptr->type);
			filePtr.print(", ");
			filePtr.print(ptr->value);
			filePtr.print(";\n");
#ifdef _WSU_MAVLINK_TEST
			Serial.println("PARAMETER_WRITE");
			Serial.println(ptr->ID);
			Serial.println(ptr->type);
			Serial.println(ptr->value);
#endif
			ptr = ptr->nxt;
		}
		filePtr.close();
	}
}
void DataLogger::readLog(){
	File filePtr = SD.open(logName, FILE_READ);
	if (!filePtr){
		//Serial1.println("SD_LOG_OPEN_READ_FAIL");
	}
	else{
	}
}
void DataLogger::writeLog(char * log){
	File filePtr = SD.open(logName, FILE_WRITE);
	if (!filePtr){
		//Serial1.println("SD_LOG_OPEN_WRITE_FAIL");
	}
	else{
		filePtr.println(log);
		filePtr.close();
	}
}
DataLogger::DataLogger(){
	pinMode(chipEnable, OUTPUT);
	digitalWrite(chipEnable, HIGH);
	if (!SD.begin(chipEnable)){
		//Serial1.println("SD_INIT_FAIL");
	}
}
DataLogger::~DataLogger(){

}