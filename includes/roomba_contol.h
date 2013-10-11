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
#ifndef ROOMBA_CONTOL_H
#define ROOMBA_CONTOL_H
/**
 * @defgroup roomba_contol Roomba Contol
 * 
 * Dieses Modul definerit IDs für das Roomba Contol System
 * 
 * Das Roomba Contoll System ist für die erweitere Kontolle des Sensorknoten gedacht
 * Der Sensorknoten sendet über die Contol Nachrichten Aufgetrete Fehler
 * @{
 */
/**
 * Fehler auf dem Sensorknoten
 * 
 * Im Data Teil ist der Fehler enthalten
 */
#define ROOMBA_CONTOLID_ERROR 0x01
/** @}*/
#endif
