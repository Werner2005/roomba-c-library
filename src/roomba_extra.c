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
#include <roomba_extra.h>
#include <roomba_repeat.h>
#ifdef ROOMBA_DEBUG
#include <stdio.h>
#endif


int8_t roomba_extra_init(){
	return 0;
}
uint8_t roomba_extra_check(roombaCommand_t* command){
	switch(command->command){
		case ROOMBA_EXTRA_STREAM:
			return ROOMBA_EXTRA_STREAM;
		case ROOMBA_EXTRA_STREAM_PAUSE_RESUME:
			return ROOMBA_EXTRA_STREAM_PAUSE_RESUME;
		default:
			return 0;
	}
}

static int8_t roomba_extra_stream(roombaCommand_t* command);
static int8_t roomba_extra_stream_pause_resume(roombaCommand_t* command);

int8_t roomba_extra_handle_command(roombaCommand_t* command){
	if(roomba_extra_check(command) == 0){
		return -1;
	}
	switch(command->command){
		case ROOMBA_EXTRA_STREAM:
			return roomba_extra_stream(command);
		case ROOMBA_EXTRA_STREAM_PAUSE_RESUME:
			return roomba_extra_stream_pause_resume(command);
		default:
			return -1;
	}
}
static volatile roombaCommand_t stream_command;
static volatile uint16_t stream_time = 0;
static int8_t roomba_extra_stream(roombaCommand_t* command){
	uint8_t i;
	int8_t tmp;
	#ifdef ROOMBA_DEBUG
	printf("htime: %d = 0x%x ltime: %d = 0x%x\n", command->data[0], command->data[0], command->data[1], command->data[1]);
	#endif
	stream_time = command->data[0] << 8;
	stream_time |= command->data[1];

	stream_command.command = ROOMBA_QUERY_LIST;
	for(i = 2; i < command->size; i++){
		stream_command.data[i-2] = command->data[i];
	}
	stream_command.size = i-2;
	tmp = roomba_repeat_start((roombaCommand_t*) &stream_command, stream_time);
	if(tmp < 0){
		return tmp;
	}
	return 0;
}
static int8_t roomba_extra_stream_pause_resume(roombaCommand_t* command){
	int8_t tmp;
	if(command->data[0] == 0){
		tmp = roomba_repeat_stop();
	}else{
		tmp = roomba_repeat_start((roombaCommand_t*) &stream_command, stream_time);
	}
	if(tmp < 0){
		return tmp;
	}
	return 0;
}
