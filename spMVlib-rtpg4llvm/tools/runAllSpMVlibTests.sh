#!/bin/bash

./runSpMVlibTests.sh "CSRbyNZ"
./runSpMVlibTests.sh "stencil"
./runSpMVlibTests.sh "unfolding"

rm -rf "$HOSTNAME"_spMVlib_all
mkdir "$HOSTNAME"_spMVlib_all
$(mv "$HOSTNAME".spMVgen.*csv "$HOSTNAME"_spMVlib_all)
