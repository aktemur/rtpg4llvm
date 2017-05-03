#include "../codeGenConstructors.h"
#include <stdio.h>

using namespace codeGen;

int main(int argc, char* argv[]) {
  CompileUnit *p = $compunit< 
  int testVar1(int x) {
    return x;
  }
  int testVar2(int x) {
    return -x;
  }
  int testVar3(int x) {
    int y = 5;
    return x * y;
  }
  double testVar4(int x) {
    double y = 2.2;
    return x * y;
  }
  int testVar5(int x) {
    int y = x;
    x = 50;
    return x + y;
  }
  double testVar6(int x) {
    double y;
    y = x + 0.5;
    return y;
  }
  double testVar7(int x) {
    double y = 2; // must promote int to double
    return y;
  }
  >$;
  p->compile();

  int (*fPtr)(int);
  fPtr = (int (*)(int))codeGen::getFunctionPointer("testVar1");  
  if(fPtr(5) != 5)
    printf("!!! FAIL !!! Var1");  
  fPtr = (int (*)(int))codeGen::getFunctionPointer("testVar2");  
  if(fPtr(5) != -5)
    printf("!!! FAIL !!! Var2");  
  fPtr = (int (*)(int))codeGen::getFunctionPointer("testVar3");  
  if(fPtr(5) != 25)
    printf("!!! FAIL !!! Var3");  
  fPtr = (int (*)(int))codeGen::getFunctionPointer("testVar5");  
  if(fPtr(5) != 55)
    printf("!!! FAIL !!! Var5");  

  double (*fPtr2)(int);
  fPtr2 = (double (*)(int))codeGen::getFunctionPointer("testVar4");  
  if(fPtr2(4) != 8.8)
    printf("!!! FAIL !!! Var4");  
  fPtr2 = (double (*)(int))codeGen::getFunctionPointer("testVar6");  
  if(fPtr2(5) != 5 + 0.5)
    printf("!!! FAIL !!! Var6: %f", fPtr2(5));  
  fPtr2 = (double (*)(int))codeGen::getFunctionPointer("testVar7");  
  if(fPtr2(5) != 2.0)
    printf("!!! FAIL !!! Var7: %f", fPtr2(5));  
}
