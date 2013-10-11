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
#ifndef ROOMBA_DEF
#define ROOMBA_DEF
#include <stdint.h>
/**
 * Alle globalen Definitionen für die Roomba Libary
 */
/**
 * @defgroup roomba_def Globale Roomba Definitionen
 * 
 * Das sind alle Globalen Roomba Definitionen
 * @{
 */
#ifndef ROOMBA_MAX_DATA
/**
 * Maximale Anzahl von Daten die ein Kommando enthalten kann
 */
#define ROOMBA_MAX_DATA 66
#endif

/**
* Repräsentiert ein Roomba Befehl
*/
struct roombaCommand{
	/**
	 * Befehl
	 */
	uint8_t command;
	/**
	 * Größe
	 */
	uint8_t size;
	/**
	 * Daten die Übermittelt werden sollen
	 * Maximal {@link ROOMBA_MAX_DATA} Bytes
	 */
	uint8_t data[ROOMBA_MAX_DATA];
};

typedef struct roombaCommand roombaCommand_t;

/** 
 * Repräsentiert ein Roomba Sensor Daten
 */
struct roombaSensorData{
	/**
	 * Sensor ID
	 */
	uint8_t sensorID;
	/*
	 * Size in Bytes
	 */
	uint8_t size;
	/**
	 * Data
	 */
	int16_t data;
};

typedef struct roombaSensorData roombaSensorData_t;
/**
 * @defgroup roomba_error Roomba Error
 * Alle Fehler die auftreten können
 * 
 * Fehler werden über Roomba Contol ID {@link ROOMBA_CONTOLID_ERROR} vom Sensorknoten übermittelt
 * @{
 */
/**
 * Kein Fehler
 */
#define ROOMBA_ERROR_NO 0x00
/**
 * Buffer Voll
 */
#define ROOMBA_ERROR_BUFFER_FULL 0x01
/**
 * Kein Zeichen erwartet
 */
#define ROOMBA_ERROR_NO_SIGN_EXPECTED 0x02
/**
 * Write Error
 */
#define ROOMBA_ERROR_WRITE 0x03
/**
 * Anfrage ist nach dem expandieren der Gruppen zu Gross
 */
#define ROOMBA_ERROR_TO_BIG 0x04
/**
 * @defgroup roomba_error_stream Stream Fehler
 * @{
 */
/**
 * Header Nicht gefunden
 */
#define ROOMBA_ERROR_HEADER_NOT_FOUND 0x05
/**
 * SensorID nicht korrekt
 */
#define ROOMBA_ERROR_SENSORID_NOT_FOUND 0x06
/**
 * Checksum nicht gueltig
 */
#define ROOMBA_ERROR_CHECKSUM_NOT_VALID 0x07
/** @}*/ /* roomba_error_stream */
/**
 * Wird zurückgeben wenn keine Daten vorliegen
 * oder der Send Befehl nicht gesendet werden kann 
 * weil noch Daten erwartet werden
 * 
 * error Variable wird nicht gesetzt!
 * 
 * Wird nur zurückgeben wenn ROOMBA_NONBLOCKING gesetzt ist!
 */
#define ROOMBA_ERROR_AGAIN 0x08
/**
 * Buffer Voll beim lesen eines Streames
 * Tritt nur auf wenn nach der Ignore Phase der Buffer immer noch Voll ist
 * @see ROOMBA_STREAM_IGNORE
 */
#define ROOMBA_ERROR_BUFFER_FULL_STREAM 0x09
/**
 * Char Ringbuffer voll gelaufen
 * 
 * Kann nur eintreten wenn {@link ROOMBA_SHORT_READ} gesetzt ist 
 */
#define ROOMBA_ERROR_BUFFER_FULL_CHAR 0x0a

/**
 * Kalibrierung auf Roomba Fehlgeschlagen
 *
 * Verbindung zum Roomba gestört!
 */
#define ROOMBA_ERROR_CALIBRATION_FAULT 0x0b

/** 
 * Sensor Befehl abgebrochen
 */
#define ROOMBA_ERROR_ABORT_SENSOR 0x0c
/** @}*/ /* roomba_error */

/**
 * @defgroup roomba_commands Roomba Kommandos
 * 
 * Dies sind alle Rooba Kommandos 
 * @{
 */

/**
 * @defgroup roomba_commands_start Start Kommandos
 * @{
 */
/** 
 * Setzt den Roomba von dem Off Modus in den Passive Modus 
 */
#define ROOMBA_START 128 
/** 
 * Setzt eine neue Baud Rate. Dieser Befehl erwartet einen weiteren Byte der die Baud Rate bestimmt. Es muss 100 ms gewarnt werden bevor der Roomba mit der neuen Bautrate angesprochen werden kann.  
 * 
 * Nicht Funktionsfähig auf dem Roomba 780
 */
