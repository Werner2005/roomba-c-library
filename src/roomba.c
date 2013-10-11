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
#include <roomba.h>
#include <stdint.h>
#include <string.h>
#ifdef ROOMBA_DEBUG
#include <stdio.h>
#endif

/*
 * Splint Anoationen
 * 
 * Erlaube Entlosschleifen
 * Wird verwednet um auf ein wert zu pollen. Der Interrupt Handler setzt dieses Wert!
 */
/*@-infloops@*/


/**
 * Write Callback
 */
static int (*write)(unsigned char c);

/*
 * Error Variable
 */
static volatile uint8_t error = ROOMBA_ERROR_NO;

/**
 * Status
 */
static volatile roombaStatus_t status = idle;
/**
 * Letzes Kommando fuer Read
 */
static volatile roombaCommand_t lastCommand;

/**
 * Daten Buffer
 * Mit der Gruppe 100 Koennen 58 SensorDaten vom Rommba entfanngen werden
 * Ist als Ringbuffer implementiert
 */
static volatile roombaSensorData_t sensorbuffer[ROOMBA_SENSOR_BUFFER_SIZE];
/**
 * Head Position
 */
static volatile uint8_t sensorbuffer_head_pos = 0;
/**
 * Foot Position
 */
static volatile uint8_t sensorbuffer_tail_pos = 0;

/**
 * Imkrementiert Position um 1
 * x = (x+1)%ROOMBA_SENSOR_BUFFER_SIZE
 * 
 * @param x Variable die Inkrementiert werden soll 
 */
#define INKREMENT_RINGBUFFER(x) (x = ((x+1) == ROOMBA_SENSOR_BUFFER_SIZE)?0:x+1)
/**
 * Inkrmenteiert Head Position um 1
 */
#define INKREMENT_HEAD_POS() INKREMENT_RINGBUFFER(sensorbuffer_head_pos)
/**
 * Inkrmenteiert Head Position um 1
 */
#define INKREMENT_TAIL_POS() INKREMENT_RINGBUFFER(sensorbuffer_tail_pos)
/**
 * Gibt Zurück ob sennsorbuffer lehr ist
 * @return true=leer false=nicht leer
 */
#define EMPTY(head, tail) (head == tail)
/**
 * Preuft ob Sensorbuffer voll ist
 * @return true=voll false=leer
 */
#define ISFULL(head, tail) (head == ((tail+1 == ROOMBA_SENSOR_BUFFER_SIZE)?0:tail+1))

#define SENSOR_EMPTY() EMPTY(sensorbuffer_head_pos, sensorbuffer_tail_pos)

#define SENSOR_ISFULL() ISFULL(sensorbuffer_head_pos, sensorbuffer_tail_pos)

#ifdef ROOMBA_SHORT_READ
static volatile uint8_t charbuffer[ROOMBA_SENSOR_BUFFER_SIZE];
static volatile uint8_t charbuffer_head_pos = 0;
static volatile uint8_t charbuffer_tail_pos = 0;
#define CHAR_EMPTY() EMPTY(charbuffer_head_pos, charbuffer_tail_pos)
#define CHAR_ISFULL() ISFULL(charbuffer_head_pos, charbuffer_tail_pos)
#endif
/**
 * Position 
 */
static volatile uint8_t pos = 0;
/**
 * Zaehler
 */
static volatile uint8_t counter = 0;

/**
 * Stream Status
 * 
 * Aufbau eines Packetes
 * 
 * .---------------.---------------------------------------------.----------.
 * |               |                                             |          |
 * |    header     |                     Data                    | Checksum |
 * |               |                                             |          |
 * +--------.------+------------.----------.------------.--------+----------+
 * |        |      |            |          |            |        |          |
 * |   19   | Size | SensorID 1 |  Data 1  | SensorID 2 | Data 2 | Chechsum |
 * |        |      |            |          |            |        |          |
 * '---------------'------------'----------'------------'--------'----------'
 */
enum roombaStreamStatus{
	/**
	 * Header
	 */
	header,
	/**
	 * Size
	 */
	size,
	/**
	 * SensorID
	 */
	sensorID,
	/**
	 * Daten
	 */
	data,
	/**
	 * Checksumme
	 */
	checksum
};
typedef enum roombaStreamStatus roombaStreamStatus_t;

static volatile roombaStreamStatus_t streamStatus = header;
/**
 * 
 */
static volatile uint8_t streamSize = 0;

static volatile uint8_t streamSizeCounter = 0;

