#!/bin/bash

cd $(dirname $0)/..

name=$(basename $1 .h)
name=$(basename $name .c)

lib_h=lib/${name}.h
lib_c=lib/${name}.c

if [ -f $lib_h ]; then
    cp -v $lib_h inc
fi

if [ -f $lib_c ]; then
    cp -v $lib_c src
fi

