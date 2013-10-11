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
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

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

static int ms(unsigned int i ){
	/*return usleep(i*1000);*/
	return usleep(i);
}

static void printTest(){
	roombaCommand_t c;
	beforTest();
	printf("teste: %s\n", __FUNCTION__);
	c.command = ROOMBA_START;
	c.size = 0;
	printf("Erwartet:\nCommand: %d Size: 0 Data: \n", ROOMBA_START);
	roomba_printCommand(&c);
}

static void printTest2(){
	roombaCommand_t c;
	beforTest();
	printf("teste: %s\n", __FUNCTION__);
	c.command = ROOMBA_DRIVE;
	c.size = 4;
	c.data[0] = 0x01;
	c.data[1] = 0x02;
	c.data[2] = 0x03;
	c.data[3] = 0x04;

	printf("Erwartet:\nCommand: %d Size: 4 Data: 1 = 0x1, 2 = 0x2, 3 = 0x3, 4 = 0x4,\n", ROOMBA_DRIVE);
	roomba_printCommand(&c);
}

static void printTest3(){
	roombaSensorData_t s;
	beforTest();
	printf("teste: %s\n", __FUNCTION__);
	s.sensorID = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	s.size = 1;
	s.data = 0x1;
	printf("Erwartet:\nSensorID: ROOMBA_SENSOR_BUMPS_WHEEL_DROPS(%d) Data: 0x1 = 1\n", ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	roomba_printSensorData(&s);
}

static void printTest4(){
	roombaSensorData_t s;
	beforTest();
	printf("teste: %s\n", __FUNCTION__);
	s.sensorID = ROOMBA_SENSOR_LIGHT_BUMPER_FRONT_LEFT;
	s.size = 1;
	s.data = 0x42;
	printf("Erwartet:\nSensorID: ROOMBA_SENSOR_LIGHT_BUMPER_FRONT_LEFT(%d) Data: 0x42 = 66\n", ROOMBA_SENSOR_LIGHT_BUMPER_FRONT_LEFT);
	roomba_printSensorData(&s);
}

static void sendCommandTest(){
	beforTest();
	printf("teste: %s\n", __FUNCTION__);
	assert(roomba_send(ROOMBA_START, 0) >= 0);
	assert(counter == 1);
	assert(buffer[0] == ROOMBA_START);
}
static void sendCommandTest2(){
	beforTest();
	printf("teste: %s\n", __FUNCTION__);
	assert(roomba_send(ROOMBA_DRIVE, 4, 0x1, 0x2, 0x3, 0x4) >= 0);
	assert(counter == 5);
	assert(buffer[0] == ROOMBA_DRIVE);
	assert(buffer[1] == 0x1);
	assert(buffer[2] == 0x2);
	assert(buffer[3] == 0x3);
	assert(buffer[4] == 0x4);
}

static void sendStartTest(){
	beforTest();
	printf("teste: %s\n", __FUNCTION__);
	assert(roomba_start() >= 0);
	assert(counter == 2);
	assert(buffer[0] == ROOMBA_START);
	assert(buffer[1] == ROOMBA_SAFE);
}

void showASCIITest(){
	beforTest();
	assert(roomba_showASCII("Dies ist ein Test") >= 0);
	assert(counter == 25);
	assert(buffer[0] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[1] == 'D');
	assert(buffer[2] == 'i');
	assert(buffer[3] == 'e');
	assert(buffer[4] == 's');

	assert(buffer[5] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[6] == ' ');
	assert(buffer[7] == 'i');
	assert(buffer[8] == 's');
	assert(buffer[9] == 't');

	assert(buffer[10] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[11] == ' ');
	assert(buffer[12] == 'e');
	assert(buffer[13] == 'i');
	assert(buffer[14] == 'n');

	assert(buffer[15] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[16] == ' ');
	assert(buffer[17] == 'T');
	assert(buffer[18] == 'e');
	assert(buffer[19] == 's');

	assert(buffer[20] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[21] == 't');
	assert(buffer[22] == ' ');
	assert(buffer[23] == ' ');
	assert(buffer[24] == ' ');
}

void showHexLEDTest(){
	beforTest();
	assert(roomba_showHexLED(0xABCD) >= 0);

	assert(counter == 5);
	assert(buffer[0] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[1] == 'A');
	assert(buffer[2] == 'B');
	assert(buffer[3] == 'C');
	assert(buffer[4] == 'D');
}

void showHexLEDTest2(){
	beforTest();
	assert(roomba_showHexLED(0x1234) >= 0);

	assert(counter == 5);
	assert(buffer[0] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[1] == '1');
	assert(buffer[2] == '2');
	assert(buffer[3] == '3');
	assert(buffer[4] == '4');
}

void showHexLEDTest3(){
	beforTest();
	assert(roomba_showHexLED(0x1000) >= 0);

	assert(counter == 5);
	assert(buffer[0] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[1] == '1');
	assert(buffer[2] == '0');
	assert(buffer[3] == '0');
	assert(buffer[4] == '0');
}

void showHexLEDTest4(){
	beforTest();
	assert(roomba_showHexLED(0x1) >= 0);

	assert(counter == 5);
	assert(buffer[0] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[1] == ' ');
	assert(buffer[2] == ' ');
	assert(buffer[3] == ' ');
	assert(buffer[4] == '1');
}

void showHexLEDTest5(){
	beforTest();
	assert(roomba_showHexLED(0x10) >= 0);

	assert(counter == 5);
	assert(buffer[0] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[1] == ' ');
	assert(buffer[2] == ' ');
	assert(buffer[3] == '1');
	assert(buffer[4] == '0');
}

void showHexLEDTest6(){
	beforTest();
	assert(roomba_showHexLED(0x100) >= 0);

	assert(counter == 5);
	assert(buffer[0] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[1] == ' ');
	assert(buffer[2] == '1');
	assert(buffer[3] == '0');
	assert(buffer[4] == '0');
}

void showHexLEDTest7(){
	beforTest();
	assert(roomba_showHexLED(0x1000) >= 0);

	assert(counter == 5);
	assert(buffer[0] == ROOMBA_LEDS_DIGIT_ASCII);
	assert(buffer[1] == '1');
	assert(buffer[2] == '0');
	assert(buffer[3] == '0');
	assert(buffer[4] == '0');
}

void driveTest(){
	int16_t v = 42;
	int16_t a = 512;
	beforTest();
	assert(roomba_drive(v, a) >= 0);

	assert(counter == 5);
	assert(buffer[0] == ROOMBA_DRIVE);
	assert(buffer[1] == (uint8_t) (v >> 8));
	assert(buffer[2] == (uint8_t) (v & 0xFF));
	assert(buffer[3] == (uint8_t) (a >> 8));
	assert(buffer[4] == (uint8_t) (a & 0xFF));
}

void driveTest2(){
	int16_t v = -42;
	int16_t a = -512;
	beforTest();
	assert(roomba_drive(v, a) >= 0);
	
	assert(counter == 5);
	assert(buffer[0] == ROOMBA_DRIVE);
	assert(buffer[1] == (uint8_t) (v >> 8));
	assert(buffer[2] == (uint8_t) (v & 0xFF));
	assert(buffer[3] == (uint8_t) (a >> 8));
	assert(buffer[4] == (uint8_t) (a & 0xFF));
}

void driveTest3(){
	int16_t v = 500;
	int16_t a = 2000;
	beforTest();
	assert(roomba_drive(v, a) >= 0);
	
	assert(counter == 5);
	assert(buffer[0] == ROOMBA_DRIVE);
	assert(buffer[1] == (uint8_t) (v >> 8));
	assert(buffer[2] == (uint8_t) (v & 0xFF));
	assert(buffer[3] == (uint8_t) (a >> 8));
	assert(buffer[4] == (uint8_t) (a & 0xFF));
}

void driveTest4(){
	int16_t v = -500;
	int16_t a = -2000;
	beforTest();
	assert(roomba_drive(v, a) >= 0);
	
	assert(counter == 5);
	assert(buffer[0] == ROOMBA_DRIVE);
	assert(buffer[1] == (uint8_t) (v >> 8));
	assert(buffer[2] == (uint8_t) (v & 0xFF));
	assert(buffer[3] == (uint8_t) (a >> 8));
	assert(buffer[4] == (uint8_t) (a & 0xFF));
}

void runCommandTests(){
	roomba_command_init(&sendTest, &ms);
	printTest();
	printTest2();
	printTest3();
	printTest4();
	sendCommandTest();
	sendCommandTest2();
	sendStartTest();
	showASCIITest();
	showHexLEDTest();
	showHexLEDTest2();
	showHexLEDTest3();
	showHexLEDTest4();
	showHexLEDTest5();
	showHexLEDTest6();
	showHexLEDTest7();
	driveTest();
	driveTest2();
	driveTest3();
	driveTest4();
}