static volatile uint32_t streamSum = 0;
/**
 * Temporaer Pointer im sensor Buffer
 * 
 * wird verwendet um daten im Buffer reinzulegen aber diese noch nicht freizugeben
 * fals die Checksumme nicht gueltig war kann so alle Sensor Daten als nicht gueltig makiert werden
 */
static volatile uint8_t streamSensorbuffer_tail_pos = 0;


static int8_t setError(uint8_t err);

/**
 * Roomba Init Methode
 */
void roomba_init(int (*w)(unsigned char c)){
	write = w;
}

/**
 * GIbt die Groese Zurueck
 * @param c Sensor Eingbae Zeichen
 * @return Gibt die Groese eines Sensor Einrages zurueck
 */
static uint8_t getSize(uint8_t c){
	switch(c){
		case ROOMBA_SENSOR_GROUP_0:
			return ROOMBA_SENSOR_GROUP_0_SIZE;
		case ROOMBA_SENSOR_GROUP_1:
			return ROOMBA_SENSOR_GROUP_1_SIZE;
		case ROOMBA_SENSOR_GROUP_2:
			return ROOMBA_SENSOR_GROUP_2_SIZE;
		case ROOMBA_SENSOR_GROUP_3:
			return ROOMBA_SENSOR_GROUP_4_SIZE;
		case ROOMBA_SENSOR_GROUP_5:
			return ROOMBA_SENSOR_GROUP_5_SIZE;
		case ROOMBA_SENSOR_GROUP_6:
			return ROOMBA_SENSOR_GROUP_6_SIZE;
		case ROOMBA_SENSOR_GROUP_100:
			return ROOMBA_SENSOR_GROUP_100_SIZE;
		case ROOMBA_SENSOR_GROUP_101:
			return ROOMBA_SENSOR_GROUP_101_SIZE;
		case ROOMBA_SENSOR_GROUP_106:
			return ROOMBA_SENSOR_GROUP_106_SIZE;
		case ROOMBA_SENSOR_GROUP_107:
			return ROOMBA_SENSOR_GROUP_107_SIZE;
		case ROOMBA_SENSOR_DISTANCE:
		case ROOMBA_SENSOR_ANGLE:
		case ROOMBA_SENSOR_VOLTAGE:
		case ROOMBA_SENSOR_CURRENT:
		case ROOMBA_SENSOR_BATTERY_CHARGE:
		case ROOMBA_SENSOR_BATTERY_CAPACITY:
		case ROOMBA_SENSOR_WALL_SIGNAL:
		case ROOMBA_SENSOR_CLIFF_SIGNAL_LEFT:
		case ROOMBA_SENSOR_CLIFF_SIGNAL_FRONT_LEFT:
		case ROOMBA_SENSOR_CLIFF_SIGNAL_FRONT_RIGHT:
		case ROOMBA_SENSOR_CLIFF_SIGNAL_RIGHT:
		case ROOMBA_SENSOR_REQUESTED_VELOCITY:
		case ROOMBA_SENSOR_REQUESTED_RADIUS:
		case ROOMBA_SENSOR_REQUESTED_VELOCITY_RIGHT:
		case ROOMBA_SENSOR_REQUESTED_VELOCITY_LEFT:
		case ROOMBA_SENSOR_ENCODER_COUNTS_RIGHT:
		case ROOMBA_SENSOR_ENCODER_COUNTS_LEFT:
		case ROOMBA_SENSOR_LIGHT_BUMPER_LEFT:
		case ROOMBA_SENSOR_LIGHT_BUMPER_FRONT_LEFT:
		case ROOMBA_SENSOR_LIGHT_BUMPER_CENTER_LEFT:
		case ROOMBA_SENSOR_LIGHT_BUMPER_CENTER_RIGHT:
		case ROOMBA_SENSOR_LIGHT_BUMPER_FRONT_RIGHT:
		case ROOMBA_SENSOR_LIGHT_BUMPER_RIGHT:
		case ROOMBA_SENSOR_MOTOR_CURRENT_LEFT:
		case ROOMBA_SENSOR_MOTOR_CURRENT_RIGHT:
		case ROOMBA_SENSOR_MOTOR_BRUSH_MAIN:
		case ROOMBA_SENSOR_MOTOR_BRUSH_SIDE:
		case ROOMBA_SENSOR_UNUSED_3:
			return 2;
		default:
			return 1;
	}
}
static int8_t getGroupStart(uint8_t c){
	switch(c){
		case ROOMBA_SENSOR_GROUP_0:
			return ROOMBA_SENSOR_GROUP_0_START;
		case ROOMBA_SENSOR_GROUP_1:
			return ROOMBA_SENSOR_GROUP_1_START;
		case ROOMBA_SENSOR_GROUP_2:
			return ROOMBA_SENSOR_GROUP_2_START;
		case ROOMBA_SENSOR_GROUP_3:
			return ROOMBA_SENSOR_GROUP_3_START;
		case ROOMBA_SENSOR_GROUP_4:
			return ROOMBA_SENSOR_GROUP_4_START;
		case ROOMBA_SENSOR_GROUP_5:
			return ROOMBA_SENSOR_GROUP_5_START;
		case ROOMBA_SENSOR_GROUP_6:
			return ROOMBA_SENSOR_GROUP_6_START;
		case ROOMBA_SENSOR_GROUP_100:
			return ROOMBA_SENSOR_GROUP_100_START;
		case ROOMBA_SENSOR_GROUP_101:
			return ROOMBA_SENSOR_GROUP_101_START;
		case ROOMBA_SENSOR_GROUP_106:
			return ROOMBA_SENSOR_GROUP_106_START;
		case ROOMBA_SENSOR_GROUP_107:
			return ROOMBA_SENSOR_GROUP_107_START;
	}
	return -1;
}
static int8_t getGroupEnd(uint8_t c){
	switch(c){
		case ROOMBA_SENSOR_GROUP_0:
			return ROOMBA_SENSOR_GROUP_0_STOP;
		case ROOMBA_SENSOR_GROUP_1:
			return ROOMBA_SENSOR_GROUP_1_STOP;
		case ROOMBA_SENSOR_GROUP_2:
			return ROOMBA_SENSOR_GROUP_2_STOP;
		case ROOMBA_SENSOR_GROUP_3:
			return ROOMBA_SENSOR_GROUP_3_STOP;
		case ROOMBA_SENSOR_GROUP_4:
			return ROOMBA_SENSOR_GROUP_4_STOP;
		case ROOMBA_SENSOR_GROUP_5:
			return ROOMBA_SENSOR_GROUP_5_STOP;
		case ROOMBA_SENSOR_GROUP_6:
			return ROOMBA_SENSOR_GROUP_6_STOP;
		case ROOMBA_SENSOR_GROUP_100:
			return ROOMBA_SENSOR_GROUP_100_STOP;
		case ROOMBA_SENSOR_GROUP_101:
			return ROOMBA_SENSOR_GROUP_101_STOP;
		case ROOMBA_SENSOR_GROUP_106:
			return ROOMBA_SENSOR_GROUP_106_STOP;
		case ROOMBA_SENSOR_GROUP_107:
			return ROOMBA_SENSOR_GROUP_107_STOP;
	}
	return -1;
}
/**
 * Prüft ob es vorher ein Sensor Befehl gesendet worden ist oder nicht
 *
 * @param c Das zu Pruefende Kommando
 */