#define ROOMBA_BAUD  129 
/** @}*/ /* roomba_commands_start */
/**
 * @defgroup roomba_command_mode Mode Kommandos 
 * @{
 */
/**
 *  Wechselt in den Safe Modus 
 */
#define ROOMBA_SAFE 131 
/** 
 * Wechselt in den Full Modus 
 */
#define ROOMBA_FULL 132 
/* @}*/ /* roomba_command_mode */
/**
 * @defgroup roomba_commands_clean Putzt Kommandos 
 * @{
 */
/** 
 * Startet normalen Säuberungsmodus 
 */
#define ROOMBA_CLEAN         135 
/** 
 * Startet den Max Säuberungsmodus 
 */
#define ROOMBA_MAX           136 
/** 
 * Startet den Spot Säuberungsmodus 
 */
#define ROOMBA_SPOT          134 
/** 
 * Suche und Docke an Dockingstationn an 
 */
#define ROOMBA_SEEK_DOCK     143 
/** 
 * Setzte oder löscht geplante Reinigungszyklus. Zum deaktivieren eines geplante Reinigungszyklus müssen alle werte auf 0 gesetzt werden. Dieser Befehl erfordert folgende Bytes: [Days] [Sun Hour] [Sun Minute] [Mon Hour] [Mon Minute] [Tue Hour] [Tue Minute] [Wed Hour] [Wed Minute] [Thu Hour] [Thu Minute] [Fri Hour] [Fri Minute] [Sat Hour] [Sat Minute] 
 */
#define ROOMBA_SCHEDULE      167 
/** 
 * Setzt die den Tag und die Uhrzeit des Roomba [Day] [Hour] [Minute] Day=0 ist Sonntag 
 */
#define ROOMBA_SET_TIME_DATE 168 
/** 
 * Ausschalten des Roomba  
 */
#define ROOMBA_POWER         133 
/**@}*/ /* roomba_commands_clean */
/**
 * @defgroup roomba_commands_motor Motor Steuerungskommandos
 * @{
 */
/** 
 * Startet Bewegung Syntax Syntax: [Velocity high byte] [Velocity low byte] [Radius high byte] [Radius low byte] Velocity (-500 - 500 mm/s) Radius (-2000 - 2000 mm)  
 */
#define ROOMBA_DRIVE            137 
/** 
 * Motoren Einzel Steuern Syntax: [Right velocity high byte] [Right velocity low byte] [Left velocity high byte] [Left velocity low byte] 
 * 
 * Auf dem Roomba 780 dm/s nicht mm/s
 */
#define ROOMBA_DRIVE_DIRECT     145 
/** 
 * Steuert die Motoren über PWM direkt an Syntax:[Right PWM high byte] [Right PWM low byte] [Left PWM high byte] [Left PWM low byte] 
 */
#define ROOMBA_DRIVE_PWM        146 
/** 
 * Aktiviert die Motoren für die Reinigung(Sauger hier inbegriffen). Dieser Befehl aktiviert den Motor mit voller Leistung. Welcher Motor hierbei eingeschaltet wird oder welche Richtung wird über eine 8 Bit Bitmaske gesetzt. Syntax: Reserved&Reserved&Reserved&Main Brush Direction&Side Bruch Clockwiese&Main Brush&Vacuum&Side Brush 
 */
#define ROOMBA_MOTORS           138 
/** 
 * Direkte PWM Steuerung der Reinigungsmotoren.Syntax: [144] [Main Brush PWM] [Side Brush PWM] [Vacuum PWM] 
 */
#define ROOMBA_MOTORS_PWM       144 
/** 
 * Steuert alle LEDs des Roombars. Welche der LEDs hierbei leuchten sollen wird ebenfalls über eine 8-Bit Maske festgelegte. Syntax: Reserved&Reserved&Reserved&Reserved&Check Robot&Dock&Spot&Debris. Danach folgt Die Fabe der Power LED und die helligkeit. Syntax: [139] [LED Bits] [Clean/Power Color] [Clean/Power Intensity] 
 */
#define ROOMBA_LEDS             139 
/** 
 * Streut die oberen LEDs. Diese sind für die Anzeige des Zeitplanes zuständig. Hierbei kann über eine 8-Bit Maske die Wochentags LEDs gesteuert werden Syntax: Reserved&Sat&Fri&Thu&Wed&Tue&Mon&Sun. Danach könenn noch 5 weiter LEDs über eine 8-Bit Maske gesteuert werden: Reserved&Reserved&Reserved&Schedule&Clock&AM&PM&Colon(:) Syntax: [162] [Weekday LED Bits][Scheduling LED Bits] 
 */
