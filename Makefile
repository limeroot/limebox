# 	limebox Makefile
#
# 	Created on 11/05/2013 17:37:12
#
# 	This file is part of the limebox command for GNU/Linux LimeRoot
#
# 	Copyright (c) LimeRoot, http://www.limeroot.org, devel@limeroot.org
#
# 	Author: Daniel R. Torres Ortiz, daniel@limeroot.org
#
# 	This program is free software: you can redistribute it and/or modify
# 	it under the terms of the GNU General Public License as published by
# 	the Free Software Foundation, either version 3 of the License, or
# 	(at your option) any later version.
#
# 	This program is distributed in the hope that it will be useful,
# 	but WITHOUT ANY WARRANTY; without even the implied warranty of
# 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# 	GNU General Public License for more details.
#
# 	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

OBJS = main.o wan.o options.o optionable.o database.o sqlite3.o printable.o jsoncpp.o interface.o wan_static.o system.o wan_connection.o ipaddress.o
CC = g++
DEBUG = -ggdb
CFLAGS = -Wall -c $(DEBUG) -std=c++0x -m32 
LFLAGS = -Wall $(DEBUG) -std=c++0x -m32 

limebox : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o limebox -ldl -lpthread

main.o : src/main.cpp src/wan.h
	$(CC) $(CFLAGS) src/main.cpp

wan.o : src/wan.h src/wan.cpp src/optionable.h src/printable.h src/ipaddress.h src/system.h src/interface.h src/wan_static.h src/database.h
	$(CC) $(CFLAGS) src/wan.cpp

options.o : src/options.h src/options.cpp
	$(CC) $(CFLAGS) src/options.cpp

optionable.o : src/optionable.h src/optionable.cpp src/options.h
	$(CC) $(CFLAGS) src/optionable.cpp
	
database.o : src/database.h src/database.cpp src/sqlite3.h
	$(CC) $(CFLAGS) src/database.cpp

sqlite3.o : src/sqlite3.h src/sqlite3.c
	gcc -Wall -c -m32 src/sqlite3.c
#-ldl -lpthread

printable.o : src/printable.h src/printable.cpp src/json.h
	$(CC) $(CFLAGS) src/printable.cpp

jsoncpp.o : src/json.h src/jsoncpp.cpp
	$(CC) $(CFLAGS) src/jsoncpp.cpp

interface.o : src/interface.h src/interface.cpp src/optionable.h src/options.h src/printable.h src/system.h
	$(CC) $(CFLAGS) src/interface.cpp

wan_static.o : src/wan_static.h src/wan_static.cpp
	$(CC) $(CFLAGS) src/wan_static.cpp

wan_connection.o : src/wan_connection.h src/wan_connection.cpp src/options.h src/database.h src/system.h src/interface.h
	$(CC) $(CFLAGS) src/wan_connection.cpp

system.o : src/system.h src/system.cpp
	$(CC) $(CFLAGS) src/system.cpp

ipaddress.o : src/ipaddress.h src/ipaddress.cpp src/options.h src/json.h
	$(CC) $(CFLAGS) src/ipaddress.cpp


clean :
	rm -rf *.o limebox lr.db
    
    
    