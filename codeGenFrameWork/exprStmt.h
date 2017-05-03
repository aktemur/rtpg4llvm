#ifndef _EXPR_STMT
#define _EXPR_STMT

#include "expressions.h"
#include "stmtCode.h"

namespace codeGen {
  
class ExprStmt : public StmtCode {
   ExprCode* e;
public:
   ExprStmt(ExprCode *e) :
     e(e) { }
   ~ExprStmt();
  
   virtual void build();
   virtual void print(int level);
};

}

#endif