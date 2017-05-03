#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <methodName>" >&2
  exit 1
fi

methodName=$1

while read matrixName
do
    echo -n $matrixName", "
    folderName="$matrixName"_"$methodName"
    values=`cat ../../../newspecs/$folderName/clangoutput.txt`
    ./findMinTiming.py $values
done < matrixNames.txt

