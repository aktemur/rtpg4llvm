#ifndef _EXPR_CODE
#define _EXPR_CODE

#include <iostream>
#include "codeGeneration.h"

namespace codeGen {

class ExprCode : public CodeGeneration {
public:
   virtual ~ExprCode();
   virtual cval build() = 0;
   virtual cval buildLHS();
   virtual void print() = 0;
};

}

#endif