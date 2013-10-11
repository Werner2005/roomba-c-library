/*
Copyright (c) 2013 Andreas Werner

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <roomba_stream.h>
#include <stdlib.h>
#ifdef ROOMBA_DEBUG
#include <stdio.h>
#include <roomba_print.h>
#endif

static volatile roombaSensorData_t sensorData[ROOMBA_SENSOR_GROUP_100_STOP];

static __inline__ int8_t checkSensorID(uint8_t sid){
	if(sid == ROOMBA_SENSOR_DISTANCE || sid == ROOMBA_SENSOR_ANGLE){
		return 1;
	}else{
		return 0;
	}
}

static int8_t checkData(roombaSensorData_t* data){
	uint8_t sID = data->sensorID;
	if(sensorData[sID].sensorID == 0){
		return 0;
	}
	if(checkSensorID(sID) == 1){
		if(sensorData[sID].data == 0 && data->data == 0){
			return 1;
		}else{
			return 0;
		}
	}else{
		if(sensorData[sID].data == data->data){
			return 1;
		}else{
			return 0;
		}
	}

}

int8_t roomba_sensor_database_init(){
	uint8_t i;
	/*
	 * Loesche Daten
	 */
	for(i = 0; i < ROOMBA_SENSOR_GROUP_100_STOP; i++){
		sensorData[i].sensorID = 0; 
		sensorData[i].size = 0; 
		sensorData[i].data = 0;
	}
	return 0;
}
int8_t roomba_sensor_database_receive(roombaSensorData_t* data){
	uint8_t sID;
	if(data == NULL|| data->sensorID == 0){
		#ifdef ROOMBA_DEBUG
		printf("Data nicht gueltig\n");
		#endif
		return -1;
	}
	sID = data->sensorID;
	#ifdef ROOMBA_DEBUG
	printf("Verarbeite Sensor Data: ");
	roomba_printSensorData(data);
	#endif
	if(checkData(data) == 1){
		#ifdef ROOMBA_DEBUG
		printf("Data schon vorhanden\n");
		#endif
		return 0;
	}
	sensorData[sID].sensorID = sID;
	sensorData[sID].size = data->size;
	sensorData[sID].data = data->data;
	#ifdef ROOMBA_DEBUG
	printf("Data eingefuegt\n");
	#endif
	return 1;
}
int8_t roomba_sensor_database_get(uint8_t sensorID, roombaSensorData_t* result){
	if(sensorData[sensorID].sensorID == 0){
		return -1;
	}
	result->sensorID = sensorData[sensorID].sensorID;
	result->size = sensorData[sensorID].size;
	result->data = sensorData[sensorID].data;
	if(checkSensorID(sensorID) == 1){
		sensorData[sensorID].data = 0;
	}
	return 0;
}
