#ifndef codeGenFrameWork_program_h
#define codeGenFrameWork_program_h

#include "compileUnit.h"
#include <vector>

namespace codeGen {
  class Program : public CompileUnit {
  private:
    vector<CompileUnit*> *units;
  public:
    Program(vector<CompileUnit*> *units): units(units) { }
    ~Program();
    virtual void build();
    virtual void print();
    virtual void compile();
  };
  
}


#endif
