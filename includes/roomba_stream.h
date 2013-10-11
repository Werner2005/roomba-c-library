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
#ifndef ROOMBA_STREAM_H
#define ROOMBA_STREAM_H
#include <roomba_def.h>
#include <roomba_sensor_database.h>
/**
 * @defgroup roomba_stream Roomba Stream Libary(deprecated)
 * 
 * @deprecated Unbenannt in {@link roomba_sensor_database Roomba Sensor Database}
 * @{
 */

/**
 * @see roomba_sensor_database_init()
 * @deprecated Umbenannt in {@link roomba_sensor_database_init()}
 */
#define roomba_stream_init() roomba_sensor_database_init()
/**
 * @see roomba_sensor_database_receive()
 * @deprecated Umbenannt in {@link roomba_sensor_database_receive()}
 */
#define roomba_stream_receive(data) roomba_sensor_database_receive(data)
/**
 * @see roomba_sensor_database_get()
 * @deprecated Umbenannt in {@link roomba_sensor_database_get()}
 */
#define roomba_stream_get(sensorID, result) roomba_sensor_database_get(sensorID, result)
/* @} */

#endif
