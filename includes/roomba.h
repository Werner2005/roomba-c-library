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
#ifndef ROOMBA_H
#define ROOMBA_H
/**
 * Roomba Reader / Writer Module
 */

/**
 * @defgroup reader_writer Roomba Reader/Writer
 * 
 * Dieses Modules ist für die Kommunikation mit dem Roomba verantwortlich. 
 * @{
 */

/**
 * @defgroup reader Roomba Reader
 * 
 * Lese Module
 * @{
 */


#include <roomba_def.h>

#ifdef DOXYGEN
/**
 * Wenn dieses Define definiert ist werden alle Blockiere Methoden nicht blockieren
 * und immer {@link ROOMBA_ERROR_AGAIN} zurückgeben wenn keine Daten verfügbar sind.
 *
 * Define muss über den Compiler gesetzt werden!(-DROOMBA_NONBLOCKING)
 */
#define ROOMBA_NONBLOCKING
#endif
/**
 * Stadien für lesen
 */
enum roombaStatus { 
	/**
	 * Idle
	 */
	idle, 
	/**
	 * Singeldata
	 */
	singeldata, 
	/**
	 * Gruppe
	 */
	group,
	/**
	 * Query List
	 */
	query,
	/**
	 * stream
	 */
	stream
};
typedef enum roombaStatus roombaStatus_t;
#ifdef DOXYGEN
/**
 * Wenn diese Define gesetzt ist. Wird ein zweiter Ringbuffer vorgeschaltete. 
 * Ziel ist die Zeit die Read benötigt zu verringern. 
 * 
 * Die Daten werden dann nicht mehr von {@link roomba_read()} verarbeitet sondern
 * beim Abrufen der Daten({@link roomba_hasSensorData()} und {@link roomba_getSensorData})
 * 
 * Define muss über den Compiler gesetzt werden!(-DROOMBA_SHORT_READ)
 */
#define ROOMBA_SHORT_READ
#endif
/** @}*/ /*End Group reader*/
/**
 * @defgroup writer Roomba Writer
 * 
 * Dieses Module ist für das schreiben eines Kommandos an den Roomba verantwortlich
 */

/**
 * Roomba Init
 * @param w Callback um ein Zeichen zu schreiben
 */
void roomba_init(int (*w)(unsigned char c));
/**
 * Verarbeite ein Zeichen
 *
 * @ingroup reader
 * @param c Zeichen das Verarbeitet werden soll
 * @return Kann verschiedene Werte zurückgeben
 * - <b>0</b> Kein Fehler
 * - <b>-{@link ROOMBA_ERROR_BUFFER_FULL}(-{@link ROOMBA_ERROR_BUFFER_FULL_CHAR}, -{@link ROOMBA_ERROR_BUFFER_FULL_STREAM})</b> Buffer voll 
 * - <b>-{@link ROOMBA_ERROR_NO_SIGN_EXPECTED}</b> Kein Zeichen erwartet
 * Beim verarbeiten eines Stream können folgende (Fehler auftreten Fehler kännen nur auftreten wenn {@link ROOMBA_SHORT_READ} nicht gesetzt ist!)
 * - <b>-{@link ROOMBA_ERROR_HEADER_NOT_FOUND}</b> Zeichen war nicht das erwarte Header Zeichen(19)
 * - <b>-{@link ROOMBA_ERROR_SENSORID_NOT_FOUND}</b> Senesor ID nicht korrekt
 * - <b>-{@link ROOMBA_ERROR_CHECKSUM_NOT_VALID}</b> Checksumme nicht korrekt
 */
int8_t roomba_read(uint8_t c);
/**
 * Gibt ein Sensor Daten zurück
 * Wenn keine Daten verfügbar sind blockiert diese Methode bis Daten 
 * da sind(Ausser {@link ROOMBA_NONBLOCKING} ist gesetzt!)
 * 
 * @ingroup reader
 * @param data Rommba Sensor Daten
 * @return Folge Werne können zurück geben werden: 
 * - <b>0</b> kein Fehler 
 * - <b>-{@link ROOMBA_ERROR_AGAIN}</b> Keine SensorDaten vorhanden! Wird nur zurück geben wenn {@link ROOMBA_NONBLOCKING} gesetzt ist!
 */
int8_t roomba_getSensorData(roombaSensorData_t* data);
/**
 * Resetet Buffer
 * @ingroup reader
 */
