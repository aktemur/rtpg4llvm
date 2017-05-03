#ifndef _GREATER_THAN_EXPR
#define _GREATER_THAN_EXPR

#include "relationalExpr.h"

namespace codeGen {

class GreaterThanExpr : public RelationalExpr {
public:
   GreaterThanExpr(ExprCode * lhs, ExprCode * rhs) :
   RelationalExpr(lhs, rhs) {
   }
    
   virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned);
   virtual llvm::Value* fpOp(llvm::Value* lval, llvm::Value* rval);
   virtual void printOp();
};

}

#endif