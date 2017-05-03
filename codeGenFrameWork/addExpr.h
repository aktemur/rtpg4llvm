#ifndef _ADD_EXPR
#define _ADD_EXPR

#include "arithmeticExpr.h"

namespace codeGen {

class AddExpr : public ArithmeticExpr {
public:
   AddExpr(ExprCode * lhs, ExprCode * rhs) :
   ArithmeticExpr(lhs, rhs) {
   }

   virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned);
   virtual llvm::Value* fpOp(llvm::Value* lval, llvm::Value* rval);
   virtual llvm::Value* pointerOp(llvm::Value* lval, llvm::Value* rval);
   virtual void printOp();
};

}

#endif