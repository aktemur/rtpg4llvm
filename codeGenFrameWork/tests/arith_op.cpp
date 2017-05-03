#include "../codeGenConstructors.h"
#include <stdio.h>

using namespace codeGen;

int main(int argc, char* argv[]) {
  CompileUnit *p = $compunit< 
  int test1() {
    return 1 + 2 - 3 * 40 / 5 % 5;
  }
  int test2() {
    int x = 5;
    int y = x++;
    return 10*x+y;
  }
  int test3() {
    int x = 5;
    int y = x--;
    return 10*x+y;
  }
  int test4() {
    int x = 5;
    int y = ++x;
    return 10*x+y;
  }
  int test5() {
    int x = 5;
    int y = --x;
    return 10*x+y;
  }
  >$;
  p->compile();
  int (*fPtr)();
  fPtr = (int (*)())codeGen::getFunctionPointer("test1");
  if(fPtr() != -1)
    printf("!!! FAIL Test 1!!!\n");  
  fPtr = (int (*)())codeGen::getFunctionPointer("test2");
  if(fPtr() != 65)
    printf("!!! FAIL Test 1!!!\n");  
  fPtr = (int (*)())codeGen::getFunctionPointer("test3");
  if(fPtr() != 45)
    printf("!!! FAIL Test 1!!!\n");  
  fPtr = (int (*)())codeGen::getFunctionPointer("test4");
  if(fPtr() != 66)
    printf("!!! FAIL Test 1!!!\n");  
  fPtr = (int (*)())codeGen::getFunctionPointer("test5");
  if(fPtr() != 44)
    printf("!!! FAIL Test 1!!!\n");  
}
