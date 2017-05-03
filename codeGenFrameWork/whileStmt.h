#ifndef _WHILE_STMT
#define _WHILE_STMT

#include "expressions.h"
#include "stmtCode.h"

namespace codeGen {
  
class WhileStmt : public StmtCode {
   ExprCode* loopCondition;
   StmtCode* loopBody;
   
public:
   WhileStmt(ExprCode *loopCondition, StmtCode *loopBody) :
   loopCondition(loopCondition), loopBody(loopBody) {
   }
   ~WhileStmt();
  
   virtual void build();
   virtual void print(int indent);
};

}

#endif
