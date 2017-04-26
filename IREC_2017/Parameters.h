#ifndef _PARAMETERS
#define _PARAMETERS

#include <MAVLINK\mavlink_types.h>
#include <MAVLINK\ardupilotmega\mavlink.h>


class Parameters{
public:

	typedef struct _param_element{
		char ID[17];
		uint8_t type;
		float value;
		struct _param_element* nxt;
	} param_element;

	uint16_t packet_drops = 0;

	void saveParameters();
	char* getParamID(uint16_t param);
	uint8_t getParamType(uint16_t);
	float getParamValue(uint16_t);
	uint16_t getCount();
	int16_t setParameter(char* ID,uint8_t type, float value);

	uint8_t system_type = MAV_TYPE_ROCKET;
	uint8_t autopilot_type = MAV_AUTOPILOT_ARDUPILOTMEGA;
	uint8_t system_mode = 1;
	uint32_t custom_mode = 0;
	uint8_t system_state = MAV_STATE_STANDBY;
	mavlink_channel_t system_channel = MAVLINK_COMM_1;
	int32_t homeAlt;
	uint16_t heartBeatRate = 2;
	unsigned long heartBeatTime = 0;
	uint8_t streamTags = 0x00;
	uint16_t streamRate[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned long streamTime[8] = {0,0,0,0,0,0,0,0};
	Parameters();
	~Parameters();
	
	uint16_t param_count;
	param_element* parameter_list;
};
#endif