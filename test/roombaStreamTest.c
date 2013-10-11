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
#include "roombaStreamTest.h"
#include <roomba_stream.h>
#include <roomba_print.h>
#include "unit.h"
#include <assert.h>

void roomba_stream_initTest(){
	roombaSensorData_t s;
	uint8_t i;
	beforTest();
	assert(roomba_stream_init() >= 0);
	/*
	 * Alle eintrage mussen leer sein
	 */
	for(i = ROOMBA_SENSOR_GROUP_100_START; i < ROOMBA_SENSOR_GROUP_100_STOP; i++){
		assert(roomba_stream_get(i, &s) < 0);
	}
}
void roomba_stream_receiveTest(){
	roombaSensorData_t s;
	roombaSensorData_t s2;
	uint8_t i;
	beforTest();
	assert(roomba_stream_init() >= 0);
	for(i = ROOMBA_SENSOR_GROUP_100_START;i < ROOMBA_SENSOR_GROUP_100_STOP; i++){
		s.sensorID = i;
		s.size = 1;
		s.data = i;
		assert(roomba_stream_receive(&s) > 0);
		assert(roomba_stream_get(i, &s2) >= 0);
		assert(s2.sensorID == i);
		assert(s2.size == 1);
		assert(s2.data == i);
	}
	for(i = ROOMBA_SENSOR_GROUP_100_START; i < ROOMBA_SENSOR_GROUP_100_STOP; i++){
		assert(roomba_stream_get(i, &s2) >= 0);
		assert(s2.sensorID == i);
		assert(s2.size == 1);
		if(i == ROOMBA_SENSOR_DISTANCE || i == ROOMBA_SENSOR_ANGLE){
			assert(s2.data == 0);
		}else{
			assert(s2.data == i);
		}
	}
	for(i = ROOMBA_SENSOR_GROUP_100_START;i < ROOMBA_SENSOR_GROUP_100_STOP; i++){
		s.sensorID = i;
		s.size = 1;
		s.data = i;
		if(i == ROOMBA_SENSOR_DISTANCE || i == ROOMBA_SENSOR_ANGLE){
			assert(roomba_stream_receive(&s) >= 0);
		}else{
			assert(roomba_stream_receive(&s) == 0);
		}
		assert(roomba_stream_get(i, &s2) >= 0);
		assert(s2.sensorID == i);
		assert(s2.size == 1);
		assert(s2.data == i);
	}
	for(i = ROOMBA_SENSOR_GROUP_100_START;i < ROOMBA_SENSOR_GROUP_100_STOP; i++){
		s.sensorID = i;
		s.size = 1;
		s.data = i*2;
		assert(roomba_stream_receive(&s) >= 0);
		assert(roomba_stream_get(i, &s2) >= 0);
		assert(s2.sensorID == i);
		assert(s2.size == 1);
		assert(s2.data == i*2);
	}
	for(i = ROOMBA_SENSOR_GROUP_100_START; i < ROOMBA_SENSOR_GROUP_100_STOP; i++){
		assert(roomba_stream_get(i, &s2) >= 0);
		assert(s2.sensorID == i);
		assert(s2.size == 1);
		if(i == ROOMBA_SENSOR_DISTANCE || i == ROOMBA_SENSOR_ANGLE){
			assert(roomba_stream_receive(&s) >= 0);
		}else{
			assert(s2.data == i*2);
		}
	}
}

void runStreamTests(){
	roomba_stream_initTest();
	roomba_stream_receiveTest();

}
