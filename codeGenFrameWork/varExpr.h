#ifndef _VAR_EXPR
#define _VAR_EXPR

#include "exprCode.h"

namespace codeGen {

class VarExpr : public ExprCode {
   string name;
public:
   VarExpr(string name) :
   name(name) {
   }
   
   virtual cval build();
  
   virtual cval buildLHS();
  
   virtual void print();
  
   string getName();
};

}

#endif