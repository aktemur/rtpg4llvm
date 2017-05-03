#ifndef _LOGICAL_AND_EXPR_
#define _LOGICAL_AND_EXPR_

#include "logicalExpr.h"

namespace codeGen {

class LogicalAndExpr : public LogicalExpr {
public:
   LogicalAndExpr(ExprCode* lhs, ExprCode* rhs) :
   LogicalExpr(lhs, rhs) {
   }
   
   virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval);
   virtual void printOp();
};

}

#endif