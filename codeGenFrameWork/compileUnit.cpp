#include "compileUnit.h"
#include <vector>

using namespace codeGen;

CompileUnit::~CompileUnit() {}

void CompileUnit::compile() {
  vector<CompileUnit*> units;
  units.push_back(this);
  CodeGeneration::compile(&units);
}

void CompileUnit::compileAndDump() {
  compile();
  CodeGeneration::TheModule->dump();
}

void* CompileUnit::getFunctionPointer(string funcName) {
  return CodeGeneration::getFunctionPointer(funcName);
}