#!/bin/bash

methodName=$1

echo "*" Compiling python-generated code for "$methodName"
while read matrixName
do
    echo -n $matrixName" "
    cd ../../../newspecs/"$matrixName"_"$methodName"
    cp ../makefile .
    cp ../main.c ./main.c
    rm -f a.out
    make >& /dev/null
    mv a.out clang.out
    rm *.o
    cp ../main2.c ./main.c
    make >& /dev/null
    mv a.out clang2.out
    cd - > /dev/null
done < matrixNames.txt

echo " "