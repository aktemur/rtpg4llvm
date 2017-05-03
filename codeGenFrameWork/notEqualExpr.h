#ifndef _NOT_EQUAL_EXPR_
#define _NOT_EQUAL_EXPR_

#include "relationalExpr.h"

namespace codeGen {

class NotEqualExpr : public RelationalExpr {
public:
    NotEqualExpr(ExprCode* lhs, ExprCode* rhs) :
    RelationalExpr(lhs, rhs) {
    }
    
    virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned);
    virtual llvm::Value* fpOp(llvm::Value* lval, llvm::Value* rval);
    virtual void printOp();
};

}

#endif