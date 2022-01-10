#!/bin/bash

SPLS=(SPL SPL-STM8S103 SPL-STM8S105 SPL-STM8S208)

#####################################################################################

any_dir_not_exists() {
    for dir in $@; do
        if ! [ -d $dir ]; then
            return 0
        fi
    done
    return 1
}

#####################################################################################

cd ..

if any_dir_not_exists ${SPLS[@]} ; then

    curl https://mamut.spseol.cz/nozka/public/spl.tgz -O spl.tgz

    for dir in ${SPLS[@]}; do
        if ! [ -d $dir ]; then
            echo Rozbaluju $dir
            tar xzf spl.tgz $dir
        fi
    done

    rm  spl.tgz
else
    echo "Vše je připraveno"
fi