void roomba_reset();
/**
 * Sendet ein Kommando
 * 
 * Wenn ein {@link ROOMBA_SENSORS} Befehl gesendet wird und derzeit noch ein Sensor Befehl in Arbeit ist.(Außer
 * wenn {@link ROOMBA_NONBLOCKING} gesetzt ist!).
 * Dieser Befehl bereitet das System auf den Empfang der Daten vor!
 * 
 * Bei den Befehlen {@link ROOMBA_QUERY_LIST} und {@link ROOMBA_STREAM} werden alle Gruppen Sensor Daten automatisch gegen
 * ihre echte Werte getauscht! Beispiel: 
 * {@link ROOMBA_SENSOR_GROUP_1} gegen: {@link ROOMBA_SENSOR_BUMPS_WHEEL_DROPS}, {@link ROOMBA_SENSOR_WALL}, ...
 * 
 * Bei dem Befehl {@link ROOMBA_STREAM_PAUSE_RESUME} wird bei den Befehl 0(Stream Abruf) Automatische das Interne System 
 * Mithilfe von {@link roomba_reset} gestopt! Bei 1 wird der Empfang von Stream Daten wieder vorbereitet.
 * 
 * @warning Es darf vor dem Befehl {@link ROOMBA_STREAM_PAUSE_RESUME}(Data 1) kein andere Sensor Befehl wie
 * ({@link ROOMBA_SENSORS} oder {@link ROOMBA_QUERY_LIST} gesetzt worden sein!
 * 
 * @ingroup writer
 * @param c Kommando das gesendet werden soll
 * 
 * @return Mögliche Rückgabe Werte sind: 
 * 
 * - <b>0</b> kein Fehler
 * - -{@link ROOMBA_ERROR_TO_BIG} Angebe Gruppen überschreiten das Maximum von {@link ROOMBA_MAX_DATA} Sensor Daten
 * - <b>-{@link ROOMBA_ERROR_AGAIN}</b> Keine SensorDaten vorhanden! Wird nur zurück geben wenn {@link ROOMBA_NONBLOCKING} gesetzt ist!
 */
int8_t roomba_sendCommand(roombaCommand_t* c);
/**
 * Prüft ob SensorDaten vorliegen
 * @ingroup reader
 * @return true: SensorDaten verfügbar false: Keine SensorDaten verfügbar
 */
uint8_t roomba_hasSensorData();

/**
 * Gibt den ersten Fehler zurück
 * Falls kein Fehler aufgetreten ist wird {@link ROOMBA_ERROR_NO} zurückgeben
 * Der Fehler wird nach dem Lesen auf {@link ROOMBA_ERROR_NO} zurückgesetzt.
 * @return gibt die Aktuelle Fehlernummer zurück oder {@link ROOMBA_ERROR_NO}
 */
int8_t roomba_getError();

/**
 * Gibt Aktuellen Sensor Paser Status zurück
 * @ingroup reader
 * @return Status
 */
roombaStatus_t roomba_getStatus();
/**
 * Bricht Sensor Pasing ab
 * 
 * Es kann vorkommen das nicht alle Pakete eines angeforderten Befehles ankommen
 * daher ist es wichtig das auch ein Sensor Befehl abgebrochen werden kann
 *
 * Falls aktuell kein Sensor Befehl aktiv ist hat dieser Befehl keine Auswirkung!
 * 
 * Diese Funktion setzt den Fehler {@link ROOMBA_ERROR_ABORT_SENSOR}
 *
 * Falls ein Sensor Stream aktiv ist wird dieser mit dem Befehl {@link ROOMBA_STREAM_PAUSE_RESUME} beendet
 * @ingroup reader
 */
void roomba_abort_sensor();
/**
 * Roomba Senosr Buffer Größe
 * @ingroup reader
 */
#ifndef ROOMBA_SENSOR_BUFFER_SIZE
#define ROOMBA_SENSOR_BUFFER_SIZE 173
#endif

/**
 * Anzahl von Stream Paketen die Ignoriert werden sollen falls der Interne Buffer voll ist
 * @ingroup reader
 */
#ifndef ROOMBA_STREAM_IGNORE
#define ROOMBA_STREAM_IGNORE 50
#endif

/** @}*/ /*End Gropu Reader/Writer*/
#endif
