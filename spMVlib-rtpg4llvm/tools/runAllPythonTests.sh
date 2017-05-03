#!/bin/bash

testMethod() {
    local methodName=$1
    ./generateMethodFolders.sh "$methodName"
    ./compilePythonGeneratedCodes.sh "$methodName"
    ./runPythonGeneratedCodesForSpeed.sh "$methodName"
    ./makeCSVForPythonGeneratedCodes.sh "$methodName" > $HOSTNAME."$methodName".csv
}

testOski() {
    methodName="split_by_block_$1_$2_OSKI_$1_$2_unfolding"

    echo "*" Generating folders for "$methodName"
    while read matrixName
    do
	echo -n $matrixName" "
	cd ../../../newspecs
	rm -rf "$matrixName"_"$methodName"
	python specialize.py "$matrixName" "[split_by_block, $1, $2, [OSKI, $1, $2], [unfolding]]"
	cd - > /dev/null
    done < matrixNames.txt
    echo " "

    ./compilePythonGeneratedCodes.sh "$methodName"
    ./runPythonGeneratedCodesForSpeed.sh "$methodName"
    ./makeCSVForPythonGeneratedCodes.sh "$methodName" > $HOSTNAME."$methodName".csv
}

testUnrolling() {
    methodName="unrolling_$1"

    echo "*" Generating folders for "$methodName"
    while read matrixName
    do
	echo -n $matrixName" "
	cd ../../../newspecs
	rm -rf "$matrixName"_"$methodName"
	python specialize.py "$matrixName" "[unrolling, $1]"
	cd - > /dev/null
    done < matrixNames.txt
    echo " "

    ./compilePythonGeneratedCodes.sh "$methodName"
    ./runPythonGeneratedCodesForSpeed.sh "$methodName"
    ./makeCSVForPythonGeneratedCodes.sh "$methodName" > $HOSTNAME."$methodName".csv
}

testMethod CSRbyNZ
testMethod stencil
testMethod unfolding
testMethod unfolding2

testOski 1 2
testOski 2 1
testOski 2 2

testUnrolling 2
testUnrolling 3
testUnrolling 4
testUnrolling 5

rm -rf "$HOSTNAME"_clang_all
mkdir "$HOSTNAME"_clang_all
$(mv "$HOSTNAME."*csv "$HOSTNAME"_clang_all)
