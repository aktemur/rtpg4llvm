#include "notEqualExpr.h"

using namespace codeGen;

llvm::Value* NotEqualExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
   cval cmplval, cmprval;
   cmplval.val = lval;
   cmprval.val = rval;
   
   promote(cmplval , cmprval);
   
   return Builder->CreateICmpNE(cmplval.val, cmprval.val);    
}

llvm::Value* NotEqualExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
    return Builder->CreateFCmpONE(lval, rval);
}

void NotEqualExpr::printOp() {
  std::cout << " != ";
}