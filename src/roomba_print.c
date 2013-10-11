#include <roomba_print.h>
#include <stdio.h>

#define SET_SENSOR_ID(s) {s, #s}
/*void SET_SENSOR_ID(int i){}*/

sensorString_t sensorStrings[] = {
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_0), /* ID: 0*/
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_1), /* ID: 1*/
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_2), /* ID: 2*/
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_3), /* ID: 3*/
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_4), /* ID: 4*/
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_5), /* ID: 5*/
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_6), /* ID: 6*/
	SET_SENSOR_ID(ROOMBA_SENSOR_BUMPS_WHEEL_DROPS), /* ID: 7*/
	SET_SENSOR_ID(ROOMBA_SENSOR_WALL), /* ID: 8*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CLIFF_LEFT), /* ID: 9*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CLIFF_FRONT_LEFT), /* ID: 10*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CLIFF_FRONT_RIGHT), /* ID: 11*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CLIFF_RIGHT), /* ID: 12*/
	SET_SENSOR_ID(ROOMBA_SENSOR_VIRTUAL_WALL), /* ID: 13*/
	SET_SENSOR_ID(ROOMBA_SENSOR_WHEEL_OVERCURRENTS), /* ID: 14*/
	SET_SENSOR_ID(ROOMBA_SENSOR_DIRT_DETECT), /* ID: 15*/
	SET_SENSOR_ID(ROOMBA_SENSOR_UNUSED_1), /* ID: 16*/
	SET_SENSOR_ID(ROOMBA_SENSOR_INFRARED_CHAR_OMMI), /* ID: 17*/
	SET_SENSOR_ID(ROOMBA_SENSOR_BUTTONS), /* ID: 18*/
	SET_SENSOR_ID(ROOMBA_SENSOR_DISTANCE), /* ID: 19*/
	SET_SENSOR_ID(ROOMBA_SENSOR_ANGLE), /* ID: 20*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CHANGING_STATE), /* ID: 21*/
	SET_SENSOR_ID(ROOMBA_SENSOR_VOLTAGE), /* ID: 22*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CURRENT), /* ID: 23*/
	SET_SENSOR_ID(ROOMBA_SENSOR_TEMPERATURE), /* ID: 24*/
	SET_SENSOR_ID(ROOMBA_SENSOR_BATTERY_CHARGE), /* ID: 25*/
	SET_SENSOR_ID(ROOMBA_SENSOR_BATTERY_CAPACITY), /* ID: 26*/
	SET_SENSOR_ID(ROOMBA_SENSOR_WALL_SIGNAL), /* ID: 27*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CLIFF_SIGNAL_LEFT), /* ID: 28*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CLIFF_SIGNAL_FRONT_LEFT), /* ID: 29*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CLIFF_SIGNAL_FRONT_RIGHT), /* ID: 30*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CLIFF_SIGNAL_RIGHT), /* ID: 31*/
	SET_SENSOR_ID(ROOMBA_SENSOR_UNUSED_2), /* ID: 32*/
	SET_SENSOR_ID(ROOMBA_SENSOR_UNUSED_3), /* ID: 33*/
	SET_SENSOR_ID(ROOMBA_SENSOR_CHARGING_SOURCES_AVAILABLE), /* ID: 34*/
	SET_SENSOR_ID(ROOMBA_SENSOR_OI_MODE), /* ID: 35*/
	SET_SENSOR_ID(ROOMBA_SENSOR_SONG_NUMBER), /* ID: 36*/
	SET_SENSOR_ID(ROOMBA_SENSOR_SONG_PLAYING), /* ID: 37*/
	SET_SENSOR_ID(ROOMBA_SENSOR_NUMBER_STREAM_PACKETS), /* ID: 38*/
	SET_SENSOR_ID(ROOMBA_SENSOR_REQUESTED_VELOCITY), /* ID: 39*/
	SET_SENSOR_ID(ROOMBA_SENSOR_REQUESTED_RADIUS), /* ID: 40*/
	SET_SENSOR_ID(ROOMBA_SENSOR_REQUESTED_VELOCITY_RIGHT), /* ID: 41*/
	SET_SENSOR_ID(ROOMBA_SENSOR_REQUESTED_VELOCITY_LEFT), /* ID: 42*/
	SET_SENSOR_ID(ROOMBA_SENSOR_ENCODER_COUNTS_RIGHT), /* ID: 43*/
	SET_SENSOR_ID(ROOMBA_SENSOR_ENCODER_COUNTS_LEFT), /* ID: 44*/
	SET_SENSOR_ID(ROOMBA_SENSOR_LIGHT_BUMPER), /* ID: 45*/
	SET_SENSOR_ID(ROOMBA_SENSOR_LIGHT_BUMPER_LEFT), /* ID: 46*/
	SET_SENSOR_ID(ROOMBA_SENSOR_LIGHT_BUMPER_FRONT_LEFT), /* ID: 47*/
	SET_SENSOR_ID(ROOMBA_SENSOR_LIGHT_BUMPER_CENTER_LEFT), /* ID: 48*/
	SET_SENSOR_ID(ROOMBA_SENSOR_LIGHT_BUMPER_CENTER_RIGHT), /* ID: 49*/
	SET_SENSOR_ID(ROOMBA_SENSOR_LIGHT_BUMPER_FRONT_RIGHT), /* ID: 50*/
	SET_SENSOR_ID(ROOMBA_SENSOR_LIGHT_BUMPER_RIGHT), /* ID: 51*/
	SET_SENSOR_ID(ROOMBA_SENSOR_INFRARED_CHAR_LEFT), /* ID: 52*/
	SET_SENSOR_ID(ROOMBA_SENSOR_INFRARED_CHAR_RIGHT), /* ID: 53*/
	SET_SENSOR_ID(ROOMBA_SENSOR_MOTOR_CURRENT_LEFT), /* ID: 54*/
	SET_SENSOR_ID(ROOMBA_SENSOR_MOTOR_CURRENT_RIGHT), /* ID: 55*/
	SET_SENSOR_ID(ROOMBA_SENSOR_MOTOR_BRUSH_MAIN), /* ID: 56*/
	SET_SENSOR_ID(ROOMBA_SENSOR_MOTOR_BRUSH_SIDE), /* ID: 57*/
	SET_SENSOR_ID(ROOMBA_SENSOR_STASIS),
	SET_SENSOR_ID(59),
	SET_SENSOR_ID(60),
	SET_SENSOR_ID(61),
	SET_SENSOR_ID(62),
	SET_SENSOR_ID(63),
	SET_SENSOR_ID(64),
	SET_SENSOR_ID(65),
	SET_SENSOR_ID(66),
	SET_SENSOR_ID(67),
	SET_SENSOR_ID(68),
	SET_SENSOR_ID(69),
	SET_SENSOR_ID(70),
	SET_SENSOR_ID(71),
	SET_SENSOR_ID(72),
	SET_SENSOR_ID(73),
	SET_SENSOR_ID(74),
	SET_SENSOR_ID(75),
	SET_SENSOR_ID(76),
	SET_SENSOR_ID(77),
	SET_SENSOR_ID(78),
	SET_SENSOR_ID(79),
	SET_SENSOR_ID(80),
	SET_SENSOR_ID(81),
	SET_SENSOR_ID(82),
	SET_SENSOR_ID(83),
	SET_SENSOR_ID(84),
	SET_SENSOR_ID(85),
	SET_SENSOR_ID(86),
	SET_SENSOR_ID(87),
	SET_SENSOR_ID(88),
	SET_SENSOR_ID(89),
	SET_SENSOR_ID(90),
	SET_SENSOR_ID(91),
	SET_SENSOR_ID(92),
	SET_SENSOR_ID(93),
	SET_SENSOR_ID(94),
	SET_SENSOR_ID(95),
	SET_SENSOR_ID(96),
	SET_SENSOR_ID(97),
	SET_SENSOR_ID(98),
	SET_SENSOR_ID(99),
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_100), /* ID: 100*/
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_101), /* ID: 101*/
	SET_SENSOR_ID(102),
	SET_SENSOR_ID(103),
	SET_SENSOR_ID(104),
	SET_SENSOR_ID(105),
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_106), /* ID: 106*/
	SET_SENSOR_ID(ROOMBA_SENSOR_GROUP_107), /* ID: 107*/
};

