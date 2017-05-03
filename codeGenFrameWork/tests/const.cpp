#include "../codeGenConstructors.h"
#include <stdio.h>

using namespace codeGen;

int main(int argc, char* argv[]) {
  CompileUnit *p = $compunit< 
  double testDouble() {
    return 2.2;
  }
  int testInt() {
    return 5;
  }
  bool testTrue() {
    return true;
  }
  bool testFalse() {
    return false;
  }
  >$;
  p->compile();

  double (*fPtr1)() = (double (*)())codeGen::getFunctionPointer("testDouble");
  if(fPtr1() != 2.2)
    printf("!!! FAIL !!! testDouble\n");  
  int (*fPtr2)() = (int (*)())codeGen::getFunctionPointer("testInt");
  if(fPtr2() != 5)
    printf("!!! FAIL !!! testInt\n");  
  bool (*fPtr3)() = (bool (*)())codeGen::getFunctionPointer("testTrue");
  if(fPtr3() != true)
    printf("!!! FAIL !!! testTrue\n");  
  bool (*fPtr4)() = (bool (*)())codeGen::getFunctionPointer("testFalse");
  if(fPtr4() != false)
    printf("!!! FAIL !!! testFalse\n");  
}
