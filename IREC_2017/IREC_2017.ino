/*
Name:    IREC_2017.ino
Created: 1/29/2017 11:30:52 PM
Author:  Sean Journot
*/

//#define _WSU_MAVLINK_TEST
#include <Arduino.h>
#include <SD.h>
#include <Wire.h>
#include "WSU_mavlink.h"
#include "Parameters.h"
#include "DataLogger.h"
#include "EM_506.h"
#include "BMP180.h"
#include "LIS331HH.h"
#include "L3GD20TR.h"

void setup() {
// serial startup
	Serial.begin(9600);
	Serial1.begin(9600);	
	Wire.begin();
	#ifdef _WSU_MAVLINK_TEST
		Serial.begin(9600);
	#endif
		pinMode(7,OUTPUT);
		pinMode(10, OUTPUT);
		digitalWrite(7, HIGH);
		delay(1000);
		digitalWrite(7, LOW);
		delay(1000);
		digitalWrite(10, HIGH);
		delay(1000);
		digitalWrite(10, LOW);
}

void loop() {
	DataLogger logger;
	Parameters vars;
	EM506 gps;
	BMP180 baro(&vars);
	MEMS_Gyro gyro;
	MEMS_Accel accel;
	char logString[100];
	unsigned long logTimeStep = 50;
	unsigned long logTime = millis();
	while (1){
		// executing tasks
		accel.runAccel();
		gyro.runGyro();
		gps.runGPS();
		baro.runBarometer();
		send_telemetry(&vars, &gps, &baro, &gyro, &accel);
		communication_receive(&vars);
		communication_send(vars.system_channel);
		// logging data
		if (millis()-logTime > logTimeStep){
			sprintf(logString, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d;",
				baro.altitude, baro.altitude - vars.homeAlt,
				baro.pressure, baro.temperature,
				accel.x, accel.y, accel.z,
				gyro.x, gyro.y, gyro.z,
				gyro.roll, gyro.pitch, gyro.yaw);
			logger.writeLog(logString);
			logTime = millis();
		}
	}
}