#define ROOMBA_LEDS_SCHEDULING  162 
/** 
 * Hier kann die Siebensegment anzeige des Roombars gesteuert werden. Es gibt hierbei 4 Siebensegment Anzeigen die über eine 8-Bit Maske gesteuert wird Syntax: Reserved&GF&E&D&CB&A Syntax: [163] [Digit 3 Bits] [Digit 2 Bits] [Digit 1 Bits] [Digit 0 Bits] 
 */
#define ROOMBA_LEDS_DIGIT_RAW   163 
/** 
 * Hier kann die Siebensegment Anzeige gesteuert werden. Hierbei werden die einzelne Siebensegment Anzeigen mit hilf von ASCII Zeichen gesteuert. Diese müssen nicht unbedingt korrekt angezeigt werden. Die Zeichen von ASCII Zeichen 32 bis 126 werden von Roomba verstanden und angezeigt(Alle Großbuchstaben + Sonderzeichen) Syntax: [164] [Digit 3 ASCII] [Digit 2 ASCII] [Digit 1 ASCII] [Digit 0 ASCII] 
 */
#define ROOMBA_LEDS_DIGIT_ASCII 164 
/** 
 * Mit diesem Befehl können die Knöpfe des Roombas gedrückt werden. Hiebei wird ebenfalls eine 8-Bit Maske verwendet. Syntax: Clock&Schedule&Day&Hour&Minute&Dock&Spot&Clean Syntax: [165] [Buttons] 
 */
#define ROOMBA_BUTTONS          165 
/** 
 * 140 Dieser Befehl ermöglicht es bis zu 4 Musikstücke auf dem Roomba zu Speichern. Jedes Musikstück kann bis zu 16 Noten enthalten. Die Noten werden hierbei im MIDI Standart definiert. Der Roomba unterstützt die Noten 31-127. Zusätzlich muss zu einer Note noch die Abspielzeit der Note übertragen werden. Diese muss in einer Zahl von 1-255 übertragen werden. 1 entspricht hierbei 1/64 Sekunden. [140] [Song Number] [Song Length] [Note Number 1] [Note Duration 1] [Note Number 2] [Note Duration 2], etc. 
 */
#define ROOMBA_SONG             140 
/** 
 * Spielt einen gespeicherten Song ab Syntax: [141] [Song Number] 
 */
#define ROOMBA_PLAY             141 

/** @}*/ /* roomba_commands_motor */

/**
 * @defgroup roomba_commands_script Script Kommmandos 
 * 
 * Auf dem Roomba 780 nicht verfügbar
 * @{
 */
/** 
 * Definiert ein Script. Die Länge kann hier 0 - 100 sein. 0 ist um das Script zu löschen. Syntax: [152] [Script Length] [Opcode 1] [Opcode 2] [Opcode 3] etc. 
 */
#define ROOMBA_SCRIPT        152 
/** 
 * Startet gespeichertes Script  
 */
#define ROOMBA_SCRPTT_PLAY   153 
/** 
 * Zeigt das aktuelle Script an 
 */
#define ROOMBA_SCRIPT_SHOW   154 
/** 
 * Wartet eine Bestimmte Zeit. Eine Einheit entspricht 15 ms Der Wert kann zwischen 0-255 sein. Während der Wartezeit kann der Roomba keine Befehle empfangenen! Syntax [155] [time] 
 */
#define ROOMBA_WAIT_TIME     155 
/** 
 * Wartet bis eine bestimmte Entfernung zurückgelegt worden ist. Als Eingabe Parameter wird eine 16 Bit Signed Integer Variable in Millimeter erwartet. Beim Vorwärts Fahren wird die Entfernungsvariable hochgezählt und beim Rückwärts Fahren zurück gezählt. Syntax: [156] [Distance high byte] [Distance low byte] 
 */
#define ROOMBA_WAIT_DISTANCE 156 
/** 
 * Wartet bis der Roomba eine bestimmte Winkel zurückgelegt hat. Der Winkel wird hierbei im Uhrzeigersinn hochgezählt und gegen den Uhrzeigersinn zurück gezählt. Als Eingabe Parameter wird eine 16 Bit Signed Variable in Grad erwartet. Syntax: [157] [Angle high byte] [Angle low byte] 
 */
#define ROOMBA_WAIT_ANGLE    157 
/**
 * Wartet bis ein Bestimmtes Ereignis eintritt. Die Ereignisse sind sind unten in einer Seeraten Tabelle aufgeführt. Um auf ein gegenteiliges Event zu warten. Zum Beispiel das der Roomba abgelegt wird Müssen negative Zahlen übertragen werden. Beispiel: [158][-5] oder [158] [251] Syntax: [158] [Event number] 
 */
