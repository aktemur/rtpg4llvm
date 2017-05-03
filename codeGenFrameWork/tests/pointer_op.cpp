#include "../codeGenConstructors.h"
#include <stdio.h>

using namespace codeGen;

int main(int argc, char* argv[]) {
  CompileUnit *p = $compunit< 
  int testArray1(int* x) {
    return x[2];
  }
  int testArray2(int* x) {
    int y = x[2];
    x[2] = 50;
    return x[2] + y;
  }
  int testArray3(int* x) {
    int *y = &x[1];
    return *y;
  }
  int testPtr1(int* x) {
    *x = 50;
    return *x;
  }
  int testPtr2(int x) {
    int *px = &x;
    *px = 50;
    return x + *px;
  }
  int testPtr3(int *x) {
    int **px = &x;
    **px = 60;
    return **px;
  }
  int testPtr4(int *x) {
    return x[x[2]];
  }
  >$;
  p->compile();

  int (*fPtr)(int*);
  int x = 5;
  int array[] = {1,2,3,4,5};
  int array2[] = {1,2,3,4,5};
  int *px = &x;
  int *px3 = &x;
  
  fPtr = (int (*)(int*))codeGen::getFunctionPointer("testArray1");  
  if(fPtr(array) != 3)
    printf("!!! FAIL !!! Array1\n");  
  fPtr = (int (*)(int*))codeGen::getFunctionPointer("testArray2");  
  if(fPtr(array) != 53 || array[2] != 50)
    printf("!!! FAIL !!! Array2\n");  
  fPtr = (int (*)(int*))codeGen::getFunctionPointer("testArray3");  
  if(fPtr(array) != 2)
    printf("!!! FAIL !!! Array3\n");  
  fPtr = (int (*)(int*))codeGen::getFunctionPointer("testPtr1");  
  if(fPtr(px) != 50 || x != 50 || *px != 50)
    printf("!!! FAIL !!! Ptr1\n");  
  fPtr = (int (*)(int*))codeGen::getFunctionPointer("testPtr3");  
  if(fPtr(px) != 60 || x != 60)
    printf("!!! FAIL !!! Ptr3\n");  
  fPtr = (int (*)(int*))codeGen::getFunctionPointer("testPtr4");  
  if(fPtr(array2) != 4)
    printf("!!! FAIL !!! Ptr4: %d\n", fPtr(array2));  

  int (*fPtr2)(int);
  fPtr2 = (int (*)(int))codeGen::getFunctionPointer("testPtr2");  
  if(fPtr2(5) != 100)
    printf("!!! FAIL !!! Ptr2\n");  
}
