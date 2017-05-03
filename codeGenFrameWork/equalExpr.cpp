#include "equalExpr.h"

using namespace codeGen;

llvm::Value* EqualExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
   cval cmplval, cmprval;
   cmplval.val = lval;
   cmprval.val = rval;
   
   promote(cmplval , cmprval);
   
   return Builder->CreateICmpEQ(cmplval.val, cmprval.val);    
}

llvm::Value* EqualExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
    return Builder->CreateFCmpOEQ(lval, rval);
}

void EqualExpr::printOp() {
  std::cout << " == ";
}