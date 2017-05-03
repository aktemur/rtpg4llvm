#include "logicalAndExpr.h"

using namespace codeGen;

llvm::Value* LogicalAndExpr::integerOp(llvm::Value* lval, llvm::Value* rval){
   llvm::Value* cmpLval = Builder->CreateIsNotNull(lval);
   llvm::Value* cmpRval = Builder->CreateIsNotNull(rval);
   
   return Builder->CreateAnd(cmpLval, cmpRval);
}

void LogicalAndExpr::printOp() {
  std::cout << " && ";
}