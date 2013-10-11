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
#include "roombaTest.h"
#include "unit.h"
#include <roomba.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define TESTMANY 1

/** 
 * 
 */
void test_init(int8_t (*sim)(uint8_t c), int (*w)(unsigned char c)){
	roomba_read_sim = sim;
	write_sim = w;
	roomba_init(test_write);
}

/**
 * Gibt ein SensorDaten Packet aus
 * Syntax: SensorID: %d Data: %d\n
 * @param SensorData
 */
void echoSensorData(roombaSensorData_t* data){
	printf("SensorID: %d Data: %x\n", data->sensorID, (uint16_t) data->data);
}
/**
 * Sensor Test Funktion
 * Sendet ein {@link ROOMBA_SENSORS} Befehl und prüfet ob er gültig gesnedet und entfangen wurde
 * Zusäzlich dient diese Funktion als read Stub. Das Schreiben des Roomba ueber RS232 getestet in dem 
 * Diese Funktion read manuell mit den Gueltigen Daten aufruft.
 *
 * @param function String mit Aufruffer Funktion
 * @param expectedSensorID Erwarete SensorID 
 * @param expectedData Erwarete Daten vom Rommba werden verwendet um daten ueber read zu senden
 * @param bigdata Bestimmt ob es sich um eine 16Bit Daten Sequenz handelt
 */
void sensorTest(const char* function, uint8_t expectedSensorID, int16_t expectedData, bool bigdata){
	roombaCommand_t c;
	roombaSensorData_t data;
	/*
	 * Baue Command zusammen
	 */
	c.command = ROOMBA_SENSORS;
	c.size = 1;
	c.data[0] = expectedSensorID;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\nSensorID: %d Data: %x\nTest Ergebnis: \n", function, c.command, c.data[0], expectedSensorID, expectedData);
	/*
	 * Sende Kommando um preuefe ob Libaray keine Fehler zurueckgibt
	 */
	assert(roomba_sendCommand(&c) >= 0);
	/*
	 * Preufe ueber write Stub ob Daten Korrekt an Roomba uebermittelt worden sind
	 */
	assert(counter == 2);
	assert(buffer[0] == c.command);
	assert(buffer[1] == c.data[0]);

	/*
	 * 16 Bit daten senden nur wenn erforderlich
	 */
	if(bigdata){
		assert(roomba_read_sim(expectedData >> 8) >= 0);
	}
	assert(roomba_read_sim(expectedData | 256) >= 0);

	/*
	 * Rufe SensorDaten ab
	 */
	assert(roomba_getSensorData(&data) >= 0);
	/*
	 * Gebe Sensordaten aus
	 */
	echoSensorData(&data);
	/*
	 * Preufe Daten die die Libaray entfangen hat
	 */
	assert(data.sensorID == expectedSensorID);
	assert(data.data == expectedData);
	assert(!roomba_hasSensorData());
}

/*
 * Testet ob der Start Kommnad gesendet werden kann
 */
void testSendStart(){
	roombaCommand_t c;
	beforTest();
	printf("Teste %s\nErwartete Daten:\n128\nTest:\n", __FUNCTION__);
	c.command = ROOMBA_START;
	c.size = 0;
	assert(roomba_sendCommand(&c) >= 0);
	assert(counter == 1);
	assert(buffer[0] == c.command);
	assert(!roomba_hasSensorData());
}
/*
 * 8 Bit Sensor Daten Test
 */
void testSendSensor(){
	beforTest();
	sensorTest(__FUNCTION__, ROOMBA_SENSOR_BUMPS_WHEEL_DROPS, 0, false);
}
/*
 * 8 Bit Sensor Daten Test
 */
void testSendSensor2(){
	beforTest();
	sensorTest(__FUNCTION__, ROOMBA_SENSOR_BUMPS_WHEEL_DROPS, 1, false);
}
/*
 * 8 Bit Sensor Daten Test Ende
 */
void testSendSensor3(){
	beforTest();
	sensorTest(__FUNCTION__, ROOMBA_SENSOR_BUMPS_WHEEL_DROPS, UINT_LEAST8_MAX, false);
}
/*
 * 8 Bit Sensor Daten Test Ende
 */
void testSendSensor4(){
	beforTest();
	sensorTest(__FUNCTION__, ROOMBA_SENSOR_BUMPS_WHEEL_DROPS, UINT_LEAST8_MAX-1, false);
}
/*
 * 16 Bit Sensor Daten Test
 */
void testSendSensor16Bit(){
	beforTest();
	sensorTest(__FUNCTION__, ROOMBA_SENSOR_DISTANCE, INT_LEAST16_MAX, true);
}
/*
 * 16 Bit Sensor Daten Test
 */
void testSendSensor16Bit2(){
	beforTest();
	sensorTest(__FUNCTION__, ROOMBA_SENSOR_DISTANCE, INT_LEAST16_MAX-1, true);
}
/*
 * 16 Bit Sensor Negativ Daten Test
 */
void testSendSensor16BitNegativ(){
	beforTest();
	sensorTest(__FUNCTION__, ROOMBA_SENSOR_DISTANCE, INT_LEAST16_MIN, true);
}
/*
 * 16 Bit Sensor Negativ Daten Test
 */
void testSendSensor16BitNegativ2(){
	beforTest();
	sensorTest(__FUNCTION__, ROOMBA_SENSOR_DISTANCE, INT_LEAST16_MIN+1, true);
}
/*
 * Testet ob der Ringbuffer richtig implementiert ist.
 * Der Ringbuffer wird 2 mal volständig durchlaufen
 * Erst 16 Bit dann 8 Bit Testet ob Eintrag gueltig gelschoescht wird
 */