#define ROOMBA_WAIT_EVENT    158 

/**
 * @defgroup roomba_commands_script_events Script Events
 * 
 * Events für {@link ROOMBA_WAIT_EVENT}
 * @{
 */
#define ROOMBA_EVNET_WHEEL_DROP        1
#define ROOMBA_EVNET_WHEEL_DROP_FRONT  2
#define ROOMBA_EVNET_WHEEL_DROP_LEFT   3
#define ROOMBA_EVNET_WHEEL_DROP_RIGHT  4
#define ROOMBA_EVNET_BUMP              5
#define ROOMBA_EVNET_BUMP_LEFT         6
#define ROOMBA_EVNET_BUMP_RIGHT        7
#define ROOMBA_EVNET_VIRTUAL_WALL      8 
#define ROOMBA_EVNET_WALL              9
#define ROOMBA_EVNET_CLIFF             10
#define ROOMBA_EVNET_CLIFF_LEFT        11
#define ROOMBA_EVNET_CLIFF_FRONT_LEFT  12
#define ROOMBA_EVNET_CLIFF_FRONT_RIGHT 13
#define ROOMBA_EVNET_CLIFF_RIGHT       14
#define ROOMBA_EVNET_HOME_BASE         15 
#define ROOMBA_EVNET_ADVANCE_BUTTON    16 
#define ROOMBA_EVNET_PLAY_BUTTON       17 
#define ROOMBA_EVNET_DIGITAL_INPUT_0   18
#define ROOMBA_EVNET_DIGITAL_INPUT_1   19
#define ROOMBA_EVNET_DIGITAL_INPUT_2   20
#define ROOMBA_EVNET_DIGITAL_INPUT_3   21
#define ROOMBA_EVNET_OI_MODE_PASSIVE   22
/** @}*/ /* roomba_commands_script_events*/
/** @}*/ /* roomba_commands_script */
/**
 * @defgroup roomba_commands_sensor Sensor Kommandos
 * @{
 */
/** 
 * Ruft einen Bestimmten Sensor ab. Ein Wert von 100 liefert alle Sensor Daten. 
 */
#define ROOMBA_SENSORS             142 
/** 
 * Lifert eine Liste von Sensor Daten Syntax: [149][Number of Packets][Packet ID 1][Packet ID 2]...[Packet ID N] 
 */
#define ROOMBA_QUERY_LIST          149 
/** 
 * Liefert eine Liste von Sensor Daten die Alle 15ms wiederholt wird. Die Checksum ist die Summe alle Bytes nach dem Header. Das Low-Byte der Summe plus die Checksum muss 0 sein((checksum+summe)%0xFF == 0). Wenn mehr Daten angefordert werden als in 15ms übertragen werden können. Ist der Datenstrom beschädigt! Maximum wird berechnet: 15 ms / 10 bits (8 data + start + stop) * Bautrate Syntax: [148] [Number of packets] [Packet ID 1] [Packet ID 2] [Packet ID 3] etc. Syntax der Antwort: [19][N-bytes][Packet ID 1][Packet 1 data...][Packet ID 2][Packet 2 data...][Checksum] 
 */
#define ROOMBA_STREAM              148 
/** 
 * Pausiert oder Startet einen Passierten Steam Syntax: [150][Stream State 0-1] 
 */
#define ROOMBA_STREAM_PAUSE_RESUME 150 
/** @}*/ /* roomba_commands_sensor*/
/**
 * @defgroup roomba_commands_extra Roomba Extra Befehle
 * @{
 */
/**
 * Erweiterer Stream Befehl
 * 
 * Feld genauso wie bei {@link ROOMBA_STREAM} nur data[0] = high Byte Wait Time(uint16_t) data[1] = low Byte Wait Time(uint16_t)
 * 
 * data[2] = Anzahl der SensorIDs
 * data[3-ROOMBA_MAX_DATA] = SensorIDs
 */
#define ROOMBA_EXTRA_STREAM 200
/**
 * Startet und Stop Extra Stream 
 * @see ROOMBA_STREAM_PAUSE_RESUME
 */
#define ROOMBA_EXTRA_STREAM_PAUSE_RESUME 201
/** @}*/ /* roomba_commands_extra */
/** @}*/ /* roomba_commands */
/**
 * @defgroup roomba_sensor Sensor Daten
 * @{
 */

/**
 * @defgroup roomba_sensor_group Sensor Gruppen Daten
 * @{
 */
