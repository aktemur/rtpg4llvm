#ifndef codeGenFrameWork_prePostOpExpr_h
#define codeGenFrameWork_prePostOpExpr_h

#include "exprCode.h"
#include "binaryExpr.h"

namespace codeGen {
  class PrePostOpBase: public ExprCode {
  protected:
    ExprCode *exp;
  public:
    ~PrePostOpBase();
    
    virtual cval build();
  protected:
    PrePostOpBase(ExprCode *e): exp(e) {}
    virtual BinaryExpr* getBinaryExpr() = 0;
    virtual cval& choosePreOrPostVal(cval& preVal, cval& postVal) = 0;
  };
  
  class PostIncrExpr: public PrePostOpBase {
  public:
    PostIncrExpr(ExprCode *exp): PrePostOpBase(exp) {}
    virtual void print();
    virtual BinaryExpr* getBinaryExpr();
    virtual cval& choosePreOrPostVal(cval& preVal, cval& postVal);
  };

  class PreIncrExpr: public PrePostOpBase {
  public:
    PreIncrExpr(ExprCode *exp): PrePostOpBase(exp) {}
    virtual void print();
    virtual BinaryExpr* getBinaryExpr();
    virtual cval& choosePreOrPostVal(cval& preVal, cval& postVal);
  };

  class PostDecrExpr: public PrePostOpBase {
  public:
    PostDecrExpr(ExprCode *exp): PrePostOpBase(exp) {}
    virtual void print();
    virtual BinaryExpr* getBinaryExpr();
    virtual cval& choosePreOrPostVal(cval& preVal, cval& postVal);
  };

  class PreDecrExpr: public PrePostOpBase {
  public:
    PreDecrExpr(ExprCode *exp): PrePostOpBase(exp) {}
    virtual void print();
    virtual BinaryExpr* getBinaryExpr();
    virtual cval& choosePreOrPostVal(cval& preVal, cval& postVal);
  };
  
}

#endif
