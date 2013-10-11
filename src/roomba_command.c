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
#include <roomba_command.h>
#include <string.h>
#include <stdarg.h>
#ifdef ROOMBA_DEBUG
#include <roomba_print.h>
#endif

/**
 * Callback fuer das senden eines Kommandos
 * 
 * Mithilfe dieses Callbacks ist es möglich auf dem Sensorknoten wie auch auf dem Contoller
 * Die Befhele zu nutzen und dabei verschiede Sense System zu nutzen
 * 
 * @param command Komanndo das gesendet werden soll
 */
static int8_t (*sendCommand)(roombaCommand_t* command);
static int (*mssleep)(unsigned int);

void roomba_command_init(int8_t (*s)(roombaCommand_t* command), int (*ms)(unsigned int)){
	sendCommand = s;
	mssleep = ms;
}

int8_t roomba_send(uint8_t commandID, uint8_t size, ...){
	va_list data;
	roombaCommand_t command;
	uint8_t i;

	memset(&command, 0, sizeof(roombaCommand_t));
	command.command = commandID;
	command.size = size;

	va_start(data, size);
	for(i = 0; i < size; i++){
		command.data[i] = (uint8_t) va_arg(data, int);
	}
	va_end(data);
	#ifdef ROOMBA_DEBUG
	roomba_printCommand(&command);
	#endif
	return sendCommand(&command);
}
int8_t roomba_start(){
	int8_t ret;
	ret = roomba_send(ROOMBA_START, 0);
	if(ret < 0) return ret;
	ret = roomba_send(ROOMBA_SAFE, 0);
	if(ret < 0) return ret;
	return 0;

}
int8_t roomba_showASCII(char * string){
	size_t size = strlen(string);
	int8_t ret;
	if(size > 4){
		ret = roomba_send(ROOMBA_LEDS_DIGIT_ASCII, 4, string[0], string[1], string[2], string[3]);
		if(ret < 0){
			return ret;
		}
		(void) mssleep(ROOMBA_LED_WAITTIME);
		/**
		 * Nachsten 4 Zeichen
		 */
		return roomba_showASCII(string+4);
	}else{
		switch(size){
			case 4:
				return roomba_send(ROOMBA_LEDS_DIGIT_ASCII, 4, string[0], string[1], string[2], string[3]);
			case 3:
				return roomba_send(ROOMBA_LEDS_DIGIT_ASCII, 4, string[0], string[1], string[2], ' ');
			case 2:
				return roomba_send(ROOMBA_LEDS_DIGIT_ASCII, 4, string[0], string[1], ' ', ' ');
			case 1:
				return roomba_send(ROOMBA_LEDS_DIGIT_ASCII, 4, string[0], ' ', ' ', ' ');
			case 0:
				return 0;
		}
	}
	return -1;
}

int8_t roomba_showHexLED(uint16_t t){
	int8_t i = 3;
	uint16_t tmp;
	char c[5];

	c[4] = '\0';
	
	while(i >= 0){
		tmp = t & 0xF;
		/*
		 * Wenn t == 0 und nicht das erste Zeichen
		 * Wird eine Lehrtaste gesetzt
		 * Das fuert dazu das eine 0x0 wie folgt
		 * dargestelt wird '   0'
		 */
		if(t == 0 && i != 3){
			c[i] = ' ';
		}else if(tmp >= 0xa){
			tmp -= 0xa;
			c[i] = (char) tmp + 'A';
		}else{
			c[i] = (char) tmp + '0';
		}
		t = t >> 4;
		i--;	
	}
	return roomba_showASCII(c);
	
}
int8_t roomba_drive(int16_t velocity, int16_t radius){
	/**
	 * Parameter Ueberprfen
	 */
	/*if(velocity < -500 || velocity > 500 || radius < -2000 || radius > 2000){
		return -1;
	}*/
	return roomba_send(ROOMBA_DRIVE, 4, (((uint16_t)velocity) >> 8), (((uint16_t) velocity) & 0xFF), (((uint16_t) radius) >> 8), (((uint16_t) radius) & 0xFF));
}
int8_t roomba_extra_stream_start(uint16_t ms, uint8_t size, ...){
	va_list data;
	roombaCommand_t command;
	uint8_t i;

	memset(&command, 0, sizeof(roombaCommand_t));
	command.command = ROOMBA_EXTRA_STREAM;
	command.size = size+3;
	command.data[0] = (uint8_t)(ms >> 8);
	command.data[1] = (uint8_t)(ms & 0xFF);
	command.data[2] = size;

	va_start(data, size);
	for(i = 0; i < size; i++){
		command.data[i+3] = (uint8_t) va_arg(data, int);
	}
	va_end(data);
	#ifdef ROOMBA_DEBUG
	roomba_printCommand(&command);
	#endif
	return sendCommand(&command);
}
int8_t roomba_extra_stream_stop(){
	return roomba_send(ROOMBA_EXTRA_STREAM_PAUSE_RESUME, 1, 0);
}
