#include "../codeGenConstructors.h"
#include <stdio.h>

using namespace codeGen;

int main(int argc, char* argv[]) {
  CompileUnit *p = $compunit< 
  bool testGT1() {
    return 5 > 6;
  }
  bool testGT2() {
    return 6 > 5;
  }
  bool testGT3() {
    return 5 >= 6;
  }
  bool testGT4() {
    return 5 >= 5;
  }
  
  bool testLT1() {
    return 5 < 6;
  }
  bool testLT2() {
    return 6 < 5;
  }
  bool testLT3() {
    return 5 <= 6;
  }
  bool testLT4() {
    return 5 <= 5;
  }
  
  bool testEQ1() {
    return 5 == 6;
  }
  bool testEQ2() {
    return 5 == 5;
  }
  bool testNE1() {
    return 5 != 6;
  }
  bool testNE2() {
    return 5 != 5;
  }

  >$;
  p->compile();
  bool (*fPtr)();

  fPtr = (bool (*)())codeGen::getFunctionPointer("testGT1");  
  if(fPtr() != false)
    printf("!!! FAIL !!! GT1");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testGT2");  
  if(fPtr() != true)
    printf("!!! FAIL !!! GT2");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testGT3");  
  if(fPtr() != false)
    printf("!!! FAIL !!! GT3");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testGT4");  
  if(fPtr() != true)
    printf("!!! FAIL !!! GT4");  

  fPtr = (bool (*)())codeGen::getFunctionPointer("testLT1");  
  if(fPtr() != true)
    printf("!!! FAIL !!! LT1");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testLT2");  
  if(fPtr() != false)
    printf("!!! FAIL !!! LT2");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testLT3");  
  if(fPtr() != true)
    printf("!!! FAIL !!! LT3");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testLT4");  
  if(fPtr() != true)
    printf("!!! FAIL !!! LT4");  

  fPtr = (bool (*)())codeGen::getFunctionPointer("testEQ1");  
  if(fPtr() != false)
    printf("!!! FAIL !!! EQ1");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testEQ2");  
  if(fPtr() != true)
    printf("!!! FAIL !!! EQ2");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testNE1");  
  if(fPtr() != true)
    printf("!!! FAIL !!! NE1");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testNE2");  
  if(fPtr() != false)
    printf("!!! FAIL !!! NE2");  
}
