#!/bin/bash

methodName=$1

echo "*" Running python-generated code for $methodName
while read matrixName
do
    echo -n $matrixName" "
    cd ../../../newspecs/"$matrixName"_"$methodName"
    rm -f clangoutput.tx
    ./clang.out 10000 | grep microsec | awk '{printf "%d ", $3}' >> clangoutput.txt 
    ./clang.out 10000 | grep microsec | awk '{printf "%d ", $3}' >> clangoutput.txt 
    ./clang.out 10000 | grep microsec | awk '{printf "%d ", $3}' >> clangoutput.txt 
    ./clang.out 10000 | grep microsec | awk '{printf "%d ", $3}' >> clangoutput.txt 
    ./clang.out 10000 | grep microsec | awk '{printf "%d\n", $3}' >> clangoutput.txt 
    cd - > /dev/null
done < matrixNames.txt

echo " "
