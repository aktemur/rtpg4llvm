#ifndef _RETURN_STMT
#define _RETURN_STMT

#include "expressions.h"
#include "stmtCode.h"

namespace codeGen {
  
class ReturnStmt : public StmtCode {
   ExprCode* exp;
   
public:
   ReturnStmt() :
   exp(NULL){
   }
   ~ReturnStmt();

   ReturnStmt(ExprCode* exp) :
      exp(exp){
   }

   virtual void build();
   virtual void print(int indent);
};

}

#endif