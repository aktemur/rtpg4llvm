#ifndef _BINARY_EXPR
#define _BINARY_EXPR

#include "exprCode.h"

namespace codeGen {

class BinaryExpr : public ExprCode {
protected:
    ExprCode* left;
    ExprCode* right;
public:
    BinaryExpr(ExprCode* left, ExprCode* right) :
    left(left), right(right) {
    }
    ~BinaryExpr();
    
    virtual cval build();
    virtual cval op(cval lval, cval rval) = 0;
    virtual void print();
    virtual void printOp() = 0;
    void setRightExpr(ExprCode *e);
    void setLeftExpr(ExprCode *e);
};

}

#endif