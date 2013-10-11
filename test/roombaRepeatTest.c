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
#include "roombaCommandTest.h"
#include "unit.h"
#include <roomba.h>
#include <roomba_def.h>
#include <roomba_command.h>
#include <roomba_print.h>
#include <roomba_repeat.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

static int8_t sendTest(roombaCommand_t* test){
	int i = 0; 
	assert(roomba_sendCommand(test) >= 0);
	printf("Send: ID: %d size: %d Data: ", test->command, test->size);
	for(i = 0; i < test->size; i++){
		printf("%d,", test->data[i]);
	}
	printf("\n");

	return 0;
}

static bool threadStartet = false;
static uint16_t time = 0;

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

static void testStartStop(){
	roombaCommand_t command; 
	beforTest();
	command.command = ROOMBA_START;
	command.size = 0;
	assert(roomba_repeat() < 0);
	assert(roomba_repeat_start(&command, 10) >= 0);
	assert(threadStartet);
	assert(time == 10);
	assert(roomba_repeat() >= 0);
	assert(roomba_repeat() >= 0);
	assert(buffer[0] == ROOMBA_START);
	assert(buffer[1] == ROOMBA_START);
	assert(roomba_repeat_stop() >= 0);
	assert(!threadStartet);
	assert(roomba_repeat() < 0);
	assert(roomba_repeat_start(&command, 11) >= 0);
	assert(threadStartet);
	assert(time == 11);
	command.command = ROOMBA_SAFE;
	assert(roomba_repeat_start(&command, 12) >= 0);
	assert(threadStartet);
	assert(time == 12);
	assert(roomba_repeat() >= 0);
	assert(roomba_repeat() >= 0);
	assert(buffer[2] == ROOMBA_SAFE);
	assert(buffer[3] == ROOMBA_SAFE);
	assert(roomba_repeat_stop() >= 0);
	assert(!threadStartet);
}

void runRepeatTests(){
	assert(roomba_repeat_init(&sendTest, &startThread, &stopThread) >= 0);
	testStartStop();

}
