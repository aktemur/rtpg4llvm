#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 '<spMVgen parameters>'" >&2
  exit 1
fi

methodName=$1

echo "*" Running spMVlib-generated code for "$methodName" for CORRECTNESS

# Run manually-generated code for performance
while read matrixName
do
    echo -n $matrixName" "
    cd ..
    mkdir -p ../../newspecs/spMVgen_"$matrixName"_"$methodName"
    ./spMVgen matrices/$matrixName splitAll "$methodName" CSRbyNZ -debug > ../../newspecs/spMVgen_"$matrixName"_"$methodName"/myoutput2.txt
    cd ../../newspecs/spMVgen_"$matrixName"_"$methodName"/
    ../"$matrixName"_"$methodName"/clang2.out 1 > clangoutput2.txt
    diff myoutput2.txt clangoutput2.txt
    cd - > /dev/null
    cd tools
done < matrixNames.txt

echo " "