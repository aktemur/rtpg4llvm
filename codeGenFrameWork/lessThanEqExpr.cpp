#include "lessThanEqExpr.h"

using namespace codeGen;

llvm::Value* LessThanEqExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
    if (isSigned) {
        return Builder->CreateICmpSLE(lval, rval);
    } else {
        return Builder->CreateICmpULE(lval, rval);
    }
}

llvm::Value* LessThanEqExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
    return Builder->CreateFCmpOLE(lval, rval);
}

void LessThanEqExpr::printOp() {
  std::cout << " <= ";
}