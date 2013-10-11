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
#ifndef ROOMBA_REPEAT
#define ROOMBA_REPEAT
#include <roomba_def.h>
/**
 * @defgroup roomba_repeat Roomba Reepeat Libary
 * 
 * Definiert eine API um eine Befehl zyklisch immer wieder auszuführen.
 * @{
 */
/**
 * Thread konnte nicht gestartet werden
 */
#define ROOMBA_REPEAT_ERROR_NOT_STARTED -2

/**
 * Initialisiert Repeat Libaray
 * 
 * @param c Sende Funktion Vergleichbar mit {@link roomba_command_init}
 * @param thread_start Funktion die einen Thread startet der alle ms die Funktion {@link roomba_repeat} aufruft.
 * @param thread_stop Funktion die den Thread Stoppt
 * @return 0 kein Fehler Aufgetreten
 */
int8_t roomba_repeat_init(int8_t (*c)(roombaCommand_t* command), int8_t (*thread_start)(uint16_t ms), int8_t (*thread_stop)());
/**
 * Funktion die immer wieder aufgerufen werden muss
 * Diese Funktion sendet das mit {@link roomba_repeat_start} definiert Kommando
 * Diese Funktion muss von dem Thread aufgerufen werden
 * 
 * @return Funktion kann folgende Werte zurückgeben:
 * - 0 kein Fehler
 * - <b>{@link ROOMBA_REPEAT_ERROR_NOT_STARTED}</b> System ist nicht gestartet
 */
int8_t roomba_repeat();
/**
 * Sendet alle ms Millisekunde den Befehl c
 *
 * Falls bereits ein Befehl gestartet worden ist wird dieser gestoppt
 *
 * @param c Befehl der gesendet werden soll
 * @param ms Zeit in Millisekunde die der Befehl wiederholte werden soll
 * @return < 0 Fehler sonst kein Fehler 
 */
int8_t roomba_repeat_start(roombaCommand_t* c, uint16_t ms);
/**
 * Stoppt Thread wieder
 * @return < 0 Fehler sonst kein Fehler 
 */
int8_t roomba_repeat_stop();
/** @}*/
#endif
