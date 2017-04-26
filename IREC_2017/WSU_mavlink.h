#ifndef _WSU_MAVLINK
#define _WSU_MAVLINK
#define MAVLINK_USE_CONVENIENCE_FUNCTIONS
#define MAVLINK_SEND_UART_BYTES

#include <MAVLINK\mavlink_types.h>
#include <Arduino.h>

#define SERIAL_BUFFER_SIZE 0x1000

mavlink_system_t mavlink_system = {20,100};
uint8_t CTSpin = 44;
uint8_t serial_buffer[SERIAL_BUFFER_SIZE];
uint16_t serial_buffer_length = 0;

static inline void communication_send(mavlink_channel_t chan){
	pinMode(CTSpin, INPUT);
	if (serial_buffer_length != 0){
		if (chan == MAVLINK_COMM_0){
			for (int i = 0; i < serial_buffer_length; i++){
				if(!digitalRead(CTSpin)){
					Serial.write(serial_buffer[i]);
				}
				else{
					int j = 0;
					for ( ; i < serial_buffer_length; j++, i++){
						serial_buffer[j] = serial_buffer[i];
					}
					serial_buffer_length = j;
					break;
				}
				if (i == serial_buffer_length - 1){
					serial_buffer_length = 0;
				}
			}
		}
		else 
		if (chan == MAVLINK_COMM_1){
			for (int i = 0; i < serial_buffer_length; i++){
				if (!digitalRead(CTSpin)){
					Serial1.write(serial_buffer[i]);
				}
				else{
					int j = 0;
					for (; i < serial_buffer_length; j++, i++){
						serial_buffer[j] = serial_buffer[i];
					}
					serial_buffer_length = j;
					break;
				}
				if (i == serial_buffer_length - 1){
					serial_buffer_length = 0;
				}
			}
		}
	}
}
static inline void comm_send_ch(mavlink_channel_t chan, uint8_t ch)
{
	//Serial.write(ch);
	communication_send(chan);
	if (serial_buffer_length < SERIAL_BUFFER_SIZE){
		serial_buffer[serial_buffer_length + 1] = ch;
		serial_buffer_length++;
		communication_send(chan);
	}
}
static inline void mavlink_send_uart_bytes(mavlink_channel_t chan, const char * buf, uint16_t len){
	//Serial.write(buf, len);
	
	communication_send(chan);
	if (serial_buffer_length + len <= SERIAL_BUFFER_SIZE){
		for (int i = 0; i < len; i++){
			serial_buffer[i + serial_buffer_length] = buf[i];
		}
		serial_buffer_length += len;
		communication_send(chan);
	}
}

#include <MAVLINK\ardupilotmega\mavlink.h>
#include "Parameters.h"
#include "EM_506.h"
#include "BMP180.h"
#include "LIS331HH.h"
#include "L3GD20TR.h"

