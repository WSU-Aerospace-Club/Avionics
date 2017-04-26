#include <MAVLINK\ardupilotmega\mavlink.h>
#include "Parameters.h"
#include <Arduino.h>
#include "DataLogger.h"


char* Parameters::getParamID(uint16_t param){
	if (param < param_count){
		param_element * ptr = parameter_list;
		for (;param > 0;param--){
			ptr = ptr->nxt;
		}
		return ptr->ID;
	}
	else{
		return NULL;
	}
};
uint8_t Parameters::getParamType(uint16_t param){
	if (param < param_count){
		param_element * ptr = parameter_list;
		for (; param > 0; param--){
			ptr = ptr->nxt;
		}
		return ptr->type;
	}
	else{
		return NULL;
	}
};
float Parameters::getParamValue(uint16_t param){
	if (param < param_count){
		param_element * ptr = parameter_list;
		for (; param > 0; param--){
			ptr = ptr->nxt;
		}
		return ptr->value;
	}
	else{ 
		return NULL;
	}
};
uint16_t Parameters::getCount(){
	return param_count;
};
int16_t Parameters::setParameter(char* ID, uint8_t type, float value){
	_param_element* ptr = parameter_list;
	for (int16_t i = 0; i < getCount(); i++){
		if (0 == strcmp(ID, ptr->ID)){
			ptr->value = value;
			ptr->type = type;
			saveParameters();
			return i;
		}
		ptr = ptr->nxt;
	}
	return false;
}
void Parameters::saveParameters(){
	DataLogger::writeParam(this);
}
Parameters::~Parameters(){
	param_element* ptr = parameter_list;
	param_element* nxt;
	while (ptr->nxt != NULL){
		nxt = ptr->nxt;
		delete ptr;
		ptr = nxt;
	} 
	delete ptr;
};
Parameters::Parameters(){
	parameter_list = DataLogger::readParam(&param_count);
};
