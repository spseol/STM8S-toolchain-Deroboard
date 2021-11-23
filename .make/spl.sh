#!/bin/bash

cd ..
curl https://mamut.spseol.cz/nozka/public/spl.tgz -O spl.tgz

if ! [ -d SPL ]; then
    tar xzf spl.tgz SPL
fi

if ! [ -d SPL-STM8S103 ]; then
    tar xzf spl.tgz SPL-STM8S103
fi
if ! [ -d SPL-STM8S105 ]; then
    tar xzf spl.tgz SPL-STM8S105
fi
if ! [ -d SPL-STM8S208 ]; then
    tar xzf spl.tgz SPL-STM8S208
fi

rm spl.tgz