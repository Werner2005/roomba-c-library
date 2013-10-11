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
#ifndef ROOMBA_SENSOR_DATABASE_H
#define ROOMBA_SENSOR_DATABASE_H
#include <roomba_def.h>
/**
 * @defgroup roomba_sensor_database Roomba Sensor Database
 * 
 * Libary zu Aufbewahrung von Sensor Daten
 * 
 * Libary kann verwent werden um zu erkennen ob Daten vom Roomba neu
 * 
 * Daten werden eingelagert mit {@link roomba_sensor_database_receive()}
 * @{
 */

/**
 * Initialisiert den Speicher
 * 
 * Der intere Speiche kann alle Gültige Sensor IDs aufnehmen
 * 
 * @result Folgende Werte können zurück gegeben werden:
 * - <b>0</b> keine Fehler
 */
int8_t roomba_sensor_database_init();
/**
 * Daten im Internen Buffer einlagern
 * Überschreibt bei Aktualisierung den Internen Buffer und meldet zurück ob die Daten neu waren!
 * 
 * Ob die Daten neu sind wird über den Vergleich
 *
 *      bufferData->data == data->data && bufferData->sensorID != 0 
 * 
 * entschieden
 * 
 * Wenn data->sensorID == {@link ROOMBA_SENSOR_DISTANCE} || data->sensorID == {@link ROOMBA_SENSOR_ANGLE} wird 
 * immer angenommen das der Wert neu ist außer der wert war 0 und ist 0!
 * 
 * @return Folgende Werte können zurück gegeben werden: 
 * - <b>0</b> Daten alt nicht aktualisiert
 * - <b>1</b> Daten neu aktualisiert
 * - <b>-1</b> Übergebende Daten fehlerhaft
 */
int8_t roomba_sensor_database_receive(roombaSensorData_t* data);
/**
 * Liefert einen Bestimmte SensorID zurück
 * 
 * Wenn data->sensorID == {@link ROOMBA_SENSOR_DISTANCE} || data->sensorID == {@link ROOMBA_SENSOR_ANGLE} 
 * dann wird der Wert automatisch auf 0 gesetzt wenn es einmal abgerufen worden ist!
 *
 * @return Folgende Werte können zurück gegeben werden: 
 * - <b>0</b> Daten vorhanden
 * - <b>-1</b> Daten unbekannt
 */
int8_t roomba_sensor_database_get(uint8_t sensorID, roombaSensorData_t* result);
/*TODO Optional */
/*int8_t roomba_sensor_database_next(roombaSensorData_t* result);*/
/* @}*/ /* roomba_sensor_database */

#endif
