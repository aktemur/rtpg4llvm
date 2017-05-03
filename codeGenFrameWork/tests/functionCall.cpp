#include "../codeGenConstructors.h"
#include <stdio.h>

using namespace codeGen;

int main(int argc, char* argv[]) {
  CompileUnit *p = $compunit< 
  int f1() {
    return 5;
  }
  int f2(int x) {
    return x*2;
  }
  int f3(int x, int y, int z) {
    return x+y+z;
  }
  
  int testFC1() {
    return f1();
  }
  int testFC2() {
    return f2(5);
  }
  int testFC3() {
    return f3(1, 2, 3);
  }
  >$;
  p->compile();

  int (*fPtr)();
  fPtr = (int (*)())codeGen::getFunctionPointer("testFC1");  
  if(fPtr() != 5)
    printf("!!! FAIL !!! FC1");  
  fPtr = (int (*)())codeGen::getFunctionPointer("testFC2");  
  if(fPtr() != 10)
    printf("!!! FAIL !!! FC2");  
  fPtr = (int (*)())codeGen::getFunctionPointer("testFC3");  
  if(fPtr() != 6)
    printf("!!! FAIL !!! FC3");  
}