void testSendMany(){
	int i;
	printf("---------------\n=========RingBuffer Test==========\n");
	/*
	 * Test Ring Buffer Implementierung
	 */
	for(i = 0; i < ROOMBA_SENSOR_BUFFER_SIZE*2; i++){  
		if(i < ROOMBA_SENSOR_BUFFER_SIZE){
			beforTest();
			/*
			 * Erzeuge immer ein andres High Byte und Low Byte
			 */
			sensorTest(__FUNCTION__, ROOMBA_SENSOR_DISTANCE, i*256+i, true);
		}else{
			beforTest();
			sensorTest(__FUNCTION__, ROOMBA_SENSOR_BUMPS_WHEEL_DROPS, i%(UINT_LEAST8_MAX+1), false);
		}
	}
	printf("=========RingBuffer Test Ende========== i = %d\n", i);
}

void testSensorGroup(){
	roombaCommand_t c;
	roombaSensorData_t data;
	beforTest();
	c.command = ROOMBA_SENSORS;
	c.size = 1;
	c.data[0] = ROOMBA_SENSOR_GROUP_0;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\nSensorID: %d Data: 0x00 ... 0x1a\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0], ROOMBA_SENSOR_GROUP_0);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);
	/*
	 * Simuliere Roomba
	 */
	assert(roomba_read_sim(0x00) >= 0); /* ID: 7 */
	assert(roomba_read_sim(0x01) >= 0); /* ID: 8 */
	assert(roomba_read_sim(0x02) >= 0); /* ID: 9 */
	assert(roomba_read_sim(0x03) >= 0); /* ID: 10 */
	assert(roomba_read_sim(0x04) >= 0); /* ID: 11 */
	assert(roomba_read_sim(0x05) >= 0); /* ID: 12 */
	assert(roomba_read_sim(0x06) >= 0); /* ID: 13 */
	assert(roomba_read_sim(0x07) >= 0); /* ID: 14 */
	assert(roomba_read_sim(0x08) >= 0); /* ID: 15 */
	assert(roomba_read_sim(0x09) >= 0); /* ID: 16 */
	assert(roomba_read_sim(0x0a) >= 0); /* ID: 17 */
	assert(roomba_read_sim(0x0b) >= 0); /* ID: 18 */
	
	assert(roomba_read_sim(0x0c) >= 0); /* ID: 19 */
	assert(roomba_read_sim(0x0d) >= 0); /* ID: 19 */

	assert(roomba_read_sim(0x0e) >= 0); /* ID: 20 */
	assert(roomba_read_sim(0x0f) >= 0); /* ID: 20 */

	assert(roomba_read_sim(0x11) >= 0); /* ID: 21 */

	assert(roomba_read_sim(0x12) >= 0); /* ID: 22 */
	assert(roomba_read_sim(0x13) >= 0); /* ID: 22 */

	assert(roomba_read_sim(0x14) >= 0); /* ID: 23 */
	assert(roomba_read_sim(0x15) >= 0); /* ID: 23 */

	assert(roomba_read_sim(0x16) >= 0); /* ID: 24 */

	assert(roomba_read_sim(0x17) >= 0); /* ID: 25 */
	assert(roomba_read_sim(0x18) >= 0); /* ID: 25 */

	assert(roomba_read_sim(0x19) >= 0); /* ID: 26 */
	assert(roomba_read_sim(0x1a) >= 0); /* ID: 26 */

	assert(buffer[0] == ROOMBA_SENSORS);
	assert(buffer[1] == ROOMBA_SENSOR_GROUP_0);
	
	/*
	 * Daten entfanngen und prefen
	 */
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 7);
	assert(data.data == 0x00);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 8);
	assert(data.data == 0x01);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 9);
	assert(data.data == 0x02);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 10);
	assert(data.data == 0x03);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 11);
	assert(data.data == 0x04);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 12);
	assert(data.data == 0x05);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 13);
	assert(data.data == 0x06);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 14);
	assert(data.data == 0x07);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 15);
	assert(data.data == 0x08);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 16);
	assert(data.data == 0x09);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 17);
	assert(data.data == 0x0a);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 18);
	assert(data.data == 0x0b);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 19);
	assert(data.data == 0x0c0d);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 20);
	assert(data.data == 0x0e0f);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 21);
	assert(data.data == 0x11);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 22);
	assert(data.data == 0x1213);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 23);
	assert(data.data == 0x1415);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 24);
	assert(data.data == 0x16);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 25);
	assert(data.data == 0x1718);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 26);
	assert(data.data == 0x191a);

	assert(!roomba_hasSensorData());
}

