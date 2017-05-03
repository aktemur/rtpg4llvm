#include "greaterThanExpr.h"

using namespace codeGen;

llvm::Value* GreaterThanExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
    if (isSigned) {
        return Builder->CreateICmpSGT(lval, rval);
    } else {
        return Builder->CreateICmpUGT(lval, rval);
    }
}

llvm::Value* GreaterThanExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
    return Builder->CreateFCmpOGT(lval, rval);
}

void GreaterThanExpr::printOp() {
  std::cout << " > ";
}