const size_t sensorStringSize = (sizeof(sensorStrings) / sizeof(sensorString_t));

#define SET_COMMAND_ID(c) {c, #c}

commandString_t commandStrings[] = {
	SET_COMMAND_ID(ROOMBA_START), /*128*/
	SET_COMMAND_ID(ROOMBA_BAUD),
	SET_COMMAND_ID(130),
	SET_COMMAND_ID(ROOMBA_SAFE),
	SET_COMMAND_ID(ROOMBA_FULL),
	SET_COMMAND_ID(ROOMBA_POWER),
	SET_COMMAND_ID(ROOMBA_SPOT),
	SET_COMMAND_ID(ROOMBA_CLEAN),
	SET_COMMAND_ID(136),
	SET_COMMAND_ID(ROOMBA_DRIVE),
	SET_COMMAND_ID(ROOMBA_MOTORS),
	SET_COMMAND_ID(ROOMBA_LEDS),
	SET_COMMAND_ID(ROOMBA_SONG),
	SET_COMMAND_ID(ROOMBA_PLAY),
	SET_COMMAND_ID(ROOMBA_SENSORS),
	SET_COMMAND_ID(ROOMBA_SEEK_DOCK),
	SET_COMMAND_ID(ROOMBA_MOTORS_PWM),
	SET_COMMAND_ID(ROOMBA_DRIVE_DIRECT),
	SET_COMMAND_ID(ROOMBA_DRIVE_PWM),
	SET_COMMAND_ID(147),
	SET_COMMAND_ID(ROOMBA_STREAM),
	SET_COMMAND_ID(ROOMBA_QUERY_LIST),
	SET_COMMAND_ID(ROOMBA_STREAM_PAUSE_RESUME),
	SET_COMMAND_ID(151),
	SET_COMMAND_ID(ROOMBA_SCRIPT),
	SET_COMMAND_ID(ROOMBA_SCRPTT_PLAY),
	SET_COMMAND_ID(ROOMBA_SCRIPT_SHOW),
	SET_COMMAND_ID(ROOMBA_WAIT_TIME),
	SET_COMMAND_ID(ROOMBA_WAIT_DISTANCE),
	SET_COMMAND_ID(ROOMBA_WAIT_ANGLE),
	SET_COMMAND_ID(ROOMBA_WAIT_EVENT),
	SET_COMMAND_ID(159),
	SET_COMMAND_ID(160),
	SET_COMMAND_ID(161),
	SET_COMMAND_ID(ROOMBA_LEDS_SCHEDULING),
	SET_COMMAND_ID(ROOMBA_LEDS_DIGIT_RAW),
	SET_COMMAND_ID(ROOMBA_LEDS_DIGIT_ASCII),
	SET_COMMAND_ID(ROOMBA_BUTTONS),
	SET_COMMAND_ID(166),
	SET_COMMAND_ID(ROOMBA_SCHEDULE),
	SET_COMMAND_ID(ROOMBA_SET_TIME_DATE),
	SET_COMMAND_ID(169),
	SET_COMMAND_ID(170),
	SET_COMMAND_ID(171),
	SET_COMMAND_ID(172),
	SET_COMMAND_ID(173),
	SET_COMMAND_ID(174),
	SET_COMMAND_ID(175),
	SET_COMMAND_ID(176),
	SET_COMMAND_ID(177),
	SET_COMMAND_ID(178),
	SET_COMMAND_ID(179),
	SET_COMMAND_ID(180),
	SET_COMMAND_ID(181),
	SET_COMMAND_ID(182),
	SET_COMMAND_ID(183),
	SET_COMMAND_ID(184),
	SET_COMMAND_ID(185),
	SET_COMMAND_ID(186),
	SET_COMMAND_ID(187),
	SET_COMMAND_ID(188),
	SET_COMMAND_ID(189),
	SET_COMMAND_ID(190),
	SET_COMMAND_ID(191),
	SET_COMMAND_ID(192),
	SET_COMMAND_ID(193),
	SET_COMMAND_ID(194),
	SET_COMMAND_ID(195),
	SET_COMMAND_ID(196),
	SET_COMMAND_ID(197),
	SET_COMMAND_ID(198),
	SET_COMMAND_ID(199),
	SET_COMMAND_ID(ROOMBA_EXTRA_STREAM),
	SET_COMMAND_ID(ROOMBA_EXTRA_STREAM_PAUSE_RESUME)
};

