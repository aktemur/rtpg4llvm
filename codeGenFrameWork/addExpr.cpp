#include "addExpr.h"

using namespace codeGen;

llvm::Value* AddExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
  llvm::Value *val = Builder->CreateNSWAdd(lval, rval);
  val->setName("add");
  return val;
}

llvm::Value* AddExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
  llvm::Value *val = Builder->CreateFAdd(lval, rval);
  val->setName("add");
  return val;
}

llvm::Value* AddExpr::pointerOp(llvm::Value* lval, llvm::Value* rval) 
{
    return Builder->CreateInBoundsGEP(lval, rval);
}

void AddExpr::printOp() {
  std::cout << " + ";
}
