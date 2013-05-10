#!/bin/bash

#/*
# *  Created on 09/04/2013 14:31:11
# *  
# *  This file is part of the LR command for GNU/Linux LimeRoot
# *
# *  Copyright (c) LimeRoot, http://www.limeroot.org, devel@limeroot.org
# *
# *  Author: Daniel R. Torres Ortiz, daniel@limeroot.org
# *
# *  This program is free software: you can redistribute it and/or modify
# *  it under the terms of the GNU General Public License as published by
# *  the Free Software Foundation, either version 3 of the License, or
# *  (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License
# *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
# *
# */



rm -rf lr

ARCH=$1
DEBUG="-ggdb"
FILE=$2
clear

if [[ -z $FILE ]]; then
    source_code=$(find . -type f \( -iname "*.c" -or -iname "*.cpp" -or -iname "*.c" -or -iname "*.h" \) -printf '%T@ %p\n' | sort -n | tail -1 | cut -f2- -d" " | sed 's/\.\///')
else
   source_code=$FILE".cpp" 
fi
source_code=${source_code%.*}

#if [ $source_code == "build" ]; then
#    echo "All up to date, nothing to build"
#    exit;
#fi

echo "Building lr"
    
object=$(echo ${source_code})

object=$(basename $object)

CC="g++ -std=c++0x -Wall -Werror ${DEBUG}"

EX="cpp"

echo ${source_code}

if [[ "${source_code}" == "sqlite3" ]]; then
    CC="gcc"
    EX="c"
fi

echo "  -${source_code}"

${CC} ${ARCH} -fpic -c  ${source_code}.${EX} -o objects/${object}.o -I $(pwd)

if [[ "${?}" != "0" ]]; then
    echo ""
    exit
fi

OBJ=$(ls objects/*.o)

g++ ${ARCH} -Wall -Werror ${DEBUG} -o lr ${OBJ}  /usr/local/lib/libboost_system.a  -ldl -lpthread -I $(pwd)

#scp lr root@192.168.0.10:/

echo ""