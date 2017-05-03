#ifndef _EMPTY_STMT
#define _EMPTY_STMT

#include "stmtCode.h"

namespace codeGen {
  
class EmptyStmt : public StmtCode {
public:
   EmptyStmt() {
   }
   virtual void build();
   virtual void print(int level);
};

}

#endif