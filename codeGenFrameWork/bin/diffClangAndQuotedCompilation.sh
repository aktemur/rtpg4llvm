#!/bin/bash

# Usage: ./diffClangAndQuotedCompilation.sh <opt-level> <file-name-without-extension>
# E.g:   ./diffClangAndQuotedCompilation.sh 1 spMVcsr

clang -O0 -S -emit-llvm $2.c -o $2.ll
opt -O$1 -S -o $2_clang_aligned.ll $2.ll
sed 's/, align [0-9][0-9]*//g' $2_clang_aligned.ll > $2_clang.ll

./quoteAndCompileFiles.sh $1 $2.c 2> $2_ours_aligned.ll
sed 's/, align [0-9][0-9]*//g' $2_ours_aligned.ll > $2_ours.ll

diff $2_ours.ll $2_clang.ll