static __inline__ uint8_t getSennsorID(roombaCommand_t c){
	/* 
	 * Kein Sennsors Befhel
	 */
	if (!(c.command == ROOMBA_SENSORS || c.command == ROOMBA_STREAM || c.command == ROOMBA_QUERY_LIST) || c.size == 0){
		return 255;
	}

	return c.data[0];
}

static uint8_t stream_ignore = 0;
static uint8_t stream_ignore_counter = 0;
static uint8_t stream_sync = 1;

static int8_t stream_reader(uint8_t c){
	uint8_t tmp;
	streamSum += c;
	streamSizeCounter++;
	switch(streamStatus){
		case header:
			#ifdef ROOMBA_DEBUG
			printf("Stream Status: header\n");
			#endif
			/*
			 * Resetzen aller Variablen
			 * Stream Header wird zu Checksumme hinzugefuegt
			 */
			streamSum = 19;
			streamSize = 0;
			streamSizeCounter = 0;
			if(c == 19){
				if(stream_ignore == 1){
					stream_ignore_counter++;
					if(stream_ignore_counter > ROOMBA_STREAM_IGNORE){
						stream_ignore_counter = 0;
						stream_ignore = 0;
						stream_sync = 1;
						if(SENSOR_ISFULL()){
							stream_ignore = 1;
							streamStatus = header;
							/*return -setError(ROOMBA_ERROR_BUFFER_FULL);*/
							return -setError(ROOMBA_ERROR_BUFFER_FULL_STREAM);
						}
					}else{
						break;
					}
				}
				streamStatus = size;
				if(SENSOR_ISFULL()){
					stream_ignore = 1;
					break;
				}
				/*
				 * Aktuelle Sensorbuffer Position finden
				 */
				streamSensorbuffer_tail_pos = sensorbuffer_tail_pos;
				/*
				 * Position reseten
				 */
				pos = 1;
				/*
				 * Setze SensorID
				 */
				sensorbuffer[streamSensorbuffer_tail_pos].sensorID = lastCommand.data[pos];
				/*
				 * Setzt Groese
				 */
				sensorbuffer[streamSensorbuffer_tail_pos].size = getSize(lastCommand.data[pos]);
				/*
				 * Loesche Data
				 */
				sensorbuffer[streamSensorbuffer_tail_pos].data = 0;
				break;
			}else{
				if(stream_ignore == 0){
					#ifdef ROOMBA_DEBUG
					printf("Error Header nicht gefunden\n");
					#endif
					streamStatus = header;
					return -setError(ROOMBA_ERROR_HEADER_NOT_FOUND);
				}else{
					break;
				}
			}
		case size:
			#ifdef ROOMBA_DEBUG
			printf("Stream Status: size\n");
			#endif
			streamSize = c;
			/*
			 * Header zahlt nicht zur Groese
			 */
			streamSizeCounter = 0;
			streamStatus = sensorID;
			break;
		case sensorID:
			#ifdef ROOMBA_DEBUG
			printf("Stream Status: sensorID\n");
			#endif
			if(c == sensorbuffer[streamSensorbuffer_tail_pos].sensorID){
				streamStatus = data;
			}else{
				streamStatus = header;
				if(stream_sync == 0){
					#ifdef ROOMBA_DEBUG
					printf("Error SensorID nicht passent\n");
					#endif
					return -setError(ROOMBA_ERROR_SENSORID_NOT_FOUND);
				}else{
					break;
				}
			}
			break;
		case data:
			#ifdef ROOMBA_DEBUG
			printf("Stream Status: data\n");
			#endif
			tmp = sensorbuffer[streamSensorbuffer_tail_pos].size;
			#ifdef ROOMBA_DEBUG
			printf("receive Data: %x Pos: (%d-%d-1)*8 = %d\n", c,tmp, counter, ((tmp-counter-1)*8));
			#endif
			/*
			 * Setze Daten
			 * High Bit um 8 Bit Nach Rechtschiften
			 */
			sensorbuffer[streamSensorbuffer_tail_pos].data |= c << ((tmp-counter-1)*8);

			/**
			 * Sensor Data vollstaendig entfangen
			 */
			if(counter == (tmp-1)){
				pos++;
				counter = 0;	
				INKREMENT_RINGBUFFER(streamSensorbuffer_tail_pos);
				if(ISFULL(sensorbuffer_head_pos, streamSensorbuffer_tail_pos)){
					/*streamStatus = header;
					return -setError(ROOMBA_ERROR_BUFFER_FULL_STREAM);*/
					stream_ignore = 1;
					break;
				}
				if(pos <= lastCommand.data[0]){
					#ifdef ROOMBA_DEBUG
					printf("Next SensorID: %d %d of %d\n",lastCommand.data[pos] , pos, lastCommand.data[0]);
					#endif
					/*
					 * Setze SensorID
					 */
					sensorbuffer[streamSensorbuffer_tail_pos].sensorID = lastCommand.data[pos];
					/*
					 * Setzt Groese
					 */
					sensorbuffer[streamSensorbuffer_tail_pos].size = getSize(lastCommand.data[pos]);
					/*
					 * Loesche Data
					 */
					sensorbuffer[streamSensorbuffer_tail_pos].data = 0;
					streamStatus = sensorID;
				}else{
					streamStatus = checksum;
				}
			}else{
				counter++;
			}
			break;
		case checksum:
			#ifdef ROOMBA_DEBUG
			printf("Stream Status: checksum\n");
			#endif
			/*
			 * Checksummer uebrpeufen 
			 *
			 * streamSizeCounter - 1 Cehcksum wird nicht mit gezahlt
			 */
			if((streamSum & 0xFF) == 0 && streamSize == streamSizeCounter-1){
				streamStatus = header;
				stream_sync = 0;
				#ifdef ROOMBA_DEBUG 
				printf("Setze buffer Tail Pos neu. Alt: %d neu: %d\n", sensorbuffer_tail_pos, streamSensorbuffer_tail_pos);
				#endif
				/*
				 * Daten Freigeben
				 */
				sensorbuffer_tail_pos = streamSensorbuffer_tail_pos;
			}else{
				streamStatus = header;
				if(stream_sync == 0){
					#ifdef ROOMBA_DEBUG
					printf("Error Checksum nicht gueltig %d stramSize: %d == streamSizeCounter: %d\n", (uint8_t) (uint8_t) (uint8_t) (uint8_t) (uint8_t) (uint8_t) (uint8_t) (uint8_t) (streamSum & 0xFF), streamSize, streamSizeCounter-1);
					#endif
					return -setError(ROOMBA_ERROR_CHECKSUM_NOT_VALID);
				}else{
					break;
				}
			}
			break;
	}
	return 0;
}
/**
 * Read Funktion
 */
