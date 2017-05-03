#include "modExpr.h"

using namespace codeGen;

llvm::Value* ModExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
  if (isSigned) {
    return Builder->CreateSRem(lval, rval);
  } else {
    return Builder->CreateURem(lval, rval);
  }
}

llvm::Value* ModExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
  std::cerr << "FP type not supported for mod operation." << endl;
  exit(1);
}

llvm::Value* ModExpr::pointerOp(llvm::Value* lval, llvm::Value* rval) 
{
  std::cerr << "Pointer type not supported for mod operation." << endl;
  exit(1);
}

void ModExpr::printOp() {
  std::cout << " % ";
}