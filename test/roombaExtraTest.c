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
#include "roombaExtraTest.h"
#include "unit.h"
#include <roomba.h>
#include <roomba_def.h>
#include <roomba_command.h>
#include <roomba_print.h>
#include <roomba_repeat.h>
#include <roomba_extra.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

static int8_t sendTest(roombaCommand_t* test){
	assert(roomba_sendCommand(test) >= 0);
	printf("Send: ");
	roomba_printCommand(test);
	return 0;
}

static bool threadStartet = false;
static uint16_t time;

static int8_t startThread(uint16_t ms){
	time = ms;
	assert(!threadStartet);
	threadStartet = true;
	printf("Starte Thread Zeit: %d\n", ms);
	return 0;
}

static int8_t stopThread(){
	assert(threadStartet);
	threadStartet = false;
	printf("Stope Thread\n");
	return 0;
}

static void testExtraCheck(){
	roombaCommand_t command;
	beforTest();
	command.command = ROOMBA_START;
	command.size = 0;
	assert(roomba_extra_check(&command) == 0);
}
static void testExtraCheck2(){
	roombaCommand_t command;
	beforTest();
	command.command = ROOMBA_EXTRA_STREAM;
	command.size = 0;
	assert(roomba_extra_check(&command) == ROOMBA_EXTRA_STREAM);
}

static void testExtraCheck3(){
	roombaCommand_t command;
	beforTest();
	command.command = ROOMBA_EXTRA_STREAM_PAUSE_RESUME;
	command.size = 0;
	assert(roomba_extra_check(&command) == ROOMBA_EXTRA_STREAM_PAUSE_RESUME);
}

static void testHandelRoombaExtraStream(){
	roombaCommand_t command;
	uint16_t ltime = 1000;
	uint16_t i = 0;
	beforTest();
	printf("Time: %d = %x\n", ltime, ltime);
	command.command = ROOMBA_EXTRA_STREAM;
	command.size = 4;
	command.data[0] = ltime >> 8;
	command.data[1] = ltime & 0xFF;
	command.data[2] = 1;
	command.data[3] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	roomba_printCommand(&command);
	assert(roomba_extra_check(&command) == ROOMBA_EXTRA_STREAM);
	assert(roomba_extra_handle_command(&command) >= 0);

	assert(threadStartet == 1);
	assert(time == 1000);
	assert(roomba_repeat() >= 0);
	assert(roomba_repeat() >= 0);
	assert(buffer[i++] == ROOMBA_QUERY_LIST);
	assert(buffer[i++] == 1);
	assert(buffer[i++] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(buffer[i++] == ROOMBA_QUERY_LIST);
	assert(buffer[i++] == 1);
	assert(buffer[i++] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	
	command.command = ROOMBA_EXTRA_STREAM_PAUSE_RESUME;
	command.size = 1;
	command.data[0] = 0;
	assert(roomba_extra_handle_command(&command) >= 0);
	assert(threadStartet == 0);

	command.command = ROOMBA_EXTRA_STREAM_PAUSE_RESUME;
	command.size = 1;
	command.data[0] = 1;
	assert(roomba_extra_handle_command(&command) >= 0);

	assert(threadStartet == 1);
	assert(time == 1000);
	assert(roomba_repeat() >= 0);
	assert(roomba_repeat() >= 0);
	assert(buffer[i++] == ROOMBA_QUERY_LIST);
	assert(buffer[i++] == 1);
	assert(buffer[i++] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(buffer[i++] == ROOMBA_QUERY_LIST);
	assert(buffer[i++] == 1);
	assert(buffer[i++] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);

	command.command = ROOMBA_EXTRA_STREAM_PAUSE_RESUME;
	command.size = 1;
	command.data[0] = 0;
	assert(roomba_extra_handle_command(&command) >= 0);
	assert(threadStartet == 0);
}

void testExtraStreamStartFunktion(){
	uint16_t ltime = 1000;
	beforTest();
	roomba_extra_stream_start(ltime, 1, ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(buffer[0] == ROOMBA_EXTRA_STREAM);
	assert(buffer[1] == (ltime >> 8));
	assert(buffer[2] == (ltime & 0xFF));
	assert(buffer[3] == 1);
	assert(buffer[4] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
}

void runExtraTests(){
	assert(roomba_repeat_init(&sendTest, &startThread, &stopThread) >= 0);
	assert(roomba_extra_init() >= 0);
	testExtraCheck();
	testExtraCheck2();
	testExtraCheck3();
	testHandelRoombaExtraStream();
	testExtraStreamStartFunktion();
	
}
