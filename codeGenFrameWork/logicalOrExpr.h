#ifndef _LOGICAL_OR_EXPR_
#define _LOGICAL_OR_EXPR_

#include "logicalExpr.h"

namespace codeGen {

class LogicalOrExpr : public LogicalExpr {
public:
   LogicalOrExpr(ExprCode* lhs, ExprCode* rhs) :
   LogicalExpr(lhs, rhs) {
   }
   
   virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval);
   virtual void printOp();
};

}

#endif
