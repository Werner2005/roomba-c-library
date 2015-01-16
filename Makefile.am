#Copyright (c) 2013 Andreas Werner
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in
#all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#THE SOFTWARE.
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = test 
include_HEADERS = includes/roomba.h \
	includes/roomba_command.h \
	includes/roomba_contol.h \
	includes/roomba_def.h \
	includes/roomba_extra.h \
	includes/roomba_print.h \
	includes/roomba_repeat.h \
	includes/roomba_sensor_database.h \
	includes/roomba_stream.h
lib_LTLIBRARIES = libroomba.la
libroomba_la_SOURCES = src/roomba.c src/roomba_command.c src/roomba_extra.c src/roomba_print.c src/roomba_repeat.c src/roomba_sensor_database.c
libroomba_la_CFLAGS = -I$(srcdir)/includes
libroomba_la_LDFLAGS = -version-info 1:0:0
