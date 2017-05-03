#include "lessThenExpr.h"

using namespace codeGen;

llvm::Value* LessThanExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
    if (isSigned) {
        return Builder->CreateICmpSLT(lval, rval);
    } else {
        return Builder->CreateICmpULT(lval, rval);
    }
}

llvm::Value* LessThanExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
    return Builder->CreateFCmpOLT(lval, rval);
}

void LessThanExpr::printOp() {
  std::cout << " < ";
}