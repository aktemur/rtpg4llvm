#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 '<spMVgen parameters>'" >&2
  exit 1
fi

methodName=$1

test() {
    local matrixName=$1
    local methodName=$2
    rm -f result.txt
    ./spMVgen matrices/$matrixName splitAll $methodName CSRbyNZ > result.txt
    local genCost=`grep codeGeneration result.txt | awk '{print $2}'`
    local runningTime=`grep multByM result.txt    | awk '{print $2}'`

    echo -n $genCost" "     >> ../../newspecs/spMVgen_"$matrixName"_"$methodName"/genCost.txt
    echo -n $runningTime" " >> ../../newspecs/spMVgen_"$matrixName"_"$methodName"/runtime.txt
}

echo "*" Running spMVlib for "$methodName"

while read matrixName
do
    echo -n $matrixName" "
    mkdir -p ../../../newspecs/spMVgen_"$matrixName"_"$methodName"
    cd ..
    rm -f ../../newspecs/spMVgen_"$matrixName"_"$methodName"/genCost.txt
    rm -f ../../newspecs/spMVgen_"$matrixName"_"$methodName"/runtime.txt
    test $matrixName "$methodName"
    test $matrixName "$methodName"
    test $matrixName "$methodName"
    test $matrixName "$methodName"
    test $matrixName "$methodName"
    cd - > /dev/null
done < matrixNames.txt

echo " "

findMins() {
    local fileName=$1
    rm -f $HOSTNAME.spMVgen.$fileName."$methodName".csv 
    while read matrixName
    do
	cd ../../../newspecs/spMVgen_"$matrixName"_"$methodName"
	values=`cat $fileName.txt`
	cd - > /dev/null
	echo -n $matrixName", "  >> $HOSTNAME.spMVgen.$fileName."$methodName".csv 
	./findMinTiming.py $values >> $HOSTNAME.spMVgen.$fileName."$methodName".csv 
    done < matrixNames.txt
}

findMins runtime
findMins genCost
