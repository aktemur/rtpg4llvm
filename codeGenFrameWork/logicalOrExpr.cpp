#include "logicalOrExpr.h"

using namespace codeGen;

llvm::Value* LogicalOrExpr::integerOp(llvm::Value* lval, llvm::Value* rval){
   llvm::Value* cmpLval = Builder->CreateIsNotNull(lval);
   llvm::Value* cmpRval = Builder->CreateIsNotNull(rval);
   
   return Builder->CreateOr(cmpLval, cmpRval);
}

void LogicalOrExpr::printOp() {
  std::cout << " || ";
}