/** Sensor ID 7 - 26  */
#define ROOMBA_SENSOR_GROUP_0 0 
#define ROOMBA_SENSOR_GROUP_0_SIZE 26
#define ROOMBA_SENSOR_GROUP_0_START 7
#define ROOMBA_SENSOR_GROUP_0_STOP 26
/** Sensor ID  7 - 16  */
#define ROOMBA_SENSOR_GROUP_1 1 
#define ROOMBA_SENSOR_GROUP_1_SIZE 10
#define ROOMBA_SENSOR_GROUP_1_START 7
#define ROOMBA_SENSOR_GROUP_1_STOP 16
/** Sensor ID 17 - 20 */
#define ROOMBA_SENSOR_GROUP_2 2 
#define ROOMBA_SENSOR_GROUP_2_SIZE 6
#define ROOMBA_SENSOR_GROUP_2_START 17
#define ROOMBA_SENSOR_GROUP_2_STOP 20
/** Sensor ID 21 - 26 */
#define ROOMBA_SENSOR_GROUP_3 3 
#define ROOMBA_SENSOR_GROUP_3_SIZE 10
#define ROOMBA_SENSOR_GROUP_3_START 21
#define ROOMBA_SENSOR_GROUP_3_STOP 26
/** Sensor ID 27 - 34 */
#define ROOMBA_SENSOR_GROUP_4 4 
#define ROOMBA_SENSOR_GROUP_4_SIZE 14
#define ROOMBA_SENSOR_GROUP_4_START 27
#define ROOMBA_SENSOR_GROUP_4_STOP 34
/** Sensor ID 35 - 42 */
#define ROOMBA_SENSOR_GROUP_5 5 
#define ROOMBA_SENSOR_GROUP_5_SIZE 12
#define ROOMBA_SENSOR_GROUP_5_START 35
#define ROOMBA_SENSOR_GROUP_5_STOP 42
/** Sensor ID 7 - 42 */
#define ROOMBA_SENSOR_GROUP_6 6 
#define ROOMBA_SENSOR_GROUP_6_SIZE 52
#define ROOMBA_SENSOR_GROUP_6_START 7
#define ROOMBA_SENSOR_GROUP_6_STOP 42
/** Sensor ID 7-58 */
#define ROOMBA_SENSOR_GROUP_100 100 
#define ROOMBA_SENSOR_GROUP_100_SIZE 93
#define ROOMBA_SENSOR_GROUP_100_START 7
#define ROOMBA_SENSOR_GROUP_100_STOP 71
/** Sensor ID 43 - 58 */
#define ROOMBA_SENSOR_GROUP_101 101 
#define ROOMBA_SENSOR_GROUP_101_SIZE 28
#define ROOMBA_SENSOR_GROUP_101_START 43
#define ROOMBA_SENSOR_GROUP_101_STOP 58
/** Sensor ID 43 - 51 */
#define ROOMBA_SENSOR_GROUP_106 106
#define ROOMBA_SENSOR_GROUP_106_SIZE 12
#define ROOMBA_SENSOR_GROUP_106_START 46
#define ROOMBA_SENSOR_GROUP_106_STOP 51
/** Sensor ID 54 - 58 */
#define ROOMBA_SENSOR_GROUP_107 107 
#define ROOMBA_SENSOR_GROUP_107_SIZE 9
#define ROOMBA_SENSOR_GROUP_107_START 54
#define ROOMBA_SENSOR_GROUP_107_STOP 58
/**@}*/ /* roomba_sensor_group */
/**
 * @defgroup roomba_sensor_extern Externe Sensor Daten
 * @{
 */
/** 
 * Gibt Information darüber ob der Roomba gegen ein Gegenstand gefahren ist oder einer Seiner Räder sich nicht am Boden befindet. Die Information erhält eine 8 Bit Maske mit folgendem Inhalt: Reserved&Reserved&Reserved&Wheeldrop Center&Wheeldrop Left&Wheeldrop Rigth&Bump Left&Bump Rigth Syntax: [7] [8-Bit-Bitmask] 
 */
#define ROOMBA_SENSOR_BUMPS_WHEEL_DROPS         7 
/** 
 * Zeigt an ob eine Wand zu sehen(1) ist oder nicht(0) Es wird hierbei ein 1 - Bit Wert übertragen(0=keine wand, 1=Wand gesehen) 
 */
#define ROOMBA_SENSOR_WALL                      8 
/**
 * Zeigt an ob der Roomba an der Linken Seite eine Klippe entdeckt hat. 
 */
#define ROOMBA_SENSOR_CLIFF_LEFT                9 
/** 
 * Zeigt an ob der Roomba an der Vorderen Linken Seite eine Klippe entdeckt hat. 
 */
#define ROOMBA_SENSOR_CLIFF_FRONT_LEFT          10 
/** 
 * Zeigt an ob der Roomba an der Vorderen Rechten Seite eine Klippe entdeckt hat. 
 */
