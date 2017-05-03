#ifndef _RELATIONAL_EXPR
#define _RELATIONAL_EXPR

#include "binaryExpr.h"

namespace codeGen {

class RelationalExpr : public BinaryExpr {
public:
   RelationalExpr(ExprCode * left, ExprCode * right) :
   BinaryExpr(left, right) {
   }

   virtual cval op(cval lval, cval rval);
   virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) = 0;
   virtual llvm::Value* fpOp(llvm::Value* lval, llvm::Value* rval) = 0;
};

}

#endif
