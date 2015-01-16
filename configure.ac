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
AC_INIT([Roomba-C-Library], [1.0], [webmaster@andy89.org])
AC_PREREQ([2.67])
AC_CONFIG_MACRO_DIR([m4])
AC_REVISION([Version 1.0 11.10.2013])
AC_COPYRIGHT([Copyright (c) by Andreas Werner <webmaster@andy89.org> Version 1.0 (11.10.2013)])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AS_VAR_APPEND(CFLAGS, " -Wall -Werror -ansi -pedantic -D_BSD_SOURCE -D_POSIX_C_SOURCE=199506L ")
AC_SUBST(CFLAGS)

AC_CONFIG_SRCDIR([src])
AC_CONFIG_SRCDIR([src/roomba.c])
AC_CONFIG_SRCDIR([src/roomba_command.c])
AC_CONFIG_SRCDIR([src/roomba_extra.c])
AC_CONFIG_SRCDIR([src/roomba_repeat.c])
AC_CONFIG_SRCDIR([src/roomba_sensor_database.c])
AC_CONFIG_SRCDIR([includes])
AC_CONFIG_SRCDIR([includes/roomba.h])
AC_CONFIG_SRCDIR([includes/roomba_command.h])
AC_CONFIG_SRCDIR([includes/roomba_contol.h])
AC_CONFIG_SRCDIR([includes/roomba_def.h])
AC_CONFIG_SRCDIR([includes/roomba_extra.h])
AC_CONFIG_SRCDIR([includes/roomba_print.h])
AC_CONFIG_SRCDIR([includes/roomba_repeat.h])
AC_CONFIG_SRCDIR([includes/roomba_sensor_database.h])
AC_CONFIG_SRCDIR([includes/roomba_stream.h])
AC_CONFIG_SRCDIR([test])
AC_CONFIG_SRCDIR([test/Makefile])
AC_CONFIG_SRCDIR([test/roombaCommandTest.c])
AC_CONFIG_SRCDIR([test/roombaCommandTest.h])
AC_CONFIG_SRCDIR([test/roombaExtraTest.c])
AC_CONFIG_SRCDIR([test/roombaExtraTest.h])
AC_CONFIG_SRCDIR([test/roombaLocalTest.c])
AC_CONFIG_SRCDIR([test/roombaRepeatTest.c])
AC_CONFIG_SRCDIR([test/roombaRepeatTest.h])
AC_CONFIG_SRCDIR([test/roombaStreamTest.c])
AC_CONFIG_SRCDIR([test/roombaStreamTest.h])
AC_CONFIG_SRCDIR([test/roombaTest.c])
AC_CONFIG_SRCDIR([test/roombaTest.h])
AC_CONFIG_SRCDIR([test/unit.c])
AC_CONFIG_SRCDIR([test/unit.h])

AC_CANONICAL_HOST

AC_CHECK_HEADER([stdint.h],,AC_MSG_ERROR([stdint.h not found]))
AC_CHECK_HEADER([string.h],,AC_MSG_ERROR([string.h not found]))
AC_CHECK_HEADER([stdio.h],,AC_MSG_ERROR([stdio.h not found]))
AC_CHECK_HEADER([stdlib.h],,AC_MSG_ERROR([stdlib.h not found]))
AC_CHECK_HEADER([stdarg.h],,AC_MSG_ERROR([stdarg.h not found]))


AC_PROG_CC
AM_PROG_AR
AM_PROG_CC_C_O

#init LIBTOOL
LT_INIT()

# Read soll so kurtz wie moeglich sein
# Daten Werden beim erst beim Abfragen verarbeitet und in den Internen Buffer Uebertragen
AC_ARG_ENABLE([short-read],AS_HELP_STRING([--enable-short-read],[Activated char buffer and Short interrupt processing time]), AC_DEFINE([ROOMBA_SHORT_READ]),)

# Debug Ausgaben Aktivieren
AC_ARG_ENABLE([debug], AS_HELP_STRING([--enable-debug], [Activate Debuging]), [
AC_DEFINE(ROOMBA_DEBUG)
AS_VAR_APPEND(CFLAGS, " -g")
], AS_VAR_APPEND(CFLAGS, " -Os")) 

# Libary im Nonblocking Modus bauen
AC_ARG_ENABLE([nonblocking], AS_HELP_STRING([--enable-nonblocking], [Activiert nonblocking mode when sending a command]), AC_DEFINE(ROOMBA_NONBLOCKING),)

AC_ARG_VAR([BUFFER_SIZE], [Setting RX Buffer Size[default: 173 Sensor Packets]])
AS_IF([test "x$ac_cv_env_BUFFER_SIZE_set" == "xset"],[
	AC_DEFINE_UNQUOTED(ROOMBA_SENSOR_BUFFER_SIZE, [$ac_cv_env_BUFFER_SIZE_value])
])

AC_ARG_VAR([STREAM_IGNORE], [Sensor stream setting: How many packets should be ignored for buffer overflow.[default: 50 Stream Packets]])
AS_IF([test "x$ac_cv_env_STREAM_IGNORE_set" == "xset"],[
	AC_DEFINE_UNQUOTED(ROOMBA_STREAM_IGNORE, [$ac_cv_env_STREAM_IGNORE_value])
])

AC_CONFIG_FILES([
Makefile
])

AC_OUTPUT
