#ifndef codeGenFrameWork_arrayExpr_h
#define codeGenFrameWork_arrayExpr_h

#include "exprCode.h"

namespace codeGen {
  
  class ArrayExpr : public ExprCode {
  private:
    ExprCode *base;
    ExprCode *index;
  public:
    ArrayExpr(ExprCode *base, ExprCode *index) :
    base(base), index(index) { }
    ~ArrayExpr();
    
    virtual cval build();
    virtual cval buildLHS();
    virtual void print();
  };
  
}

#endif
