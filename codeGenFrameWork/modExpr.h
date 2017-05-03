#ifndef codeGenFrameWork_modExpr_h
#define codeGenFrameWork_modExpr_h

#include "arithmeticExpr.h"

namespace codeGen {
  
  class ModExpr : public ArithmeticExpr {
  public:
    ModExpr(ExprCode *lhs, ExprCode *rhs) :
    ArithmeticExpr(lhs, rhs) {
    }
    
    virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned);
    virtual llvm::Value* fpOp(llvm::Value* lval, llvm::Value* rval);
    virtual llvm::Value* pointerOp(llvm::Value* lval, llvm::Value* rval);
    virtual void printOp();
  };
  
}

#endif
