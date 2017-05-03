#include "funcCallExpr.h"
#include <vector>

using namespace codeGen;

FuncCallExpr::~FuncCallExpr() {
  for (typeof(args->begin())s = args->begin(); s != args->end(); ++s) {
    delete (*s);    
  }
  delete args;
}

cval FuncCallExpr::build() {
   cval ret;
   llvm::Function *callee = TheModule->getFunction(funcName);
   
   if(callee == NULL) {
      std::cerr << "Function " << funcName << " not found.\n";
      exit(1);
   }
  
   vector<llvm::Value*> argumans;
   for (typeof(args->begin())s = args->begin(); s != args->end(); ++s) {
      argumans.push_back((*s)->build().val);    
   }
   
   ret.val = Builder->CreateCall(callee, argumans);
   
   return ret;
}

void FuncCallExpr::print() {
  std::cout << funcName << "(";
  typeof(args->begin()) argIt = args->begin(), argItEnd = args->end();
  while(true) {
    (*argIt)->print();
    ++argIt;
    if(argIt != argItEnd) 
      std::cout << ", ";
    else
      break;
  }
  std::cout << ")";  
}