void testSensorGroupAll(){
	roombaCommand_t c;
	roombaSensorData_t data;
	beforTest();
	c.command = ROOMBA_SENSORS;
	c.size = 1;
	c.data[0] = ROOMBA_SENSOR_GROUP_100;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\nSensorID: %d Data: 0x00 ... 0x1a\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0], ROOMBA_SENSOR_GROUP_100);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_SENSORS);
	assert(buffer[1] == ROOMBA_SENSOR_GROUP_100);
	/*
	 * Simuliere Roomba
	 */
	assert(roomba_read_sim(0x00) >= 0); /* ID: 7 */
	assert(roomba_read_sim(0x01) >= 0); /* ID: 8 */
	assert(roomba_read_sim(0x02) >= 0); /* ID: 9 */
	assert(roomba_read_sim(0x03) >= 0); /* ID: 10 */
	assert(roomba_read_sim(0x04) >= 0); /* ID: 11 */
	assert(roomba_read_sim(0x05) >= 0); /* ID: 12 */
	assert(roomba_read_sim(0x06) >= 0); /* ID: 13 */
	assert(roomba_read_sim(0x07) >= 0); /* ID: 14 */
	assert(roomba_read_sim(0x08) >= 0); /* ID: 15 */
	assert(roomba_read_sim(0x09) >= 0); /* ID: 16 */
	assert(roomba_read_sim(0x0a) >= 0); /* ID: 17 */
	assert(roomba_read_sim(0x0b) >= 0); /* ID: 18 */
	
	assert(roomba_read_sim(0x0c) >= 0); /* ID: 19 */
	assert(roomba_read_sim(0x0d) >= 0); /* ID: 19 */

	assert(roomba_read_sim(0x0e) >= 0); /* ID: 20 */
	assert(roomba_read_sim(0x0f) >= 0); /* ID: 20 */

	assert(roomba_read_sim(0x11) >= 0); /* ID: 21 */

	assert(roomba_read_sim(0x12) >= 0); /* ID: 22 */
	assert(roomba_read_sim(0x13) >= 0); /* ID: 22 */

	assert(roomba_read_sim(0x14) >= 0); /* ID: 23 */
	assert(roomba_read_sim(0x15) >= 0); /* ID: 23 */

	assert(roomba_read_sim(0x16) >= 0); /* ID: 24 */

	assert(roomba_read_sim(0x17) >= 0); /* ID: 25 */
	assert(roomba_read_sim(0x18) >= 0); /* ID: 25 */

	assert(roomba_read_sim(0x19) >= 0); /* ID: 26 */
	assert(roomba_read_sim(0x1a) >= 0); /* ID: 26 */

	assert(roomba_read_sim(0x1b) >= 0); /* ID: 27 */
	assert(roomba_read_sim(0x1c) >= 0); /* ID: 27 */

	assert(roomba_read_sim(0x1d) >= 0); /* ID: 28 */
	assert(roomba_read_sim(0x1e) >= 0); /* ID: 28 */

	assert(roomba_read_sim(0x1f) >= 0); /* ID: 29 */
	assert(roomba_read_sim(0x20) >= 0); /* ID: 29 */

	assert(roomba_read_sim(0x21) >= 0); /* ID: 30 */
	assert(roomba_read_sim(0x22) >= 0); /* ID: 30 */

	assert(roomba_read_sim(0x23) >= 0); /* ID: 31 */
	assert(roomba_read_sim(0x24) >= 0); /* ID: 31 */

	assert(roomba_read_sim(0x25) >= 0); /* ID: 32 */
	assert(roomba_read_sim(0x26) >= 0); /* ID: 33 */
	assert(roomba_read_sim(0x26) >= 0); /* ID: 33 */
	assert(roomba_read_sim(0x27) >= 0); /* ID: 34 */
	assert(roomba_read_sim(0x28) >= 0); /* ID: 35 */
	assert(roomba_read_sim(0x29) >= 0); /* ID: 36 */
	assert(roomba_read_sim(0x2a) >= 0); /* ID: 37 */
	assert(roomba_read_sim(0x2b) >= 0); /* ID: 38 */
	
	assert(roomba_read_sim(0x2c) >= 0); /* ID: 39 */
	assert(roomba_read_sim(0x2d) >= 0); /* ID: 39 */

	assert(roomba_read_sim(0x2e) >= 0); /* ID: 40 */
	assert(roomba_read_sim(0x2f) >= 0); /* ID: 40 */

	assert(roomba_read_sim(0x30) >= 0); /* ID: 41 */
	assert(roomba_read_sim(0x31) >= 0); /* ID: 41 */

	assert(roomba_read_sim(0x32) >= 0); /* ID: 42 */
	assert(roomba_read_sim(0x33) >= 0); /* ID: 42 */

	assert(roomba_read_sim(0x34) >= 0); /* ID: 43 */
	assert(roomba_read_sim(0x35) >= 0); /* ID: 43 */

	assert(roomba_read_sim(0x36) >= 0); /* ID: 44 */
	assert(roomba_read_sim(0x37) >= 0); /* ID: 44 */

	assert(roomba_read_sim(0x38) >= 0); /* ID: 45 */

	assert(roomba_read_sim(0x39) >= 0); /* ID: 46 */
	assert(roomba_read_sim(0x3a) >= 0); /* ID: 46 */

	assert(roomba_read_sim(0x3b) >= 0); /* ID: 47 */
	assert(roomba_read_sim(0x3c) >= 0); /* ID: 47 */

	assert(roomba_read_sim(0x3d) >= 0); /* ID: 48 */
	assert(roomba_read_sim(0x3e) >= 0); /* ID: 48 */

	assert(roomba_read_sim(0x3f) >= 0); /* ID: 49 */
	assert(roomba_read_sim(0x40) >= 0); /* ID: 49 */
	
	assert(roomba_read_sim(0x41) >= 0); /* ID: 50 */
	assert(roomba_read_sim(0x42) >= 0); /* ID: 50 */

	assert(roomba_read_sim(0x43) >= 0); /* ID: 51 */
	assert(roomba_read_sim(0x44) >= 0); /* ID: 51 */

	assert(roomba_read_sim(0x45) >= 0); /* ID: 52 */

	assert(roomba_read_sim(0x46) >= 0); /* ID: 53 */

	assert(roomba_read_sim(0x47) >= 0); /* ID: 54 */
	assert(roomba_read_sim(0x48) >= 0); /* ID: 54 */

	assert(roomba_read_sim(0x49) >= 0); /* ID: 55 */
	assert(roomba_read_sim(0x4a) >= 0); /* ID: 55 */

	assert(roomba_read_sim(0x4b) >= 0); /* ID: 56 */
	assert(roomba_read_sim(0x4c) >= 0); /* ID: 56 */

	assert(roomba_read_sim(0x4d) >= 0); /* ID: 57 */
	assert(roomba_read_sim(0x4e) >= 0); /* ID: 57 */

	assert(roomba_read_sim(0x4f) >= 0); /* ID: 58 */
	

	assert(buffer[0] == ROOMBA_SENSORS);
	assert(buffer[1] == ROOMBA_SENSOR_GROUP_100);
	
	/*
	 * Daten entfanngen und prefen
	 */
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 7);
	assert(data.data == 0x00);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 8);
	assert(data.data == 0x01);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 9);
	assert(data.data == 0x02);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 10);
	assert(data.data == 0x03);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 11);
	assert(data.data == 0x04);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 12);
	assert(data.data == 0x05);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 13);
	assert(data.data == 0x06);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 14);
	assert(data.data == 0x07);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 15);
	assert(data.data == 0x08);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 16);
	assert(data.data == 0x09);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 17);
	assert(data.data == 0x0a);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 18);
	assert(data.data == 0x0b);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 19);
	assert(data.data == 0x0c0d);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 20);
	assert(data.data == 0x0e0f);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 21);
	assert(data.data == 0x11);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 22);
	assert(data.data == 0x1213);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 23);
	assert(data.data == 0x1415);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 24);
	assert(data.data == 0x16);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 25);
	assert(data.data == 0x1718);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 26);
	assert(data.data == 0x191a);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 27);
	assert(data.data == 0x1b1c);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 28);
	assert(data.data == 0x1d1e);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 29);
	assert(data.data == 0x1f20);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 30);
	assert(data.data == 0x2122);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 31);
	assert(data.data == 0x2324);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 32);
	assert(data.data == 0x25);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 33);
	assert(data.data == 0x2626);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 34);
	assert(data.data == 0x27);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 35);
	assert(data.data == 0x28);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 36);
	assert(data.data == 0x29);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 37);
	assert(data.data == 0x2a);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 38);
	assert(data.data == 0x2b);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 39);
	assert(data.data == 0x2c2d);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 40);
	assert(data.data == 0x2e2f);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 41);
	assert(data.data == 0x3031);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 42);
	assert(data.data == 0x3233);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 43);
	assert(data.data == 0x3435);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 44);
	assert(data.data == 0x3637);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 45);
	assert(data.data == 0x38);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 46);
	assert(data.data == 0x393a);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 47);
	assert(data.data == 0x3b3c);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 48);
	assert(data.data == 0x3d3e);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 49);
	assert(data.data == 0x3f40);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 50);
	assert(data.data == 0x4142);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 51);
	assert(data.data == 0x4344);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 52);
	assert(data.data == 0x45);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 53);
	assert(data.data == 0x46);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 54);
	assert(data.data == 0x4748);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 55);
	assert(data.data == 0x494a);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 56);
	assert(data.data == 0x4b4c);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 57);
	assert(data.data == 0x4d4e);
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == 58);
	assert(data.data == 0x4f);
	
	assert(!roomba_hasSensorData());
}