#define ROOMBA_SENSOR_CLIFF_FRONT_RIGHT         11 
/** 
 * Zeigt an ob der Roomba an der Rechten Seite eine Klippe entdeckt hat. 
 */
#define ROOMBA_SENSOR_CLIFF_RIGHT               12 
/** 
 * Zeigt an ob der Roomba eine Virtual Wall eindeckt hat  
 */
#define ROOMBA_SENSOR_VIRTUAL_WALL              13 
/** 
 * Zeit an ob einer Der Motoren zu viel Strom zieht. Diese Daten werden beim Roomba 500 nicht verwendet. Es muss geprüft werden ob der Roomba 700 solche Sensoren hat. 
 */
#define ROOMBA_SENSOR_WHEEL_OVERCURRENTS        14 
/** 
 * Zeigt den Verschmutzheitsgrad an. Es handelt sich um ein 8 Bit Wert von 0 - 255 
 */
#define ROOMBA_SENSOR_DIRT_DETECT               15 
/** 
 * Ungenutzte Byte. Dieses Nachricht wird bei den Gruppen Befehlen 0, 1 und 6 gesendet 
 */
#define ROOMBA_SENSOR_UNUSED_1                    16 
/** 
 * Ein Zeichen das über den IR Empfänger des Roomba empfangenen worden ist. 0 Bedeutet das kein Zeichen empfangenen worden ist. Die Werte können von 0-255 sein. 
 */
#define ROOMBA_SENSOR_INFRARED_CHAR_OMMI        17 
/** 
 * Ein Zeichen das über den Linken IR Empfänger des Roomba empfangenen worden ist. 0 Bedeutet das kein Zeichen empfangenen worden ist. Die Werte können von 0-255 sein. 
 */
#define ROOMBA_SENSOR_INFRARED_CHAR_LEFT        52 
/** 
 * Ein Zeichen das über den Rechten IR Empfänger des Roomba empfangenen worden ist. 0 Bedeutet das kein Zeichen empfangenen worden ist. Die Werte können von 0-255 sein. 
 */
#define ROOMBA_SENSOR_INFRARED_CHAR_RIGHT       53 
/** 
 * Gibt Information über die Aktuell gedrückt Tasten des Roomba's an. Übertragen wird eine 8-Bit-Maske Syntax: Clock&Schedule&Day&Hour&Minute&Dock&Spot&Clean 
 */
#define ROOMBA_SENSOR_BUTTONS                   18 
/** 
 * Die Entfernung in Millimeter die der Rommmba zurückgelegt hat seit dem die letzte Anfrage getätigt worden ist. Die Entfernung wird berechnet in dem der Weg der das Linke Rad und das Rechte Rad zurückgelegt hat durch zwei dividiert. Übertragen wird ein 16 Bit Wert von -32768 - 32767 Syntax: [19] [High- Byte] [Low- Byte] 
 */
#define ROOMBA_SENSOR_DISTANCE                  19 
/** 
 * Der Winkel in Grad die der Roomba zurückgelegt hat seit dem die letzte Anfrage getätigt worden ist. Positive Zahlen bedeutet das der Roomba sich im Uhrzeigersinn gedreht an und negative Zahlen das der Roomba sich gegen den Uhrzeigersinn gedreht hat. 
 */
#define ROOMBA_SENSOR_ANGLE                     20 
/** 
 * Stärke des Wand Signales als 16- Bit Wert. Gibt einen Wert von 0-1023 zurück. Syntax [27] [High- Byte] [Low- Byte] 
 */
#define ROOMBA_SENSOR_WALL_SIGNAL               27 
/** 
 * Stärke des Linken Klippen Signal. Gibt einen 16- Bit Wert zurück von 0-4095 Syntax: [28] [High- Byte] [Low- Byte] 
 */
#define ROOMBA_SENSOR_CLIFF_SIGNAL_LEFT         28 
/** 
 * Stärke des Vorderen Linken Klippen Signal. Gibt einen 16- Bit Wert zurück von 0-4095 Syntax: [28] [High- Byte] [Low- Byte] 
 */
#define ROOMBA_SENSOR_CLIFF_SIGNAL_FRONT_LEFT   29 
/** 
 * Stärke des Vorderen Rechten Klippen Signal. Gibt einen 16- Bit Wert zurück von 0-4095 Syntax: [28] [High- Byte] [Low- Byte] 
 */
#define ROOMBA_SENSOR_CLIFF_SIGNAL_FRONT_RIGHT  30 
/** 
 * Stärke des Rechten Klippen Signal. Gibt einen 16- Bit Wert zurück von 0-4095 Syntax: [28] [High- Byte] [Low- Byte] 
 */
