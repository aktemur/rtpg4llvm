#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 '<spMVlib params>'" >&2
  exit 1
fi

methodName=$1

while read matrixName
do
    echo -n $matrixName", "
    cd ../../../newspecs/spMVgen_"$matrixName"_"$methodName"
    values=`cat myoutput.txt`
    cd - > /dev/null
    ./findMinTiming.py $values
done < matrixNames.txt