void testQueryList(){
	roombaCommand_t c;
	roombaSensorData_t data;
	beforTest();
	c.command = ROOMBA_QUERY_LIST;
	c.size = 2;
	c.data[0] = 1;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\n%d\nSensorID: %d Data: 0x42\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0], c.data[1] , ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_QUERY_LIST);
	assert(buffer[1] = 1);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);

	assert(roomba_read_sim(0x42) >= 0);

	assert(roomba_hasSensorData());

	assert(roomba_getSensorData(&data) >= 0);

	assert(data.sensorID == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(data.data == 0x42);
	echoSensorData(&data);
}
void testQueryList2(){
	roombaCommand_t c;
	roombaSensorData_t data;
	beforTest();
	c.command = ROOMBA_QUERY_LIST;
	c.size = 3;
	c.data[0] = 2;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	c.data[2] = ROOMBA_SENSOR_CLIFF_SIGNAL_LEFT;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\n%d\n%d\nSensorID: %d Data: 0x42\n\nSensorID: %d Data: 0x1234\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0], c.data[1], c.data[2] , ROOMBA_SENSOR_BUMPS_WHEEL_DROPS, ROOMBA_SENSOR_CLIFF_SIGNAL_LEFT);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_QUERY_LIST);
	assert(buffer[1] == 2);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(buffer[3] == ROOMBA_SENSOR_CLIFF_SIGNAL_LEFT);

	assert(roomba_read_sim(0x42) >= 0);
	assert(roomba_read_sim(0x12) >= 0);
	assert(roomba_read_sim(0x34) >= 0);

	assert(roomba_hasSensorData());

	assert(roomba_getSensorData(&data) >= 0);

	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(data.data == 0x42);

	assert(roomba_hasSensorData());

	assert(roomba_getSensorData(&data) >= 0);

	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_CLIFF_SIGNAL_LEFT);
	assert(data.data == (uint16_t) 0x1234);
}
void testQueryList10(){
	roombaCommand_t c;
	roombaSensorData_t data;
	beforTest();
	c.command = ROOMBA_QUERY_LIST;
	c.size = 11;
	c.data[0] = 10;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	c.data[2] = ROOMBA_SENSOR_WALL;
	c.data[3] = ROOMBA_SENSOR_CLIFF_LEFT;
	c.data[4] = ROOMBA_SENSOR_CLIFF_FRONT_LEFT;
	c.data[5] = ROOMBA_SENSOR_CLIFF_FRONT_RIGHT;
	c.data[6] = ROOMBA_SENSOR_DISTANCE;
	c.data[7] = ROOMBA_SENSOR_ANGLE;
	c.data[8] = ROOMBA_SENSOR_VOLTAGE;
	c.data[9] = ROOMBA_SENSOR_CURRENT;
	c.data[10] = ROOMBA_SENSOR_BATTERY_CHARGE;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\nSensorID: 10 Stück Data: 0x00..0x0a\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0]);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_QUERY_LIST);
	assert(buffer[1] = 10);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(buffer[3] = ROOMBA_SENSOR_WALL);
	assert(buffer[4] = ROOMBA_SENSOR_CLIFF_LEFT);
	assert(buffer[5] = ROOMBA_SENSOR_CLIFF_FRONT_LEFT);
	assert(buffer[6] = ROOMBA_SENSOR_CLIFF_FRONT_RIGHT);
	assert(buffer[7] = ROOMBA_SENSOR_DISTANCE);
	assert(buffer[8] = ROOMBA_SENSOR_ANGLE);
	assert(buffer[9] = ROOMBA_SENSOR_VOLTAGE);
	assert(buffer[10] = ROOMBA_SENSOR_CURRENT);
	assert(buffer[11] = ROOMBA_SENSOR_BATTERY_CHARGE);

	assert(roomba_read_sim(0x0) >= 0);
	assert(roomba_read_sim(0x1) >= 0);
	assert(roomba_read_sim(0x2) >= 0);
	assert(roomba_read_sim(0x3) >= 0);
	assert(roomba_read_sim(0x4) >= 0);
	assert(roomba_read_sim(0x5) >= 0);

	assert(roomba_read_sim(0x6) >= 0);
	assert(roomba_read_sim(0x7) >= 0);
	assert(roomba_read_sim(0x8) >= 0);
	assert(roomba_read_sim(0x9) >= 0);
	assert(roomba_read_sim(0xa) >= 0);
	assert(roomba_read_sim(0xb) >= 0);
	assert(roomba_read_sim(0xc) >= 0);
	assert(roomba_read_sim(0xd) >= 0);
	assert(roomba_read_sim(0xe) >= 0);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(data.data == 0x00);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_WALL);
	assert(data.data == 0x01);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_CLIFF_LEFT);
	assert(data.data == 0x02);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_CLIFF_FRONT_LEFT);
	assert(data.data == 0x03);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_CLIFF_FRONT_RIGHT);
	assert(data.data == 0x04);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_DISTANCE);
	assert(data.data == 0x0506);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_ANGLE);
	assert(data.data == 0x0708);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_VOLTAGE);
	assert(data.data == 0x090a);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_CURRENT);
	assert(data.data == 0x0b0c);

	assert(roomba_hasSensorData());
	assert(roomba_getSensorData(&data) >= 0);
	echoSensorData(&data);
	assert(data.sensorID == ROOMBA_SENSOR_BATTERY_CHARGE);
	assert(data.data == 0x0d0e);

	assert(!roomba_hasSensorData());
}
void testQueryListGruppen(){
	roombaCommand_t c;
	int i = 0;
	int j = 0;
	beforTest();
	printf("Teste %s\n", __FUNCTION__);
	c.command = ROOMBA_QUERY_LIST;
	c.size = 2;
	c.data[0] = 1;
	c.data[1] = ROOMBA_SENSOR_GROUP_100;
	assert(roomba_sendCommand(&c) >= 0);

	assert(counter == (ROOMBA_SENSOR_GROUP_100_STOP-ROOMBA_SENSOR_GROUP_100_START)+1+2);
	assert(buffer[j++] == ROOMBA_QUERY_LIST);
	printf("%d(buffer[%d]) == %d\n", buffer[j], j, ROOMBA_SENSOR_GROUP_100_STOP-ROOMBA_SENSOR_GROUP_100_START+1);
	assert(buffer[j++] == ROOMBA_SENSOR_GROUP_100_STOP-ROOMBA_SENSOR_GROUP_100_START+1);
	for(i = ROOMBA_SENSOR_GROUP_100_START; i <= ROOMBA_SENSOR_GROUP_100_STOP; i++){
		printf("%d(buffer[j]) == %d(i)\n", buffer[j], i);
		assert(buffer[j++] == i);
	}

}
void testQueryListGruppenError(){
	roombaCommand_t c;
	beforTest();
	printf("Teste %s\n", __FUNCTION__);
	c.command = ROOMBA_QUERY_LIST;
	c.size = 3;
	c.data[0] = 2;
	c.data[1] = ROOMBA_SENSOR_GROUP_100;
	c.data[2] = ROOMBA_SENSOR_GROUP_0;
	assert(roomba_sendCommand(&c) < 0);
	assert(roomba_getError() == ROOMBA_ERROR_TO_BIG);
}
void testQueryListGruppenError2(){
	roombaCommand_t c;
	beforTest();
	printf("Teste %s\n", __FUNCTION__);
	c.command = ROOMBA_QUERY_LIST;
	c.size = 4;
	c.data[0] = 3;
	c.data[1] = ROOMBA_SENSOR_GROUP_6;
	c.data[2] = ROOMBA_SENSOR_GROUP_106;
	c.data[3] = ROOMBA_SENSOR_GROUP_6;
	assert(roomba_sendCommand(&c) < 0);
	assert(roomba_getError() == ROOMBA_ERROR_TO_BIG);
}
void testQueryListGruppenError3(){
	roombaCommand_t c;
	beforTest();
	printf("Teste %s\n", __FUNCTION__);
	c.command = ROOMBA_QUERY_LIST;
	c.size = 3;
	c.data[0] = 2;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	c.data[2] = ROOMBA_SENSOR_GROUP_100;
	assert(roomba_sendCommand(&c) < 0);
	assert(roomba_getError() == ROOMBA_ERROR_TO_BIG);
}

