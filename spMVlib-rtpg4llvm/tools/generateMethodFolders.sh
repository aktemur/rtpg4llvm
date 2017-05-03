#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 '<method name>'" >&2
  exit 1
fi

methodName=$1

echo "*" Generating folders for "$methodName"
while read matrixName
do
    echo -n $matrixName" "
    cd ../../../newspecs
    rm -rf "$matrixName"_"$methodName"
    python specialize.py "$matrixName" $methodName
    cd - > /dev/null
done < matrixNames.txt

echo " "
