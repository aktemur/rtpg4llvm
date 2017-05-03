#include "greaterThanEqExpr.h"

using namespace codeGen;

llvm::Value* GreaterThanEqExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
    if (isSigned) {
        return Builder->CreateICmpSGE(lval, rval);
    } else {
        return Builder->CreateICmpUGE(lval, rval);
    }
}

llvm::Value* GreaterThanEqExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
    return Builder->CreateFCmpOGE(lval, rval);
}

void GreaterThanEqExpr::printOp() {
  std::cout << " >= ";
}