#ifndef ROOMBA_SHORT_READ
int8_t roomba_read(uint8_t c){
#else
static int8_t roomba_read_parser(uint8_t c){
#endif
	uint8_t tmp;
	if(SENSOR_ISFULL()){
		#ifdef ROOMBA_DEBUG
		printf("error Buffer voll\n");
		#endif
		return -setError(ROOMBA_ERROR_BUFFER_FULL);
	}

	if(status == idle){
		/**
		 * Sende Stream Abbruch da es sein kann das ein Stream noch aktiv ist
		 */
		(void) write((unsigned char) ROOMBA_STREAM_PAUSE_RESUME);
		(void) write((unsigned char) 0);
		#ifdef ROOMBA_DEBUG
		printf("Error Keine Daten erwaretet\n");
		#endif
		return -setError(ROOMBA_ERROR_NO_SIGN_EXPECTED);
	/*
	 * Stream auswerten
	 */
	}else if(status == stream){
		return stream_reader(c);
	}

	tmp = sensorbuffer[sensorbuffer_tail_pos].size;
	#ifdef ROOMBA_DEBUG
	printf("receive Data: %x Pos: (%d-%d-1)*8 = %d\n", c,tmp, counter, ((tmp-counter-1)*8));
	#endif
	/*
	 * Setze Daten
	 * High Bit um 8 Bit Nach Rechtschiften
	 */
	sensorbuffer[sensorbuffer_tail_pos].data |= c << ((tmp-counter-1)*8);

	/**
	 * Sensor Data vollstaendig entfangen
	 */
	if(counter == (tmp-1)){
		/*
		 * Counter reseten
		 */
		counter = 0;
		/*
		 * Daten Freigeben
		 */
		INKREMENT_TAIL_POS();
		switch(status){
			case singeldata:
				/*
				 * Status wieder auf idle setzen
				 */
				status = idle;
				#ifdef ROOMBA_DEBUG
				printf("status: idle\n");
				#endif
				break;
			case group:
				/**
				 * Inkrementiere Positoin
				 */
				pos++;
				tmp = getSennsorID(lastCommand);
				/*
				 * Preufe ob alle Daten entfangen worden sind
				 */
				if((int8_t) pos <= getGroupEnd(tmp)){
					#ifdef ROOMBA_DEBUG
					printf("Next SensorID: %d of %d\n", pos, getGroupEnd(tmp));
					#endif
					/*
					 * Setze SensorID
					 */
					sensorbuffer[sensorbuffer_tail_pos].sensorID = pos;
					/*
					 * Setzt Groese
					 */
					sensorbuffer[sensorbuffer_tail_pos].size = getSize(pos);
					/*
					 * Loesche Data
					 */
					sensorbuffer[sensorbuffer_tail_pos].data = 0;
				}else{
					#ifdef ROOMBA_DEBUG
					printf("End Group Pos: %d of %d\n", pos, getGroupEnd(tmp));
					#endif
					/*
					 * Status wieder auf idle setzen
					 */
					status = idle;
					#ifdef ROOMBA_DEBUG
					printf("status: idle\n");
					#endif
				}
				break;
			case query:
				pos++;
				if(pos <= lastCommand.data[0]){
					#ifdef ROOMBA_DEBUG
					printf("Next SensorID: %d %d of %d\n",lastCommand.data[pos] , pos, lastCommand.data[0]);
					#endif
					/*
					 * Setze SensorID
					 */
					sensorbuffer[sensorbuffer_tail_pos].sensorID = lastCommand.data[pos];
					/*
					 * Setzt Groese
					 */
					sensorbuffer[sensorbuffer_tail_pos].size = getSize(lastCommand.data[pos]);
					/*
					 * Loesche Data
					 */
					sensorbuffer[sensorbuffer_tail_pos].data = 0;
				}else{
					/*
					 * Status wieder auf idle setzen
					 */
					status = idle;
					#ifdef ROOMBA_DEBUG
					printf("status: idle\n");
					#endif
				}
				break;
			default:
				#ifdef ROOMBA_DEBUG
				printf("Haette nicht passieren druerfen %s %s %d", __FILE__, __FUNCTION__, __LINE__);
				#endif
				break;
		}
	
	/**
	 * Erwarte naechsten 8 Bit
	 */
	}else{
		counter++;
	}
	return 0;
}
#ifdef ROOMBA_SHORT_READ
static int8_t flushBuffer(){
	int8_t ret;
	/*
	 * Parse Daten im Read Buffer
	 */
	while(!CHAR_EMPTY()){
		#if ROOMBA_DEBUG
		printf("Parse Daten: %c = 0x%x Headpos: %u\n", charbuffer[charbuffer_head_pos], charbuffer[charbuffer_head_pos], charbuffer_head_pos);
		#endif
		/*
		 * Wenn Buffer Voll warte bis wieder platz da ist;)
		 */
		ret = roomba_read_parser(charbuffer[charbuffer_head_pos]);
		if(ret == (int8_t)(ROOMBA_ERROR_BUFFER_FULL*(-1)) || ret == (int8_t)(ROOMBA_ERROR_BUFFER_FULL_STREAM*(-1))){
			break;
		}
		INKREMENT_RINGBUFFER(charbuffer_head_pos);
	}
	return 0;
}
int8_t roomba_read(uint8_t c){
	if(CHAR_ISFULL()){
		/*
		 * Buffer Voll Sende Stream Pause!
		 */
		#ifdef ROOMBA_DEBUG
		printf("Char Buffer full %c=0x%x\n", c, c);
		#endif
		return -setError(ROOMBA_ERROR_BUFFER_FULL_CHAR);
	}
	if(status == idle){
		/**
		 * Sende Stream Abbruch da es sein kann das ein Stream noch aktiv ist
		 */
		(void) write((unsigned char) ROOMBA_STREAM_PAUSE_RESUME);
		(void) write((unsigned char) 0);
		/**
		 * Keine Daten erwartet
		 * 
		 * Da es sein kann das ein Stream aktiv ist wird ein Pause Befehl gesenet um den Stram abzuschalten
		 */
		#ifdef ROOMBA_DEBUG
		printf("Error Keine Daten erwaretet %c=0x%x\n", c, c);
		#endif
		return -setError(ROOMBA_ERROR_NO_SIGN_EXPECTED);
	}
	charbuffer[charbuffer_tail_pos] = c;
	/*#if ROOMBA_DEBUG
	printf("New Charbuffer Tail Pos: %u\n", charbuffer_tail_pos+1);
	#endif*/
	INKREMENT_RINGBUFFER(charbuffer_tail_pos);
	return 0;
}
#endif
/*
 * Gebe Sensordaten zuruck
 */
int8_t roomba_getSensorData(roombaSensorData_t* data){
	#ifdef ROOMBA_SHORT_READ
	int8_t ret;
	ret = flushBuffer();
	if(ret < 0) return ret;
	#endif
	#ifndef ROOMBA_NONBLOCKING
	/*
	 * Blockiere bis Daten entfangen werden
	 */
	while(SENSOR_EMPTY()){}
	#else
	if(SENSOR_EMPTY()){
		return ((int8_t)ROOMBA_ERROR_AGAIN)*(-1);
	}
	#endif
	#ifdef ROOMBA_DEBUG
	printf("Lese HeadPos: %u\n", sensorbuffer_head_pos);
	#endif
	/*
	 * Kopiere Sensor Daten
	 */
	data->sensorID = sensorbuffer[sensorbuffer_head_pos].sensorID;
	data->data = sensorbuffer[sensorbuffer_head_pos].data;
	data->size = sensorbuffer[sensorbuffer_tail_pos].size;
	/*
	 * Gebe Aktuelle Pos im Buffer frei
	 */
	INKREMENT_HEAD_POS();
	return 0;
}
/**
 * Uebersetzt Group ID in SensorIDs(Wird benoetigt fuer Stream)
 * Uebergebener Parameter wird veraendert!
 * 
 * @param c Command der Erwetert werden soll
 * @return -{@link ROOMBA_ERROR_TO_BIG} Wenn die Anfrage grosser wird als {@link ROOMBA_MAX_DATA} 0 wenn kein fehler aufgetreten ist
 */
static int8_t expandGroupIDs(roombaCommand_t* c){
	roombaCommand_t tmp;
	uint8_t i;
	uint8_t position = 1;
	int8_t ret;
	uint8_t sid;
	uint8_t end;
	tmp.command = c->command;
	for(i = 1; i < c->size; i++){
		if(position == ROOMBA_MAX_DATA){
			return -setError(ROOMBA_ERROR_TO_BIG);
		}
		ret = getGroupStart(c->data[i]);
		if(ret != -1){
			end = (uint8_t) getGroupEnd(c->data[i]);
			for(sid = (uint8_t) ret; sid <= end; sid++){
				if(position == ROOMBA_MAX_DATA){
					return -setError(ROOMBA_ERROR_TO_BIG);
				}
				tmp.data[position++] = sid;
			}
		}else{
			tmp.data[position++] = c->data[i];
		}
	}
	tmp.size = position;
	tmp.data[0] = position-1;
	memcpy(c, &tmp, sizeof(roombaCommand_t));
	return 0;
}
/*
 * Sende Command
 */
int8_t roomba_sendCommand(roombaCommand_t* c){
	uint8_t i;
	uint8_t sensorID = 0;
	int tmp;
	int8_t ret;
	/*
	 * Prüfen ob Daten erwaretet wird und setze entsprechent status
	 * Blockiert fals grade Daten entfangen worden sind!
	 */
	if(c->command == ROOMBA_SENSORS || c->command == ROOMBA_STREAM || c->command == ROOMBA_QUERY_LIST){

		#ifdef ROOMBA_SHORT_READ
		/*
		 * Verarbeite Daten im Buffer
		 * Verhindert das Daten im Buffer sind die nicht zu dem Aktuellen Befehl passen!
		 */
		ret = flushBuffer();
		if(ret < 0) return ret;
		#endif

		/*
		 * Pruefen ob es sich um ein SENSOR Command handelt oder ein STEAM Kommando
		 */
		switch(c->command){
			case ROOMBA_SENSORS:
			{

				#ifndef ROOMBA_NONBLOCKING
				/*
				 * Warte bis Daten entfangen sind bevor ein neuer Befehl gesendet wird
				 */
				while(!SENSOR_ISFULL() && status != idle) {}
				#else
				if(!SENSOR_ISFULL() && status != idle){
					return ((int8_t)ROOMBA_ERROR_AGAIN)*(-1);
				}
				#endif
				pos = 0;
				/*
				 * Lese SensorID aus Command
				 */
				sensorID = getSennsorID(*c);
				switch(c->data[0]){
					/*
					 * Erwarte Group Nachrichten
					 */
					case ROOMBA_SENSOR_GROUP_0:
					case ROOMBA_SENSOR_GROUP_1:
					case ROOMBA_SENSOR_GROUP_2:
					case ROOMBA_SENSOR_GROUP_3:
					case ROOMBA_SENSOR_GROUP_4:
					case ROOMBA_SENSOR_GROUP_5:
					case ROOMBA_SENSOR_GROUP_6:
					case ROOMBA_SENSOR_GROUP_100:
					case ROOMBA_SENSOR_GROUP_101:
					case ROOMBA_SENSOR_GROUP_106:
					case ROOMBA_SENSOR_GROUP_107:
						#ifdef ROOMBA_DEBUG
						printf("status: group\n");
						#endif
						status = group;
						/*
						 * Hohle Start SensorID
						 */
						pos = (uint8_t) getGroupStart(sensorID);
						#ifdef ROOMBA_DEBUG
						printf("GroupID: %d \nSensorID: %d\n", sensorID, pos);
						#endif
						sensorID = pos;
						break;
					/*
					 * Erwarte Singel Data
					 */
					default:
						#ifdef ROOMBA_DEBUG
						printf("status: singeldata\n");
						#endif
						status = singeldata;
						break;
				}
				break;
			}
			case ROOMBA_QUERY_LIST:
			{
				/*
				for(i = 1; i <= c->data[0]; i++){
					if(getGroupStart(c->data[i]) != -1){
						#ifdef ROOMBA_DEBUG
						printf("error Gruppen koennen nicht ueber Query abgerufen werden\n");
						#endif
						return -setError(ROOMBA_ERROR_QUERY_AND_GROUP);
					}
				}*/
				/**
				 * Expandiere Gruppen
				 *
				 * Dies wird gemacht um eine einfache Verarbeitung beim Enpfangen zu Gewerleisten
				 * Flas ein Fehler auftritt wird dieser Zurckgeben und der Befhel wird nicht gesenet
				 */
				ret = expandGroupIDs(c);
				if(ret < 0){
					return ret;
				}
				#ifdef ROOMBA_DEBUG
				printf("status: Query List\n");
				#endif
				status = query;
				/*
				 * Erste SensorID die Erwartet wird ist auf pos 1, da in data[0] die groese der Erwarteten Daten ist.
				 */
				pos = 1;
				sensorID = c->data[pos];
				#ifdef ROOMBA_DEBUG
				printf("Next SensorID: %d %d of %d\n",sensorID , pos, c->data[0]);
				#endif
				break;
			}
			/*
			 * Erwarte Sensor Steam
			 */
			case ROOMBA_STREAM:
			{
				/**
				 * Alter Stream beenden
				 */
				if(status == stream){
					(void) write((unsigned char) ROOMBA_STREAM_PAUSE_RESUME);
					(void) write((unsigned char) 0);
				}
				#ifdef ROOMBA_DEBUG
				printf("status: steam\n");
				#endif
				/**
				 * Expandiere Gruppen
				 *
				 * Dies wird gemacht um eine einfache Verarbeitung beim Enpfangen zu Gewerleisten
				 * Flas ein Fehler auftritt wird dieser Zurckgeben und der Befhel wird nicht gesenet
				 */
				ret = expandGroupIDs(c);
				if(ret < 0){
					return ret;
				}
				status = stream;
				streamStatus = header;
				pos = 1;
				sensorID = c->data[pos];
				break;
			}
			default:
				/**
				 * Kann nimals auftretten
				 */
				return -1;
		}
		counter = 0;	
		/*
		 * Buffer vorbereitet
		 */
		#ifdef ROOMBA_DEBUG
		printf("Schreibe in Bufferpos: %d\n", sensorbuffer_tail_pos);
		#endif
		#ifdef ROOMBA_DEBUG
		printf("SensorID: %d\n", sensorID);
		#endif
		/*
		 * Setze SensorID
		 */
		sensorbuffer[sensorbuffer_tail_pos].sensorID = sensorID;
		/*
		 * Setzt Groese
		 */
		sensorbuffer[sensorbuffer_tail_pos].size = getSize(sensorID);
		/*
		 * Loesche Data
		 */
		sensorbuffer[sensorbuffer_tail_pos].data = 0;

		/*
		 * Setze LastCommad fuer read
		 */
		lastCommand.command = c->command;
		lastCommand.size = c->size;
		for(i = 0; i < c->size; i++){
			lastCommand.data[i] = c->data[i];
		}
	}
	/*
	 * Schreibe Befehl
	 */
	tmp = write((unsigned char) c->command);
	/*
	 * Error Behandlung
	 */
	if(tmp <= -1){
		status = idle;
		(void) setError(ROOMBA_ERROR_WRITE);
		return (int8_t) tmp;
	}
	/*
	 * Schreibe 
	 */
	for(i = 0; i < c->size; i++){
		tmp = write((unsigned char) c->data[i]);
		/*
		 * Error Behandlung
		 */
		if(tmp <= -1){
			status = idle;
			(void) setError(ROOMBA_ERROR_WRITE);
			return (int8_t) tmp;
		}
	}

	if(c->command == ROOMBA_STREAM_PAUSE_RESUME){
		if(c->data[0] == 1){
			streamStatus = header;
			status = stream;
		}else{
			/*
			 * Reseten des System
			 */
			roomba_reset();
		}
	}

	return 0;
}
/**
 * Resetet System
 */
void roomba_reset(){
	status = idle;
	streamStatus = header;
	#ifdef ROOMBA_SHORT_READ
	charbuffer_head_pos = 0;
	charbuffer_tail_pos = 0;
	#endif
	sensorbuffer_head_pos = 0;
	sensorbuffer_tail_pos = 0;
}
/*
 * Preft ob Sensor daten vorliegen
 */
uint8_t roomba_hasSensorData(){
	#ifdef ROOMBA_SHORT_READ
	(void) flushBuffer();
	#endif
	return (uint8_t) (!SENSOR_EMPTY()); 
}
static int8_t setError(uint8_t err){
	if(error == ROOMBA_ERROR_NO){
		return (int8_t) (error = err);
	}
	return (int8_t) error;
}
int8_t roomba_getError(){
	uint8_t err = error;
	#ifdef ROOMBA_DEBUG
	printf("Get Error: %d\n", err);
	#endif
	error = ROOMBA_ERROR_NO;
	return (int8_t) err;
}
roombaStatus_t roomba_getStatus(){
	return status;
}
void roomba_abort_sensor(){
	if(status == stream){
		(void) write((unsigned char) ROOMBA_STREAM_PAUSE_RESUME);
		(void) write((unsigned char) 0);
	}
	#ifdef ROOMBA_SHORT_READ
	/**
	 * Char Buffer leeren
	 */
	(void) flushBuffer();
	#endif
	if(status != idle){
		status = idle;
		(void) setError(ROOMBA_ERROR_ABORT_SENSOR);
	}
}