#define ROOMBA_SENSOR_CLIFF_SIGNAL_RIGHT        31 
/** 
 * Unbenutzt */
#define ROOMBA_SENSOR_UNUSED_2                  32 
/** 
 * Unbenutzt */
#define ROOMBA_SENSOR_UNUSED_3                  33 
/** 
 * Gibt den Aktuellen Geschwindigkeitwert in Millimeter pro Sekunde zurück. Der Wert kann zwischen -500 - 500 mm/s liegen 
 */
#define ROOMBA_SENSOR_REQUESTED_VELOCITY        39 
/** 
 * Gibt den Radius an der mit Hilfe des Drive Befehls gesetzt worden ist zurück. 
 */
#define ROOMBA_SENSOR_REQUESTED_RADIUS          40 
/** 
 * Gibt den Aktellen Geschwindigkeitswert des Rechten Rades zurück. Der Werkt kann zwischen -500 - 500 mm/s liegen 
 */
#define ROOMBA_SENSOR_REQUESTED_VELOCITY_RIGHT  41 
/** 
 * Gibt den Aktellen Geschwindigkeitswert des Linken Rades zurück. Der Werkt kann zwischen -500 - 500 mm/s liegen 
 */
#define ROOMBA_SENSOR_REQUESTED_VELOCITY_LEFT   42 
/** 
 * RAW Daten des Rechten Ecncoders. Liefert einen 16 Bit Wert von 0 - 65535. Syntax [43] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_ENCODER_COUNTS_RIGHT      43 
/** 
 * RAW Daten des Linken Ecncoders. Liefert einen 16 Bit Wert von 0 - 65535. Syntax [43] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_ENCODER_COUNTS_LEFT       44 
/** 
 * Gibt eine 8 - Bit - Maske zurück der IR Näherungssensoren zurück. Syntax: Reserved&Reserved&Lt Bumper Rigth&Lt Bumper Front Rigth&Lt Bumper Center Right&Lt Bumper Center Left&Lt Bumper Front Left&Lt Bumper Left 
 */
#define ROOMBA_SENSOR_LIGHT_BUMPER              45 
/**
 * Stärke des Linken IR Näherungssensoren. Liefert ein 16 Bit Wert von 0 - 4095 zurück. Syntax [46] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_LIGHT_BUMPER_LEFT         46 
/** 
 * Stärke des Vorderen Linken IR Näherungssensoren. Liefert ein 16 Bit Wert von 0 - 4095 zurück. Syntax [47] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_LIGHT_BUMPER_FRONT_LEFT   47 
/** 
 * Stärke des Zentalen Linken IR Näherungssensoren. Liefert ein 16 Bit Wert von 0 - 4095 zurück. Syntax [48] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_LIGHT_BUMPER_CENTER_LEFT  48 
/** 
 * Stärke des Zentalen Rechten IR Näherungssensoren. Liefert ein 16 Bit Wert von 0 - 4095 zurück. Syntax [49] [High Byte] [Low Byte]  
 */
#define ROOMBA_SENSOR_LIGHT_BUMPER_CENTER_RIGHT 49 
/** 
 * Stärke des Vorderen Rechten IR Näherungssensoren. Liefert ein 16 Bit Wert von 0 - 4095 zurück. Syntax [50] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_LIGHT_BUMPER_FRONT_RIGHT  50 
/** 
 * Stärke des Vorderen Rechten IR Näherungssensoren. Liefert ein 16 Bit Wert von 0 - 4095 zurück. Syntax [51] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_LIGHT_BUMPER_RIGHT        51 
/**
 * Liefert den Wert des Nachlauf Sensor er liefert 1 wenn der Roomba Vorwärts fährt und 0 Wenn der Roomba sich seitlich oder nicht bewegt. 
 */
#define ROOMBA_SENSOR_STASIS                    58 
/** @}*/ /* roomba_sensor_extern */
/**
 * @defgroup roomba_sensor_intern Interne Sensor Daten
 * @{
 */
/** 
 * Zeigt den Status des Lade Zyklus an. 0 = Lädt nicht 1 = Wiederauffrisch- Ladezyklus 2 = Aufgeladen 3 = Entladung 4 = Warten 5 = Fehler beim Laden 
 */
#define ROOMBA_SENSOR_CHANGING_STATE             21 
/** 
 * Zeigt die Aktuelle Spannung der Quelle an in Millivolt. Übertragen wird ein 16 Bit Wert von 0 - 65535 mV Syntax [22] [High Byte] [Low byte] 
 */
