#ifndef _CONST_EXPR
#define _CONST_EXPR

#include "exprCode.h"

namespace codeGen {

  class ConstExprBase : public ExprCode {
  public:
    virtual cval build() = 0;
    
    virtual cval buildForGlobal(llvm::Type *toType);
  };
  
  template<class T>
  class ConstExpr : public ConstExprBase {
    T val;
  public:
    
    ConstExpr(T val) : val(val) {
    }
    
    virtual cval build() {
      return con();
    }
    
    virtual T getVal() {
      return val;
    }
    
    virtual void print();

  private:
    cval con();
    
  };
  
  class NullExpr : public ExprCode {
  public:
    NullExpr() { }
    
    virtual cval build();
  };  
  
}

#endif