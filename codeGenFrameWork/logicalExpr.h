#ifndef _LOGICAL_EXPR
#define _LOGICAL_EXPR

#include "binaryExpr.h"

namespace codeGen {

class LogicalExpr : public BinaryExpr {
public:
   LogicalExpr(ExprCode * left, ExprCode * right) :
   BinaryExpr(left, right) {
   }
   
   virtual cval op(cval lval, cval rval);
   virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval) = 0;
   
};

}

#endif