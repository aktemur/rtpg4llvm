#ifndef _ARITHMETIC_EXPR
#define _ARITHMETIC_EXPR

#include "binaryExpr.h"

namespace codeGen {

class ArithmeticExpr : public BinaryExpr {
public:
    ArithmeticExpr(ExprCode* left, ExprCode* right) :
    BinaryExpr(left, right) {
    }
    
    virtual cval op(cval lval, cval rval);
    virtual llvm::Value* integerOp(llvm::Value* lval, llvm::Value* rval, bool isSigned) = 0;
    virtual llvm::Value* fpOp(llvm::Value* lval, llvm::Value* rval) = 0;
    virtual llvm::Value* pointerOp(llvm::Value* lval, llvm::Value* rval) = 0;
};

}

#endif