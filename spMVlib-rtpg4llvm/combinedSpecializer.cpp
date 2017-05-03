#include "codeGenConstructors.h"
#include "spMVgen.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace codeGen;
using namespace spMVgen;

StmtCode* CombinedSpecializer::getLocalDecls() {
  return NULL;
}

std::queue<WeightedStmt>* CombinedSpecializer::getBody(Matrix *matrix) {
  return NULL;
}

Matrix* CombinedSpecializer::getMatrixForGeneration() {
  return NULL;
}

void CombinedSpecializer::setOptimizationPasses() {
  // TODO: Using pass list of specOne is correct only when
  // we use the splitAll splitter. Fix this code.
  specOne->setOptimizationPasses();
}

CompileUnit* CombinedSpecializer::getAllDecls() {
  CompileUnit *declsOne = specOne->getAllDecls();
  stage++;
  CompileUnit *declsTwo = specTwo->getAllDecls();

  CompileUnit *allDecls = $compunit<
    `unit(declsOne`)
    `unit(declsTwo`)
  >$;
  return allDecls;
}

void CombinedSpecializer::dumpAssemblyText(llvm::MCStreamer *Str) {
  if(specOne->getMMMatrix()->elts.size() > 0)
    specOne->dumpAssemblyText(Str);
  stage++;
  if(specTwo->getMMMatrix()->elts.size() > 0)
    specTwo->dumpAssemblyText(Str);
}

void CombinedSpecializer::dumpAssemblyConstData(llvm::MCStreamer *Str) {
  if(specOne->getMMMatrix()->elts.size() > 0)
    specOne->dumpAssemblyConstData(Str);
  stage++;
  if(specTwo->getMMMatrix()->elts.size() > 0)
    specTwo->dumpAssemblyConstData(Str);
}

void CombinedSpecializer::dumpAssemblyData(llvm::MCStreamer *Str) {
  if(specOne->getMMMatrix()->elts.size() > 0)
    specOne->dumpAssemblyData(Str);
  stage++;
  if(specTwo->getMMMatrix()->elts.size() > 0)
    specTwo->dumpAssemblyData(Str);
}

void CombinedSpecializer::dumpMatrixValueSetterFunction(llvm::MCStreamer *Str) {
  if(specOne->getMMMatrix()->elts.size() > 0)
    specOne->dumpMatrixValueSetterFunction(Str);
  stage++;
  if(specTwo->getMMMatrix()->elts.size() > 0)
    specTwo->dumpMatrixValueSetterFunction(Str);
}

void CombinedSpecializer::setMatrixValuesInGeneratedAssembly() {
  if(specOne->getMMMatrix()->elts.size() > 0)
    specOne->setMatrixValuesInGeneratedAssembly();
  stage++;
  if(specTwo->getMMMatrix()->elts.size() > 0)
    specTwo->setMatrixValuesInGeneratedAssembly();
}

