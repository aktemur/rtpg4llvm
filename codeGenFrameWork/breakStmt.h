#ifndef codeGenFrameWork_breakStmt_h
#define codeGenFrameWork_breakStmt_h

#include "stmtCode.h"

namespace codeGen {

class BreakStmt : public StmtCode {
public:
  BreakStmt() { }
    
  virtual void build();
  virtual void print(int indent);
};

}

#endif