#define ROOMBA_SENSOR_VOLTAGE                    22 
/** 
 * Zeigt den Aktuellen Verbrauch Milliampere an. Ein negativer Wert zeigt an das die Spannungsquelle belastet wird(Normal Betrieb) und ein positiver Wert zeigt an das die Spannungsquelle geladen wird. 
 */
#define ROOMBA_SENSOR_CURRENT                    23 
/** 
 * Zeigt die Temperatur in Grad Celsius der Batterie an. Wert von -128 - 127 
 */
#define ROOMBA_SENSOR_TEMPERATURE                24 
/** 
 * Aktuelle lade Wert in Milliampere Stunden. Gibt einen 16 Bit Wert von 0 - 65535 mAh zurück. Syntax: [25] [High Byte] [Low byte] 
 */
#define ROOMBA_SENSOR_BATTERY_CHARGE             25 
/** 
 * Zeigt die Aktuelle Ladung der Batterie an in Milliampere Stunden. Gibt einen 16-Bit Wert von 0 - 65535 mAh zurück. Syntax: [26] [High Byte] [Low byte] 
 */
#define ROOMBA_SENSOR_BATTERY_CAPACITY           26 
/**
 * Gibt an welche Strom Quelle aktuell angeschlossen ist. Gibt eine 8- Bit Maske zurück Syntax Reserved&Reserved&Reserved&Reserved&Reserved&Reserved&Home Base&Inernal Changer Syntax: [34] [Bit Mask] 
 */
#define ROOMBA_SENSOR_CHARGING_SOURCES_AVAILABLE 34 
/** 
 * Gibt den Aktuellen OI Modus zurück. 0 = Off 1 = Passive 2 = Safe 3 = Full 
 */
#define ROOMBA_SENSOR_OI_MODE                    35 
/** 
 * Gibt die Aktuelle Song Nummer an die Aktuell abgespielt wird. Wert von 0 - 15 
 */
#define ROOMBA_SENSOR_SONG_NUMBER                36 
/** 
 * Gibt zurück ob aktuell ein Song abgespielt wird oder nicht. 0 = kein Song wird abgespielt 1 = Song wird abgespielt 
 */
#define ROOMBA_SENSOR_SONG_PLAYING               37 
/** 
 * Gibt die aktuelle Nummer des Stream Paketes zurück. Es kann ein Wert von 0-108 zurück  
 */
#define ROOMBA_SENSOR_NUMBER_STREAM_PACKETS      38 
/** 
 * Aktueller Strom Verbrauch des Linken Motors. Liefert ein 16 Bit Wert zwischen -32768 – 32767 mA zurück Syntax [52] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_MOTOR_CURRENT_LEFT         54 
/** 
 * Aktueller Strom Verbrauch des Rechten Motors. Liefert ein 16 Bit Wert zwischen -32768 – 32767 mA zurück Syntax [53] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_MOTOR_CURRENT_RIGHT        55 
/** 
 * Aktueller Strom Verbrauch des Staubsaugmotors. Liefert ein 16 Bit Wert zwischen -32768 – 32767 mA zurück Syntax [53] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_MOTOR_BRUSH_MAIN           56 
/** 
 * Aktueller Strom Verbrauch des Seitlichen Staubsaugmotors. Liefert ein 16 Bit Wert zwischen -32768 – 32767 mA zurück Syntax [53] [High Byte] [Low Byte] 
 */
#define ROOMBA_SENSOR_MOTOR_BRUSH_SIDE           57 
/** @}*/ /* roomba_sensor_intern */
/**
 * @defgroup roomba_oi_modes OI Modes
 * @{
 */
/**
 * Off Mode
 * 
 * In diesem Modus nimmt der Roomba nur den {@link ROOMBA_START} Befehl an
 */
#define ROOMBA_SENSOR_OI_MODE_OFF 0
/**
 * Passiv Mode
 * 
 * In diesem Modus nimmder Roomba alle Sensor Befehle an, den {@link ROOMBA_SONG} und alle Mode Befehl an
 */
#define ROOMBA_SENSOR_OI_MODE_PASSIVE 1
/**
 * Safe Mode
 * 
 * Im Safe Mode akzeptiert der Roomba alle Befehl 
 * 
 * Die interne Sotware wechselt automatisch in den {@link ROOMBA_SENSOR_OI_MODE_PASSIVE} wenn ein Kritischer Zustand Auftritt
 */
#define ROOMBA_SENSOR_OI_MODE_SAFE 2
/**
 * Full Mode
 * 
 * Im Full Mode akzeptiert der Roomba alle Befehle. Die interne API ist deaktiviert!
 */
#define ROOMBA_SENSOR_OI_MODE_FULL 3
/** @}*/ /* roomba_oi_modes */
/** @}*/ /* roomba_sensor */


/** @}*/ /* roomba_def*/
#endif
