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
#ifndef ROOMBA_EXTRA
#define ROOMBA_EXTRA
#include <roomba_def.h>
/**
 * @defgroup roomba_extra Roomba Extra Libary
 * 
 * Defineirt eine Libary um die Interne Befehle des Roomba zu erweitern
 * @{
 */
/**
 * Roomba Extra Init
 */
int8_t roomba_extra_init();
/**
 * Überprüft ob ein Befehl ein Extra Befehl ist
 * @param command Commando das Überprüft werden soll
 * @return Folgende Rückgaben sind möglichen
 * - 0: Ist kein Extra Befehl
 * - <b>{@link ROOMBA_EXTRA_STREAM}:</b> Besondere Sensor Stream es ist Möglichkeit die Intervalldauer 
 *   einzustellen. Befehl verwendet {@link roomba_repeat Roomba Repeat Libary} 
 * - <b>{@link ROOMBA_EXTRA_STREAM_PAUSE_RESUME}:</b> Starte oder Stoppt Extra Stream
 */
uint8_t roomba_extra_check(roombaCommand_t* command);
/**
 * Führt ein Extra Kommando aus
 * @param command Kommando
 * @return < 0 Fehler > kein Fehler
 */
int8_t roomba_extra_handle_command(roombaCommand_t* command);
/**@}*/

#endif