void testStream(){
	roombaCommand_t c;
	roombaSensorData_t data;
	int i; 
	beforTest();
	c.command = ROOMBA_STREAM;
	c.size = 2;
	c.data[0] = 1;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\n%d\nSensorID: %d Data: 0x42\n150\n0\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0], c.data[1] , ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_STREAM);
	assert(buffer[1] == 1);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);

	/*
	 * Wiederhole Senden des Stream 10 mal 
	 */
	for(i = 0; i < 10; i++){

		assert(roomba_read_sim((char) 19) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x02) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x42) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0xA2) >= 0);
	
		assert(roomba_hasSensorData());
	
		assert(roomba_getSensorData(&data) >= 0);
	
		assert(data.sensorID == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
		assert(data.data == 0x42);
		echoSensorData(&data);

		assert(!roomba_hasSensorData());
	}
	
	c.command = ROOMBA_STREAM_PAUSE_RESUME;
	c.size = 1;
	c.data[0] = 0;

	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[3] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[4] == 0);
	/*
	 * Test ob Stream abgeschaltet ist
	 */	
	assert(roomba_read_sim(0x00) < 0);

}

void testStream2(){
	roombaCommand_t c;
	roombaSensorData_t data;
	int i; 
	beforTest();
	c.command = ROOMBA_STREAM;
	c.size = 3;
	c.data[0] = 2;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	c.data[2] = ROOMBA_SENSOR_VOLTAGE;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\n%d\n%d\nSensorID: %d Data: 0x42\nSensorID: %d Data: 0x4344\n150\n0\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0], c.data[1], c.data[2] , ROOMBA_SENSOR_BUMPS_WHEEL_DROPS, ROOMBA_SENSOR_VOLTAGE);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_STREAM);
	assert(buffer[1] == 2);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(buffer[3] == ROOMBA_SENSOR_VOLTAGE);

	/*
	 * Wiederhole Senden des Stream 10 mal 
	 */
	for(i = 0; i < 10; i++){

		assert(roomba_read_sim((char) 19) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x05) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x42) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_VOLTAGE) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x43) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x44) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x02) >= 0);
	
		assert(roomba_hasSensorData());
	
		assert(roomba_getSensorData(&data) >= 0);
		assert(data.sensorID == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
		assert(data.data == 0x42);
		echoSensorData(&data);

		assert(roomba_getSensorData(&data) >= 0);
		assert(data.sensorID == ROOMBA_SENSOR_VOLTAGE);
		assert(data.data == 0x4344);
		echoSensorData(&data);

		assert(!roomba_hasSensorData());
	}
	
	c.command = ROOMBA_STREAM_PAUSE_RESUME;
	c.size = 1;
	c.data[0] = 0;

	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[4] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[5] == 0);
	/*
	 * Test ob Stream abgeschaltet ist
	 */	
	assert(roomba_read_sim(0x00) < 0);

}

