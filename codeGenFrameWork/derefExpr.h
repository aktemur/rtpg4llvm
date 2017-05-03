#ifndef _DEREF_EXPR
#define _DEREF_EXPR

#include "exprCode.h"

namespace codeGen {

class DerefExpr : public ExprCode {
    ExprCode *exp;
public:
    DerefExpr(ExprCode *exp) :
    exp(exp) {
    }
    
    ~DerefExpr();
  
    virtual cval build();
  
    virtual cval buildLHS();

    virtual void print();
};

}

#endif