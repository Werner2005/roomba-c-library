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
#ifndef ROOMBA_PRINT_H
#define ROOMBA_PRINT_H
#include <roomba_def.h>
#include <stddef.h>
/**
 * @defgroup roomba_print Roomba Print Libary
 * 
 * Definiert Befehle um {@link roombaCommand_t} und {@link roombaSensorData_t} auszugeben
 * 
 * <b>Wahrung: Libary verbraucht sehr viel Speicher!</b>
 * @{
 */
/**
 * Gibt einen Sensor Wert mit Namen aus
 * 
 * Syntax: 
 *
 *      SensorID: NAME_DES_SENSORS(%d) Data: 0x%x = %d\n
 * 
 * @param data Wert der Ausgeben werden soll
 */
void roomba_printSensorData(roombaSensorData_t* data);
/**
 * Gibt ein Befehl aus mit der 
 * 
 * Syntax: 
 *
 *      Command: NAME_DES_COMMAND(%d) Size: %d Data: %d = 0x%x, ...\n
 *
 * @param c Kommando das Ausgaben werden soll
 */
void roomba_printCommand(roombaCommand_t* c);
/**
 * Aufbau eines Sensor Strings
 */
struct sensorString{
	/**
	 * Sensor ID
	 */
	uint8_t sensorID;
	/**
	 * Sensor ID als String
	 */
	char* sensorStr;
};
typedef struct sensorString sensorString_t;
/**
 * Array mit allen Sensor Bezeichnungen
 * 
 * Beispiel: 
 *
 *      printf("%s", GET_SENSOR_STRING(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS));
 */
extern sensorString_t sensorStrings[];
/**
 * Aufbau eines Kommando Strings
 */
struct commandString{
	/**
	 * Kommando ID
	 */
	uint8_t commandID;
	/**
	 * Kommando ID als String
	 */
	char* commandStr;
};
typedef struct commandString commandString_t;
/**
 * Gibt Sensor String zurück
 * @return String mit der Bezeichnung
 */
#define GET_SENSOR_STRING(i) sensorStrings[i].sensorStr
/**
 * Größe des SensorStrings Array
 */
extern const size_t sensorStringSize;
/**
 * Bestimmt Größe des Arrays
 * @return Größe des Arrays
 */
#define GET_SENSOR_STRING_SIZE() sensorStringSize

/**
 * Array mit allen Kommando Bezeichnungen
 * 
 * Beispiel: 
 *
 *      printf("%s", GET_COMMAND_STRING(ROOMBA_DRIVE));
 */
extern commandString_t commandStrings[];
/**
 * Größe des CommandStrings Array
 */
extern const size_t commandStringSize;

/**
 * Gibt Kommando String zurück
 * @return String mit der Bezeichung
 */
#define GET_COMMAND_STRING(i) commandStrings[i-ROOMBA_START].commandStr
/**
 * Bestimmt Größe des Arrays
 * @return Größe des Arrays
 */
#define GET_COMMAND_STRING_SIZE() commandStringSize
/** @}*/ /* roomab_print */
#endif
