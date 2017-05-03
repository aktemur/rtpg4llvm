#ifndef _LOGICAL_NOT_EXPR_
#define _LOGICAL_NOT_EXPR_

#include "exprCode.h"

namespace codeGen {

class LogicalNotExpr : public ExprCode {
  ExprCode* exp;
public:
  LogicalNotExpr(ExprCode* exp) :
  exp(exp) {
  }
  ~LogicalNotExpr();
  
  virtual cval build();
  virtual void print();
};

}

#endif