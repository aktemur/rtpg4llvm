#ifndef _FUNC_CALL_EXPR
#define _FUNC_CALL_EXPR

#include <vector>
#include "exprCode.h"

namespace codeGen {

class FuncCallExpr : public ExprCode {
   string funcName;
   vector<ExprCode*> *args;
public:
   FuncCallExpr(string funcName, vector<ExprCode*> *args) :
   funcName(funcName), args(args){
   }
   
   ~FuncCallExpr();
  
   virtual cval build();

   virtual void print();
};

}

#endif