static void communication_receive(Parameters* vars)
{
	mavlink_message_t msg;
	mavlink_status_t status;

	while (Serial1.available())
	{
		uint8_t c;
		Serial1.readBytes(&c, 1);
		// Try to get a new message
		if (mavlink_parse_char(vars->system_channel, c, &msg, &status)) {
			// Handle message
			#ifdef _WSU_MAVLINK_TEST
				Serial.print("message_ID\n");
				Serial.print(msg.msgid);
				Serial.write("\n");
			#endif
			switch (msg.msgid)
			{
			case MAVLINK_MSG_ID_HEARTBEAT:
				{
					//Serial.print("heartbeat recieved/n");	 // E.g. read GCS heartbeat and go into
															 // comm lost mode if timer times out
				}
				break;
			case MAVLINK_MSG_ID_COMMAND_LONG:
				{
					mavlink_command_long_t command;
					mavlink_msg_command_long_decode(&msg, &command);
					if (command.target_system == mavlink_system.sysid){
						switch (command.command){
						case MAV_CMD_COMPONENT_ARM_DISARM:
							if (command.param1 == 1){
								vars->system_mode = vars->system_mode |= 1 << 7;
								mavlink_msg_command_ack_send(vars->system_channel, command.command, MAV_RESULT_ACCEPTED);
							}
							else if(command.param1 == 0)
							{ 
								vars->system_mode = vars->system_mode &= ~(1 << 7);
								mavlink_msg_command_ack_send(vars->system_channel, command.command, MAV_RESULT_ACCEPTED);
							}
							else{
								mavlink_msg_command_ack_send(vars->system_channel, command.command, MAV_RESULT_FAILED);
							};
							break;
						case MAV_CMD_DO_PARACHUTE:
							// ConfigHWParachute.cs
							if (command.param1 == 2 && vars->system_mode == 129){
								// deploy_main_chute();
								mavlink_msg_command_ack_send(vars->system_channel, command.command, MAV_RESULT_ACCEPTED);
							}
							else{
								mavlink_msg_command_ack_send(vars->system_channel, command.command, MAV_RESULT_DENIED);
							};
							break;
						case MAV_CMD_DO_SET_HOME:
							if (command.param1 == 2){
								// home.lat = command.param5;
								// home.long = command.param6;
								// home.alt = command.param7;
								mavlink_msg_command_ack_send(vars->system_channel, command.command, MAV_RESULT_ACCEPTED);
							}
							else if (command.param1 == 1){
								// setHomeHere();
								mavlink_msg_command_ack_send(vars->system_channel, command.command, MAV_RESULT_ACCEPTED);
							}
							else{
								mavlink_msg_command_ack_send(vars->system_channel, command.command, MAV_RESULT_FAILED);
							}
							break;
						case MAV_CMD_GET_HOME_POSITION:
							break;
						case MAV_CMD_SET_MESSAGE_INTERVAL:

							break;
						case MAV_CMD_GET_MESSAGE_INTERVAL:
							break;
						default:
							mavlink_msg_command_ack_send(vars->system_channel, command.command, MAV_RESULT_UNSUPPORTED);
							break;
						}
					}
				}
				break;
			case MAVLINK_MSG_ID_AUTOPILOT_VERSION_REQUEST:
				__mavlink_autopilot_version_request_t version_request;
				mavlink_msg_autopilot_version_request_decode(&msg, &version_request);
				if (version_request.target_system = mavlink_system.sysid){
					mavlink_msg_autopilot_version_send(vars->system_channel, MAV_PROTOCOL_CAPABILITY_ENUM_END, 9, 9, 9, 9, 0, 0, 0, 0, 0, 0);
				}
				break;
			case MAVLINK_MSG_ID_SET_MODE:
				__mavlink_set_mode_t setMode;
				mavlink_msg_set_mode_decode(&msg, &setMode);
				if (setMode.target_system == mavlink_system.sysid){
					vars->custom_mode = setMode.custom_mode;
					//system_mode = setMode.base_mode;
					#ifdef _WSU_MAVLINK_TEST
					Serial.print("custom_mode\n");
					Serial.print(vars->custom_mode);
					Serial.print("\n");
					#endif
				}
				break;
			case MAVLINK_MSG_ID_MISSION_REQUEST:
				__mavlink_mission_request_t mission_request;
				mavlink_msg_mission_request_decode(&msg, &mission_request);
				if (mission_request.target_system == mavlink_system.sysid){
					mavlink_msg_mission_item_send(vars->system_channel, mission_request.target_system, mission_request.target_component, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				}
				break;
			case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:
				__mavlink_request_data_stream_t data_stream;
				mavlink_msg_request_data_stream_decode(&msg, &data_stream);
				if (data_stream.target_system == mavlink_system.sysid){
					#ifdef _WSU_MAVLINK_TEST
						Serial.println(data_stream.req_stream_id);
						Serial.println(data_stream.req_message_rate);
						Serial.println(data_stream.start_stop);
					#endif
						switch (data_stream.req_stream_id){
						case MAV_DATA_STREAM_ALL:
							if (data_stream.start_stop == 1){
								vars->streamTags = 0xFF;
							} else{
								vars->streamTags = 0x00;
							}
							for (int i = 0; i < 8; i++){
								vars->streamRate[i] = data_stream.req_message_rate;
							}
							break;
						case MAV_DATA_STREAM_RAW_SENSORS:
							if (data_stream.start_stop == 1){
								vars->streamTags |= 1 << 0;
							}
							else{
								vars->streamTags &= ~(1 << 0);
							}
							vars->streamRate[0] = data_stream.req_message_rate;
							break;
						case MAV_DATA_STREAM_EXTENDED_STATUS:
							if (data_stream.start_stop == 1){
								vars->streamTags |= 1 << 1;
							}
							else{
								vars->streamTags &= ~(1 << 1);
							}
							vars->streamRate[1] = data_stream.req_message_rate;
							break;
						case MAV_DATA_STREAM_RC_CHANNELS:
							if (data_stream.start_stop == 1){
								vars->streamTags |= 1 << 2;
							}
							else{
								vars->streamTags &= ~(1 << 2);
							}
							vars->streamRate[2] = data_stream.req_message_rate;
							break;
						case MAV_DATA_STREAM_RAW_CONTROLLER:
							if (data_stream.start_stop == 1){
								vars->streamTags |= 1 << 3;
							}
							else{
								vars->streamTags &= ~(1 << 3);
							}
							vars->streamRate[3] = data_stream.req_message_rate;
							break;
						case MAV_DATA_STREAM_POSITION:
							if (data_stream.start_stop == 1){
								vars->streamTags |= 1 << 4;
							}
							else{
								vars->streamTags &= ~(1 << 4);
							}
							vars->streamRate[4] = data_stream.req_message_rate;
								break;
						case MAV_DATA_STREAM_EXTRA1:
							if (data_stream.start_stop == 1){
								vars->streamTags |= 1 << 5;
							}
							else{
								vars->streamTags &= ~(1 << 5);
							}
							vars->streamRate[5] = data_stream.req_message_rate;
								break;
						case MAV_DATA_STREAM_EXTRA2:
							if (data_stream.start_stop == 1){
								vars->streamTags |= 1 << 6;
							}
							else{
								vars->streamTags &= ~(1 << 6);
							}
							vars->streamRate[6] = data_stream.req_message_rate;
							break;
						case MAV_DATA_STREAM_EXTRA3:
							if (data_stream.start_stop == 1){
								vars->streamTags |= 1 << 7;
							}
							else{
								vars->streamTags &= ~(1 << 7);
							}
							vars->streamRate[7] = data_stream.req_message_rate;
							break;
						}
				}
				break;
			case MAVLINK_MSG_ID_STATUSTEXT:
				__mavlink_statustext_t status_text;
				mavlink_msg_statustext_decode(&msg, &status_text);
					#ifdef _WSU_MAVLINK_TEST
						Serial.print("status_text severity\n");
						Serial.print(status_text.severity);
						Serial.print("\nstatus_text message\n");
						status_text.text[50] = NULL;
						Serial.print(status_text.text);
						Serial.print("\n");
					#endif
					mavlink_msg_statustext_send(vars->system_channel, 0, status_text.text);
				break;
			case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
				
				__mavlink_param_request_list_t list_request;
				for (int i = 0; i < vars->getCount(); i++){
					mavlink_msg_param_value_send(vars->system_channel, vars->getParamID(i),
						vars->getParamValue(i), vars->getParamType(i), vars->getCount(), i);
					delay(60);
				}
				
				break;
			case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
				__mavlink_param_request_read_t request_param;
				mavlink_msg_param_request_read_decode(&msg, &request_param);
				if (request_param.target_system == mavlink_system.sysid){
					#ifdef _WSU_MAVLINK_TEST
						Serial.print("parameter request\n");
						Serial.print(request_param.param_index);
						Serial.print("\n");
					#endif
					mavlink_msg_param_value_send(vars->system_channel, vars->getParamID(request_param.param_index),
						vars->getParamValue(request_param.param_index), vars->getParamType(request_param.param_index),
						vars->getCount(), request_param.param_index);
				}
				break;
			case MAVLINK_MSG_ID_PARAM_SET:
				__mavlink_param_set_t paramSet;
				mavlink_msg_param_set_decode(&msg, &paramSet);
				#ifdef _WSU_MAVLINK_TEST
					Serial.print("\nPARAM_SET.id:\n");
					Serial.print(paramSet.param_id);
					Serial.print("\nPARAM_SET.type:\n");
					Serial.print(paramSet.param_type);
					Serial.print("\nPARAM_SET.value:\n");
					Serial.print(paramSet.param_value);
					Serial.print("\n \n");
				#endif
				if (paramSet.target_system == mavlink_system.sysid){
					int16_t varID = vars->setParameter(paramSet.param_id,paramSet.param_type,paramSet.param_value);
					if (varID > -1){
						mavlink_msg_param_value_send(vars->system_channel, vars->getParamID(varID),
							vars->getParamValue(varID), vars->getParamType(varID),
							vars->getCount(), varID);
					}
				}
				break;
			default:
				break;
			}
		}
		else{
		}
		vars->packet_drops += status.packet_rx_drop_count;
	}
	// Update global packet drops counter
	vars->packet_drops += status.packet_rx_drop_count;
}
static void send_telemetry(Parameters* vars, EM506* gps, BMP180* baro, MEMS_Gyro* gyro, MEMS_Accel* accel){
	if (1 < (micros() - vars->heartBeatTime)*(float)vars->heartBeatRate / 1000000){
		mavlink_msg_heartbeat_send(vars->system_channel, vars->system_type, vars->autopilot_type,
			vars->system_mode, vars->custom_mode, vars->system_state);
		vars->heartBeatTime = micros();
	}
	if ((1 < (micros() - vars->streamTime[0])*(float)vars->streamRate[0] / 1000000) && ((vars->streamTags & 1 << 0) >> 0 == 1)){
		//do MAV_DATA_STREAM_RAW_SENSORS
		
		EM506::Loc loc = gps->getLoc();
		EM506::Time time = gps->getTime();
		EM506::ActiveSat sat = gps->getActiveSat();
		EM506::CourseSpeed csp = gps->getCourseSpeed();
		mavlink_msg_gps_raw_int_send(vars->system_channel,time.UTC,sat.mode2,loc.latitude,
			loc.longitude, loc.altitude, sat.HDOP * 100, sat.VDOP * 100, csp.speed1 * 100, csp.course1 * 100, sat.satCount);
		//mavlink_msg_radio_send();
		mavlink_msg_scaled_imu_send(vars->system_channel,time.UTC,
			accel->accel_X.x,accel->accel_Y.y,accel->accel_Z.z,
			gyro->x,gyro->y,gyro->z,
			0,0,0);
		mavlink_msg_attitude_send(vars->system_channel, time.UTC, gyro->roll, gyro->pitch, gyro->yaw, gyro->x*1000.0, gyro->y*1000.0, gyro->z*1000.0);
		mavlink_msg_scaled_pressure_send(vars->system_channel, time.UTC,
			baro->pressure, 0, baro->temperature/10);
		vars->streamTime[0] = micros();
	}
	if ((1 < (micros() - vars->streamTime[1])*(float)vars->streamRate[1] / 1000000) && ((vars->streamTags & 1 << 1) >> 1 == 1)){
		//do MAV_DATA_STREAM_EXTENDED_STATUS
		vars->streamTime[1] = micros();
	}
	if ((1 < (micros() - vars->streamTime[2])*(float)vars->streamRate[2] / 1000000) && ((vars->streamTags & 1 << 2) >> 2 == 1)){
		//do MAV_DATA_STREAM_RC_CHANNELS
		vars->streamTime[2] = micros();
	}
	if ((1 < (micros() - vars->streamTime[3])*(float)vars->streamRate[3] / 1000000) && ((vars->streamTags & 1 << 3) >> 3 == 1)){
		//do MAV_DATA_STREAM_RAW_CONTROLLER
		vars->streamTime[3] = micros();
	}
	if ((1 < (micros() - vars->streamTime[4])*(float)vars->streamRate[4] / 1000000) && ((vars->streamTags & 1 << 4) >> 4 == 1)){
		//do MAV_DATA_STREAM_POSITION
		EM506::Loc loc = gps->getLoc();
		mavlink_msg_global_position_int_send(vars->system_channel, millis(), loc.latitude, loc.longitude, (int32_t)baro->altitude*1000, ((int32_t)baro->altitude - vars->homeAlt)*1000, 0, 0, 0, 45);
		vars->streamTime[4] = micros();
	}
	if ((1 < (micros() - vars->streamTime[5])*(float)vars->streamRate[5] / 1000000) && ((vars->streamTags & 1 << 5) >> 5 == 1)){
		//do MAV_DATA_STREAM_EXTRA1
		vars->streamTime[5] = micros();
	}
	if ((1 < (micros() - vars->streamTime[6])*(float)vars->streamRate[6] / 1000000) && ((vars->streamTags & 1 << 6) >> 6 == 1)){
		//do MAV_DATA_STREAM_EXTRA2
		vars->streamTime[6] = micros();
	}
	if ((1 < (micros() - vars->streamTime[7])*(float)vars->streamRate[7] / 1000000) && ((vars->streamTags & 1 << 7) >> 7 == 1)){
		//do MAV_DATA_STREAM_EXTRA3
		vars->streamTime[7] = micros();
	}
}
#endif