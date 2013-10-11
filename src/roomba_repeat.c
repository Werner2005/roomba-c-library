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
#include <roomba_repeat.h>
#include <string.h>

static int8_t (*start)(uint16_t ms);
static int8_t (*stop)();
static int8_t (*sendCommand)(roombaCommand_t* command);

static volatile roombaCommand_t command;
static volatile int8_t started = 0;

int8_t roomba_repeat_init(int8_t (*c)(roombaCommand_t* command), int8_t (*thread_start)(uint16_t ms), int8_t (*thread_stop)()){
	sendCommand = c;
	start = thread_start;
	stop = thread_stop;
	return 0;
}
int8_t roomba_repeat(){
	if(started == 1){
		return sendCommand(((roombaCommand_t*) &command));
	}else{
		return ROOMBA_REPEAT_ERROR_NOT_STARTED;
	}
}
int8_t roomba_repeat_start(roombaCommand_t* c, uint16_t ms){
	int8_t tmp;
	memcpy((roombaCommand_t*) &command, c, sizeof(roombaCommand_t));
	
	if(started == 1){
		tmp = roomba_repeat_stop();
		if(tmp < 0){
			return -1;
		}
	}
	
	started = 1;

	tmp = start(ms);
	if(tmp < 0){
		return -1;
	}
	return 0;
}
int8_t roomba_repeat_stop(){
	int8_t tmp;
	started = 0;
	tmp = stop();
	if(tmp < 0){
		return -1;
	}
	return 0;
}
