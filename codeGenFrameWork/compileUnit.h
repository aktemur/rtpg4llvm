#ifndef _COMPILE_UNIT_
#define _COMPILE_UNIT_

#include "codeGeneration.h"

namespace codeGen {

class CompileUnit : public CodeGeneration {
public:
   virtual ~CompileUnit();
   virtual void build() = 0;
   virtual void print() = 0;
   virtual void compile();
   virtual void compileAndDump();
   void* getFunctionPointer(string funcName);
};

}

#endif
