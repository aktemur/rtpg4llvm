#include "../codeGenConstructors.h"
#include <stdio.h>

using namespace codeGen;

int main(int argc, char* argv[]) {
  CompileUnit *p = $compunit< 
  bool testAND1() {
    return true && true;
  }
  bool testAND2() {
    return true && false;
  }
  bool testAND3() {
    return false && false;
  }
  bool testOR1() {
    return true || true;
  }
  bool testOR2() {
    return true || false;
  }
  bool testOR3() {
    return false || false;
  }
  bool testNOT1() {
    return !false;
  }
  bool testNOT2() {
    return !true;
  }
  >$;
  p->compile();
  bool (*fPtr)();

  fPtr = (bool (*)())codeGen::getFunctionPointer("testAND1");  
  if(fPtr() != true)
    printf("!!! FAIL !!! AND1");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testAND2");  
  if(fPtr() != false)
    printf("!!! FAIL !!! AND2");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testAND3");  
  if(fPtr() != false)
    printf("!!! FAIL !!! AND3");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testOR1");  
  if(fPtr() != true)
    printf("!!! FAIL !!! OR1");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testOR2");  
  if(fPtr() != true)
    printf("!!! FAIL !!! OR2");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testOR3");  
  if(fPtr() != false)
    printf("!!! FAIL !!! OR3");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testNOT1");  
  if(fPtr() != true)
    printf("!!! FAIL !!! NOT1");  
  fPtr = (bool (*)())codeGen::getFunctionPointer("testNOT2");  
  if(fPtr() != false)
    printf("!!! FAIL !!! NOT2");  
}
