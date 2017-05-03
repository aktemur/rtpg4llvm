#ifndef _LESS_THEN_EXPR
#define _LESS_THEN_EXPR

#include "relationalExpr.h"

namespace codeGen {

class LessThanExpr : public RelationalExpr {
public:
   LessThanExpr(ExprCode * lhs, ExprCode * rhs) :
   RelationalExpr(lhs, rhs) {
   }
    
   virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned);
   virtual llvm::Value* fpOp(llvm::Value* lval, llvm::Value* rval);
   virtual void printOp();
};

}

#endif