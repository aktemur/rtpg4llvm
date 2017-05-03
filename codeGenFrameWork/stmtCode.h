#ifndef _STMT_CODE
#define _STMT_CODE

#include "codeGeneration.h"
#include <iostream>

namespace codeGen {

  class StmtCode : public CodeGeneration {
  public:
    virtual ~StmtCode();
    virtual void build() = 0;
    virtual void print(int indent) = 0;
    void print();
  protected:
    void indent(int level);
  };
    
}

#endif
