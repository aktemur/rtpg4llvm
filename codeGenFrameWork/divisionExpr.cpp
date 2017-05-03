#include "divisionExpr.h"

using namespace codeGen;

llvm::Value* DivisionExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
   if (isSigned) {
      return Builder->CreateSDiv(lval, rval);
   } else {
      return Builder->CreateUDiv(lval, rval);
   }
}

llvm::Value* DivisionExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
   return Builder->CreateFDiv(lval, rval);
}

llvm::Value* DivisionExpr::pointerOp(llvm::Value* lval, llvm::Value* rval) 
{
   std::cerr << "Incompatible types in division." << endl;
   exit(1);
}

void DivisionExpr::printOp() {
  std::cout << " / ";
}