void testStream3(){
	roombaCommand_t c;
	roombaSensorData_t data;
	int i; 
	beforTest();
	c.command = ROOMBA_STREAM;
	c.size = 11;
	c.data[0] = 10;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	c.data[2] = ROOMBA_SENSOR_WALL;
	c.data[3] = ROOMBA_SENSOR_CLIFF_LEFT;
	c.data[4] = ROOMBA_SENSOR_CLIFF_FRONT_LEFT;
	c.data[5] = ROOMBA_SENSOR_CLIFF_FRONT_RIGHT;
	c.data[6] = ROOMBA_SENSOR_DISTANCE;
	c.data[7] = ROOMBA_SENSOR_ANGLE;
	c.data[8] = ROOMBA_SENSOR_VOLTAGE;
	c.data[9] = ROOMBA_SENSOR_CURRENT;
	c.data[10] = ROOMBA_SENSOR_BATTERY_CHARGE;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\nSensorID: 10 Stück Data: 0x00..0x0a\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0]);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_STREAM);
	assert(buffer[1] = 10);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	assert(buffer[3] = ROOMBA_SENSOR_WALL);
	assert(buffer[4] = ROOMBA_SENSOR_CLIFF_LEFT);
	assert(buffer[5] = ROOMBA_SENSOR_CLIFF_FRONT_LEFT);
	assert(buffer[6] = ROOMBA_SENSOR_CLIFF_FRONT_RIGHT);
	assert(buffer[7] = ROOMBA_SENSOR_DISTANCE);
	assert(buffer[8] = ROOMBA_SENSOR_ANGLE);
	assert(buffer[9] = ROOMBA_SENSOR_VOLTAGE);
	assert(buffer[10] = ROOMBA_SENSOR_CURRENT);
	assert(buffer[11] = ROOMBA_SENSOR_BATTERY_CHARGE);

	/*
	 * Wiederhole Senden des Stream 200 mal 
	 * 
	 * Buffer Ende testen
	 */
	for(i = 0; i < 200; i++){
		assert(roomba_read_sim((char) 19) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x19) >= 0);
		assert(!roomba_hasSensorData());

		assert(roomba_read_sim(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x0) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_WALL) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x1) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_CLIFF_LEFT) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x2) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_CLIFF_FRONT_LEFT) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x3) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_CLIFF_FRONT_RIGHT) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x4) >= 0);

		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_DISTANCE) >= 0);
		assert(roomba_read_sim(0x5) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x6) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_ANGLE) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x7) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x8) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_VOLTAGE) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0x9) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0xa) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_CURRENT) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0xb) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0xc) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(ROOMBA_SENSOR_BATTERY_CHARGE) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0xd) >= 0);
		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0xe) >= 0);

		assert(!roomba_hasSensorData());
		assert(roomba_read_sim(0xd1) >= 0);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
		assert(data.data == 0x00);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_WALL);
		assert(data.data == 0x01);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_CLIFF_LEFT);
		assert(data.data == 0x02);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_CLIFF_FRONT_LEFT);
		assert(data.data == 0x03);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_CLIFF_FRONT_RIGHT);
		assert(data.data == 0x04);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_DISTANCE);
		assert(data.data == 0x0506);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_ANGLE);
		assert(data.data == 0x0708);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_VOLTAGE);
		assert(data.data == 0x090a);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_CURRENT);
		assert(data.data == 0x0b0c);

		assert(roomba_hasSensorData());
		assert(roomba_getSensorData(&data) >= 0);
		echoSensorData(&data);
		assert(data.sensorID == ROOMBA_SENSOR_BATTERY_CHARGE);
		assert(data.data == 0x0d0e);


		assert(!roomba_hasSensorData());
	}
	
	c.command = ROOMBA_STREAM_PAUSE_RESUME;
	c.size = 1;
	c.data[0] = 0;

	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[12] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[13] == 0);
	/*
	 * Test ob Stream abgeschaltet ist
	 */	
	assert(roomba_read_sim(0x00) < 0);

}

