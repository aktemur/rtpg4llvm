#ifndef _UNARYMINUS_EXPR
#define _UNARYMINUS_EXPR

#include "exprCode.h"

namespace codeGen {

class UnaryMinusExpr : public ExprCode {
    ExprCode* exp;
public:
    UnaryMinusExpr(ExprCode* exp) :
    exp(exp) {
    }
    ~UnaryMinusExpr();
  
    virtual cval build();
  
    virtual void print();
};

}

#endif