const size_t commandStringSize = (sizeof(commandStrings) / sizeof(commandString_t));


/**
 * Gibt ein SensorDaten Packet aus
 * Syntax: SensorID: %d Data: %d\n
 * @param SensorData
 */
void roomba_printSensorData(roombaSensorData_t* data){
	uint8_t sensorID = data->sensorID;
	int16_t d = data->data;
	char* string;
	if(sensorID < GET_SENSOR_STRING_SIZE()){
		string = GET_SENSOR_STRING(sensorID);
		printf("SensorID: %s(%d) Data: 0x%x = %d\n", string, (int) sensorID, (unsigned int) d, (int) d);
	}else{
		printf("Unbekannte SensorID: %d Data: 0x%x = %d\n", (int) sensorID, (unsigned int)d, (int) d);
	}
}
void roomba_printCommand(roombaCommand_t* c){
	uint8_t i;
	if(c->command < GET_COMMAND_STRING_SIZE()+ROOMBA_START){
		printf("Command: %s(%d) Size: %d Data: ", GET_COMMAND_STRING(c->command), (int) c->command, (int) c->size);
	}else{
		printf("Unbekannte Command ID: %d Size: %d Data: ", (int) c->command, (int) c->size);
	}
	for(i = 0; i < c->size; i++){
		printf("%d = 0x%x, ", (int) c->data[i], (unsigned int) c->data[i]);
	}
	printf("\n");
}