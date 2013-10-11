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
#include "unit.h"
#include <stdlib.h>
#include <stdio.h>
#include <roomba.h>
/**
 * Buffer fuer geschriebe Zeichen
 */
volatile unsigned char buffer[256];
/**
 * Zahler um zu pruefen wie viele Zeichen geschrieben worden sind
 */
volatile uint8_t counter = 0;
/**
 * Bestimmt ob ein Schreibfehler auftreten soll
 */
volatile bool writeError = false;
/**
 * Roomba Simulation Callback
 * @param c Zeichen das geschieben werden soll
 * @return < 0 Fehler
 */
int8_t (*roomba_read_sim)(uint8_t c);
int (*write_sim)(unsigned char c) = NULL;

/**
 * Write Stub
 * 
 * Simuliert Schreiben auf RS232
 * 
 * Speichert alle geschreieben Zeichen im buffer
 * @param c zeichen das geschrieben werden soll
 * @return Simuliert Schreibfehler wenn writeError auf true
 */
int test_write(unsigned char c){
	printf("%u\n", c);
	buffer[counter++] = c;
	if(!writeError){
		if(write_sim != NULL){
			return write_sim(c);
		}
		return 0;
	}else{
		return -1;
	}
}
/**
 * Funktion muss vor jedem Test aufgerufen werden
 * Loescht wirte buffer und setzt counter = 0
 */
void beforTest(){
	int i;
	printf("---------------\nTest inizalisiert\n");
	counter = 0;
	writeError = false;
	for(i = 0; i < 256; i++){
		buffer[i] = 0;
	}
	/*
	 * Error Reseten
	 */
	roomba_getError();
	roomba_reset();
}