void testStream4(){
	roombaCommand_t c;
	beforTest();
	c.command = ROOMBA_STREAM;
	c.size = 2;
	c.data[0] = 1;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\n%d\nSensorID: %d Data: 0x42\n150\n0\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0], c.data[1] , ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_STREAM);
	assert(buffer[1] == 1);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);

	/*
	 * Sende Kommando
	 * testen Verhalten bei erneuten senden
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[3] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[4] == 0);

	assert(buffer[5] == ROOMBA_STREAM);
	assert(buffer[6] == 1);
	assert(buffer[7] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);


	c.command = ROOMBA_STREAM_PAUSE_RESUME;
	c.size = 1;
	c.data[0] = 0;

	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[8] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[9] == 0);
	/*
	 * Test ob Stream abgeschaltet ist
	 */	
	assert(roomba_read_sim(0x00) < 0);
}

void testStream5(){
	roombaCommand_t c;
	beforTest();
	c.command = ROOMBA_STREAM;
	c.size = 2;
	c.data[0] = 1;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_STREAM);
	assert(buffer[1] == 1);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);


	c.command = ROOMBA_STREAM_PAUSE_RESUME;
	c.size = 1;
	c.data[0] = 0;

	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[3] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[4] == 0);
	/*
	 * Test ob Stream abgeschaltet ist
	 */	
	assert(roomba_read_sim(0x00) < 0);
	assert(buffer[5] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[6] == 0);


	/*
	 * Aktiviere Stream wieder
	 */
	c.command = ROOMBA_STREAM_PAUSE_RESUME;
	c.size = 1;
	c.data[0] = 1;

	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[7] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[8] == 1);

	/*
	 * Teste ob Stream nach reaktivieren noch arbeitet
	 */
	assert(roomba_read_sim((char) 19) >= 0);
	assert(!roomba_hasSensorData());
	assert(roomba_read_sim(0x02) >= 0);
	assert(!roomba_hasSensorData());
	assert(roomba_read_sim(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS) >= 0);
	assert(!roomba_hasSensorData());
	assert(roomba_read_sim(0x42) >= 0);
	assert(!roomba_hasSensorData());
	assert(roomba_read_sim(0xA2) >= 0);

	assert(roomba_hasSensorData());

	c.command = ROOMBA_STREAM_PAUSE_RESUME;
	c.size = 1;
	c.data[0] = 0;

	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[9] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[10] == 0);
	/*
	 * Test ob Stream abgeschaltet ist
	 */	
	assert(roomba_read_sim(0x00) < 0);
}

void testStreamError(){
	roombaCommand_t c;
	beforTest();
	c.command = ROOMBA_STREAM;
	c.size = 2;
	c.data[0] = 1;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	printf("Teste %s\nErwarete Send Data:\n%d\n%d\n%d\nSensorID: %d Data: 0x42\n150\n0\nTest Ergebnis: \n", __FUNCTION__, c.command, c.data[0], c.data[1] , ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_STREAM);
	assert(buffer[1] == 1);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);

	/*
	 * Sende Falscher Header
	 */
	roomba_read_sim((char) 20);
	assert(!roomba_hasSensorData());
	assert(roomba_getError() == ROOMBA_ERROR_HEADER_NOT_FOUND); 

	assert(roomba_read_sim((char) 19) >= 0);
	assert(!roomba_hasSensorData());
	assert(roomba_read_sim(0x02) >= 0);
	assert(!roomba_hasSensorData());

	/*
	 * Falsche Sensor ID
	 */
	roomba_read_sim(0x42);
	assert(!roomba_hasSensorData());
	assert(roomba_getError() == ROOMBA_ERROR_SENSORID_NOT_FOUND);


	assert(roomba_read_sim((char) 19) >= 0);
	assert(!roomba_hasSensorData());
	/*
	 * teste Flasche Groese
	 */
	assert(roomba_read_sim(0x03) >= 0);
	assert(!roomba_hasSensorData());


	assert(roomba_read_sim(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS) >= 0);
	assert(!roomba_hasSensorData());
	assert(roomba_read_sim(0x42) >= 0);
	assert(!roomba_hasSensorData());
	roomba_read_sim(0x00);
	assert(!roomba_hasSensorData());
	assert(roomba_getError() == ROOMBA_ERROR_CHECKSUM_NOT_VALID);

	assert(roomba_read_sim((char) 19) >= 0);
	assert(!roomba_hasSensorData());
	assert(roomba_read_sim(0x02) >= 0);
	assert(!roomba_hasSensorData());


	assert(roomba_read_sim(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS) >= 0);
	assert(!roomba_hasSensorData());
	assert(roomba_read_sim(0x42) >= 0);
	assert(!roomba_hasSensorData());
	/*
	 * Flasche Checksumme
	 */
	roomba_read_sim(0xA1);
	assert(!roomba_hasSensorData());
	assert(roomba_getError() == ROOMBA_ERROR_CHECKSUM_NOT_VALID);

	assert(!roomba_hasSensorData());
	
	c.command = ROOMBA_STREAM_PAUSE_RESUME;
	c.size = 1;
	c.data[0] = 0;

	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[3] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[4] == 0);
	/*
	 * Test ob Stream abgeschaltet ist
	 */	
	assert(roomba_read_sim(0x00) < 0);

}
void testStreamError2(){
	roombaCommand_t c;
	beforTest();
	printf("Teste %s\n", __FUNCTION__);
	c.command = ROOMBA_STREAM;
	c.size = 3;
	c.data[0] = 2;
	c.data[1] = ROOMBA_SENSOR_GROUP_100;
	c.data[2] = ROOMBA_SENSOR_GROUP_0;
	assert(roomba_sendCommand(&c) < 0);
	assert(roomba_getError() == ROOMBA_ERROR_TO_BIG);
}
void testStreamError3(){
	roombaCommand_t c;
	beforTest();
	printf("Teste %s\n", __FUNCTION__);
	c.command = ROOMBA_STREAM;
	c.size = 4;
	c.data[0] = 3;
	c.data[1] = ROOMBA_SENSOR_GROUP_6;
	c.data[2] = ROOMBA_SENSOR_GROUP_106;
	c.data[3] = ROOMBA_SENSOR_GROUP_6;
	assert(roomba_sendCommand(&c) < 0);
	assert(roomba_getError() == ROOMBA_ERROR_TO_BIG);
}
void testStreamError4(){
	roombaCommand_t c;
	beforTest();
	printf("Teste %s\n", __FUNCTION__);
	c.command = ROOMBA_STREAM;
	c.size = 4;
	c.size = 3;
	c.data[0] = 2;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	c.data[2] = ROOMBA_SENSOR_GROUP_100;
	assert(roomba_sendCommand(&c) < 0);
	assert(roomba_getError() == ROOMBA_ERROR_TO_BIG);
}

