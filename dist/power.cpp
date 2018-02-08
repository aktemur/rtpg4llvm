#include "codeGenConstructors.h"
#include <iostream>
 
using namespace codeGen;

int main(int argc, const char *argv[]) {
  int power = 5; // Specialize for the fifth power
   
  ExprCode *c = $expr< 1 >$;
  for(int i=0; i < power; i++) {
    c = $expr< `expr(c`) * x >$;
  }
  // c is now the code "1*x*x*x*x*x"
 
  CompileUnit *powFunCode = $compunit<
    int pow(int x) { // capture the free variable x in c
    return `expr(c`);
  }
  >$;
   
  // Compile the function and grab a pointer
  powFunCode->compile();
  delete powFunCode; // Safe to delete after compilation
  typedef int(*powFunType)(int);
  powFunType powFunPtr = 
    (powFunType) codeGen::getFunctionPointer("pow");  
 
  std::cout << "2^5=" << powFunPtr(2) << "\n";
  std::cout << "3^5=" << powFunPtr(3) << "\n";
  std::cout << "4^5=" << powFunPtr(4) << "\n";
 
  return 0;
}
