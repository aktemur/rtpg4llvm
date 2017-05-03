#ifndef codeGenFrameWork_continueStmt_h
#define codeGenFrameWork_continueStmt_h

#include "stmtCode.h"

namespace codeGen {
  
class ContinueStmt : public StmtCode {
public:
  ContinueStmt() {
  }
  
  virtual void build();
  virtual void print(int indent);
};

}

#endif
