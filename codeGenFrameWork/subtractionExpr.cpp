#include "subtractionExpr.h"

using namespace codeGen;

llvm::Value* SubtractionExpr::integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned){
   return Builder->CreateNSWSub(lval, rval);
}

llvm::Value* SubtractionExpr::fpOp(llvm::Value* lval, llvm::Value* rval){
   return Builder->CreateFSub(lval, rval);
}

llvm::Value* SubtractionExpr::pointerOp(llvm::Value* lval, llvm::Value* rval){
   if (rval->getType()->isIntegerTy()) {     
      llvm::Value* negated_rval = Builder->CreateNeg(rval);
      return Builder->CreateInBoundsGEP(lval, negated_rval);
   }else  {
      std::cerr << "Unsupported operation: pointer + (not integer type) " << endl;
      exit(1);
   }
}

void SubtractionExpr::printOp() {
  std::cout << " - ";
}