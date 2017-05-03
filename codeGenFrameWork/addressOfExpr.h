#ifndef _ADDRESSOF_EXPR
#define _ADDRESSOF_EXPR

#include "exprCode.h"

namespace codeGen {

class AddressOfExpr : public ExprCode {
    ExprCode *exp;
public:
    AddressOfExpr(ExprCode *exp) :
    exp(exp) {
    }
  
    ~AddressOfExpr();

    virtual cval build();
  
    virtual void print();
};

}

#endif