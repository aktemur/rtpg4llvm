#include "../codeGenConstructors.h"
#include <stdio.h>

using namespace codeGen;

int main(int argc, char* argv[]) {
  int n = 42;
  double d = 55.5;
  ExprCode *exp = $expr< x + 5 >$;
  StmtCode *stmt = $stmt< f(&y); >$;
  StmtCode *twoStmts = $stmt< f(&y); y = 88; >$;
  StmtCode *withDecl = $stmt< int y = 88; >$;
  string name = "test1";
  string name2 = "generatedName";
  string nameX = "x";
  int array[4] = {1,2,3,4};
  int arraySize = 4;

  CompileUnit *funs1 = $compunit<
    int fun1() { return 27; }
    int fun2() { return 5; }
  >$;
    
  CompileUnit *funs2 = $compunit<
    `unit(funs1`)
    int fun3() { return 7; }
  >$;

  CompileUnit *p = $compunit<
  double globalArray[`int(arraySize`)] = `list(array, 4`);
  `unit(funs2`)

  void f(int *v) {
    *v = 67;
  }

  int test1() {
    return `int(n`);
  }
  
  double test2() {
    return `double(d`);
  }

  double test2b() {
    double localArray[`int(arraySize`)] = `list(array, 4`);
    return localArray[1] + globalArray[3];
  }

  int test3() {
    int x = 40;
    int y = `expr(exp`);
    return y;
  }

  int test4() {
    int y = 5;
    `stmt(stmt`);
    return y;
  }

  int test5() {
    return `name(name`)();
  }

  int test6() {
    return `name ();
  }

  int `name2 () {
    return 999;
  }

  int test7() {
    int y = 5;
    `stmt(twoStmts`);
    return y;
  }

  int test8() {
    `stmt(withDecl`);
    return y;
  }

  int test9() {
    return fun1()+fun2()+fun3();
  }

  int test10() {
    int x = 444;
    return `name(nameX`);
  }

  int test11() {
    int x[3] = {3,4,5};
    return `name(nameX`)[2];
  }

  int test12() {
    int `name(nameX`) = 12;
    return x;
  }

  >$;
  p->compile();

  int (*fPtr)(void);
  
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test1");  
  if(fPtr() != 42)
    printf("!!! FAIL !!! test1\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test3");  
  if(fPtr() != 45)
    printf("!!! FAIL !!! test3\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test4");  
  if(fPtr() != 67)
    printf("!!! FAIL !!! test4\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test5");  
  if(fPtr() != 42)
    printf("!!! FAIL !!! test5\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test6");  
  if(fPtr() != 42)
    printf("!!! FAIL !!! test6\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer(name2);  
  if(fPtr() != 999)
    printf("!!! FAIL !!! %s\n", name2.c_str());

  fPtr = (int (*)(void))codeGen::getFunctionPointer("test7");  
  if(fPtr() != 88)
    printf("!!! FAIL !!! test7\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test8");  
  if(fPtr() != 88)
    printf("!!! FAIL !!! test8\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test9");  
  if(fPtr() != 39)
    printf("!!! FAIL !!! test9\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test10");  
  if(fPtr() != 444)
    printf("!!! FAIL !!! test10\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test11");  
  if(fPtr() != 5)
    printf("!!! FAIL !!! test11\n");
  fPtr = (int (*)(void))codeGen::getFunctionPointer("test12");  
  if(fPtr() != 12)
    printf("!!! FAIL !!! test12\n");

  double (*fPtr2)(void);
  fPtr2 = (double (*)(void))codeGen::getFunctionPointer("test2");  
  if(fPtr2() != 55.5)
    printf("!!! FAIL !!! test2\n");
  fPtr2 = (double (*)(void))codeGen::getFunctionPointer("test2b");  
  if(fPtr2() != 2.0+4.0)
    printf("!!! FAIL !!! test2b\n");

}
