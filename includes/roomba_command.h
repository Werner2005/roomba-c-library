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
#ifndef ROOMBA_COMMAND_H
#define ROOMBA_COMMAND_H
#include <roomba_def.h>
/**
 * @defgroup roomba_command Roomba Command Libary
 * 
 * In diesem Modul wird eine API definiert um vereinfacht und Standardisiert mit dem Roomba zu kommunizieren.
 * @{
 */

/**
 * Wartezeit die gewartet wird zwischen 4 Buchstaben
 * 
 * Wird von {@link roomba_showASCII}
 */
#define ROOMBA_LED_WAITTIME 2000
/**
 * Initialisiert Libary
 * 
 * @param s sende Callback
 *          Funktion bekommt ein Kommando Übergeben das Versendet werden soll
 *          Diese Funktion soll einen Wert von < 0 zurück wenn ein Fehler aufgetreten ist
 * 
 *	    Beispiel: über sdds an Roomba oder Sensorknoten über RS232 an Roomba ...
 * @param ms Funktion wie sleep. Dieser Callback soll eine bestimmte Zeit in Milisekunden warten. Dieser Callback wird für {@link roomba_showASCII()} benötigt
 *          
 */
void roomba_command_init(int8_t (*s)(roombaCommand_t* command), int (*ms)(unsigned int));
/**
 * Sendet ein Befehl an den Roomba
 * 
 * @param commandID Kommando ID Beispiel: {@link ROOMBA_START}
 * @param size Anzahl der Daten Bytes
 * @param ... Daten
 * @return < 0 Fehler aufgetreten >= 0 kein Fehler aufgetreten
 */
int8_t roomba_send(uint8_t commandID, uint8_t size, ...);
/**
 * Sendet ROOMBA_START und ROOMBA_SAFE um die Kommunikation mit dem Roomba zu initialisieren. 
 * Zusätzlich bricht diese Befehlsreihenfolge den Aktuellen Befehl ab!
 * @return < 0 Fehler aufgetreten >= 0 kein Fehler aufgetreten
 */
int8_t roomba_start();
/**
 * Einfacher Bewege Befehl Verwendet {@link ROOMBA_DRIVE}
 * 
 * @param velocity Geschwindelt in mm/s (-500 - 500 mm/s)
 * @param radius in mm (-2000 mm - 2000 mm)
 * @return < 0 Fehler aufgetreten >= 0 kein Fehler aufgetreten
 */
int8_t roomba_drive(int16_t velocity, int16_t radius);
/**
 * Zeigt ein Bestimmten Text auf der LED Anzeige des Roomba an
 * Falls der Text > 4 Zeichen ist wird er in 4 Zeichen Sequenzen
 * gesplittet und nacheinander in {@link ROOMBA_LED_WAITTIME} ms Abständen angezeigt
 * 
 * Es ist zu beachten das der Roomba nur eine Segmentanzeige besitzt
 * und daher die Buchstaben nur angenähert werden!
 * 
 * @param string String der Angezeigt werden soll
 * @return < 0 Fehler aufgetreten >= 0 kein Fehler aufgetreten
 */
int8_t roomba_showASCII(char * string);
/**
 * Zeigt eine Zahl auf der Anzeige des Roombas in Hex an
 * @param t Zahl die Angezeigt werden soll
 * @return < 0 Fehler aufgetreten >= 0 kein Fehler aufgetreten
 * @see roomba_showASCII(char*)
 */
int8_t roomba_showHexLED(uint16_t t);
/**
 * Startet eine Sensor Stream
 *
 * @warning diese Funktion darf nicht zusammen mit der roomba.c verwendet werden!
 *
 * @param ms Perioden Dauer
 * @param size Anzahl der SensorIDs
 * @param ... SensorIDs
 */
int8_t roomba_extra_stream_start(uint16_t ms, uint8_t size, ...);
/**
 * Stoppt Sensor Stream
 * 
 * @warning diese Funktion darf nicht zusammen mit der roomba.c verwendet werden!
 */
int8_t roomba_extra_stream_stop();
/** @}*/ /* roomba_command */

#endif
