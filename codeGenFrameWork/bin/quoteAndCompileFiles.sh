#!/bin/bash

# Usage: ./diffClangAndQuotedCompilation.sh <opt-level> <file-name>
# E.g:   ./diffClangAndQuotedCompilation.sh 1 spMVcsr.c

echo '#include "../codeGenConstructors.h"

using namespace codeGen;

int main(int argc, char* argv[]) {
  CompileUnit *p = $compunit<
' > __temp.cpp
cat $2 >> __temp.cpp
echo "
  >$;
  p->compileAndDump($1);
}
" >> __temp.cpp

export CLASSPATH=$CLASSPATH:../../compiler/
python ../../compiler/preprocess.py __temp > /dev/null

clang++ -O0 `llvm-config --libs core engine native jit ipo instrumentation bitreader` `llvm-config --ldflags` `llvm-config --cppflags` -o __temp.out ../build/*.o __temp-pp.cpp
chmod 766 __temp.out
./__temp.out
rm __temp.out
