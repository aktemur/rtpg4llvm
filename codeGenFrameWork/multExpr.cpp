#include "multExpr.h"

using namespace codeGen;

llvm::Value* MultExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) 
{
  llvm::Value *val = Builder->CreateNSWMul(lval, rval);
  val->setName("mul");
  return val;
}

llvm::Value* MultExpr::fpOp(llvm::Value* lval, llvm::Value* rval) 
{
  llvm::Value *val = Builder->CreateFMul(lval, rval);
  val->setName("mul");
  return val;
}

llvm::Value* MultExpr::pointerOp(llvm::Value* lval, llvm::Value* rval) 
{
    std::cerr << "Incompatible types in mul." << endl;
    exit(1);
}

void MultExpr::printOp() {
  std::cout << " * ";
}

