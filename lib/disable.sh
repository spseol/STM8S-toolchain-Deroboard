#!/bin/bash

cd $(dirname $0)/..

name=$(basename $1 .h)
name=$(basename $name .c)

inc_h=inc/${name}.h
src_c=src/${name}.c
lib_h=lib/${name}.h
lib_c=lib/${name}.c

if [ -f $lib_h -o -f $lib_c ]; then
    date=$(date +"%y%m%d%H%M%S")
    [ -f $inc_h ] && mv -v $inc_h lib/$(basename $lib_h .h)-${date}.h
    [ -f $src_c ] && mv -v $src_c lib/$(basename $lib_c .c)-${date}.c
else
    [ -f $inc_h ] && mv -v $inc_h lib
    [ -f $src_c ] && mv -v $src_c lib
fi