void bufferOverFlowTest(){
	int i;
	roombaCommand_t command;
	beforTest();
	for(i = 0; i < ROOMBA_SENSOR_BUFFER_SIZE*3; i++){
		command.command = ROOMBA_SENSORS;
		command.size = 1;
		command.data[0] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
		roomba_sendCommand(&command);
		roomba_read_sim(0x1);
		roomba_hasSensorData();/*Flush fals SHORT_READ aktiv ist*/
	}
	assert(roomba_getError() == ROOMBA_ERROR_BUFFER_FULL);
}

void bufferOverFlowTest2(){
	int i;
	roombaCommand_t c;
	beforTest();

	c.command = ROOMBA_STREAM;
	c.size = 2;
	c.data[0] = 1;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;

	/*
	 * Sende Kommando
	 */
	assert(roomba_sendCommand(&c) >= 0);

	assert(buffer[0] == ROOMBA_STREAM);
	assert(buffer[1] == 1);
	assert(buffer[2] == ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
	
	for(i = 0; i < ROOMBA_SENSOR_BUFFER_SIZE; i++){
		roomba_read_sim((char) 19);
		roomba_read_sim(0x02);
		roomba_read_sim(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
		roomba_read_sim(0x42);
		roomba_read_sim(0xA2);
	}
	roomba_hasSensorData();
	for(i = 0; i < ROOMBA_SENSOR_BUFFER_SIZE; i++){
		roomba_read_sim((char) 19);
		roomba_read_sim(0x02);
		roomba_read_sim(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS);
		roomba_read_sim(0x42);
		roomba_read_sim(0xA2);
	}
	assert(roomba_getError() == ROOMBA_ERROR_BUFFER_FULL_CHAR);
}

void testWriteError(){
	roombaCommand_t c;
	beforTest();
	writeError = true;
	c.command = ROOMBA_SENSORS;
	c.size = 1;
	c.data[0] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	assert(roomba_sendCommand(&c) < 0);
	assert(roomba_getError() == ROOMBA_ERROR_WRITE);
}

void testAbortSensor(){
	roombaCommand_t c;
	beforTest();
	c.command = ROOMBA_SENSORS;
	c.size = 1;
	c.data[0] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	assert(roomba_sendCommand(&c) >= 0);
	roomba_abort_sensor();
	assert(roomba_getError() == ROOMBA_ERROR_ABORT_SENSOR);
}
void testAbortSensor2(){
	beforTest();
	roomba_abort_sensor();
	assert(roomba_getError() == ROOMBA_ERROR_NO);
}
void testAbortSensor3(){
	roombaCommand_t c;
	beforTest();
	c.command = ROOMBA_STREAM;
	c.size = 1;
	c.data[0] = 1;
	c.data[1] = ROOMBA_SENSOR_BUMPS_WHEEL_DROPS;
	assert(roomba_sendCommand(&c) >= 0);
	roomba_abort_sensor();
	assert(roomba_getError() == ROOMBA_ERROR_ABORT_SENSOR);
	assert(buffer[2] == ROOMBA_STREAM_PAUSE_RESUME);
	assert(buffer[3] == 0);
}

void startTesting(){
	testSendStart();
	testSendSensor();
	testSendSensor2();
	testSendSensor3();
	testSendSensor4();
	testSendSensor16Bit();
	testSendSensor16Bit2();
	testSendSensor16BitNegativ();
	testSendSensor16BitNegativ2();
	#ifdef TESTMANY
	testSendMany();
	#endif
	testSensorGroup();
	testSensorGroupAll();
	testQueryList();
	testQueryList2();
	testQueryList10();
	testQueryListGruppen();
	testQueryListGruppenError();
	testQueryListGruppenError2();
	testQueryListGruppenError3();

	testStream();
	testStream2();
	testStream3();
	testStream4();
	testStream5();

	testStreamError();
	testStreamError2();
	testStreamError3();
	testStreamError4();

	bufferOverFlowTest();
	bufferOverFlowTest2();

	testWriteError();

	testAbortSensor();
	testAbortSensor2();
	testAbortSensor3();

	printf("Alle Test erflogreich\n");

}

