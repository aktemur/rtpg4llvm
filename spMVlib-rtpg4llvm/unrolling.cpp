#include "spMVgen.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Vectorize.h"
#include "llvm/Transforms/IPO.h"

using namespace codeGen;
using namespace spMVgen;

Matrix* Unrolling::getMatrixForGeneration() {
  return mmMatrix->toCSRMatrix();
}

StmtCode* Unrolling::getLocalDecls() {
  return $stmt<
    int k; double ww; 
    int i; int kk; 
  >$;
}

std::queue<WeightedStmt>* Unrolling::getBody(Matrix* matrix) {
  string rows = newName("rows", stage);
  string cols = newName("cols", stage);
  string vals = newName("vals", stage);

  StmtCode *cleanupLoop;
  if(u == 1) {
    cleanupLoop = $stmt< ; >$; 
  } else {
    cleanupLoop = $stmt<
      for(; k < kk; k++) {
        ww += `vals [k] * v[ `cols[k]];
      }
    >$;
  }

  StmtCode *stmt = $stmt<
    for (i = 0; i < `int(matrix->n`); i++) {
      ww = 0.0;
      k = `rows [i];
      kk = `rows [i+1];
      for(0; k <= kk - `int(u`); k += `int(u`)) {
        `stmt(getInnerLoopBody()`)
      }
      `stmt(cleanupLoop`)
      w[i] += ww;
    }
  >$;

  WeightedStmt body(stmt, 0);
  std::queue<WeightedStmt> *stmts = new std::queue<WeightedStmt>;
  stmts->push(body);
  return stmts;
}

StmtCode* Unrolling::getInnerLoopBody() {
  string cols = newName("cols", stage);
  string vals = newName("vals", stage);
  ExprCode *e = $expr< `vals[k] * v[`cols[k]] >$;
  for(int i = 1; i < u; ++i) {
    e = $expr< `expr(e`) + `vals[k + `int(i`)] * v[`cols[k + `int(i`)]] >$; 
  }
  return $stmt< ww += `expr(e`); >$;
}

class UnrollingOptimizationPassPopulator: public OptimizationPassPopulator {
public:
  void addOptimizationPasses(llvm::PassManagerBase &MPM, llvm::FunctionPassManager &FPM) {
    FPM.add(llvm::createCFGSimplificationPass());
    FPM.add(llvm::createScalarReplAggregatesPass());
    
    MPM.add(llvm::createBasicAliasAnalysisPass());
    MPM.add(llvm::createFunctionAttrsPass());
    MPM.add(llvm::createEarlyCSEPass());
    MPM.add(llvm::createInstructionCombiningPass());
    MPM.add(llvm::createTailCallEliminationPass());
    MPM.add(llvm::createReassociatePass());
    MPM.add(llvm::createLoopRotatePass());
    MPM.add(llvm::createIndVarSimplifyPass());
    MPM.add(llvm::createLoopIdiomPass());
    MPM.add(llvm::createLoopDeletionPass());
    MPM.add(llvm::createLoopUnrollPass());
    MPM.add(llvm::createGVNPass());
    MPM.add(llvm::createCFGSimplificationPass());
    MPM.add(llvm::createInstructionCombiningPass());
  }
};

void Unrolling::setOptimizationPasses() {
  CodeGeneration::setOptimizationPassPopulator(new UnrollingOptimizationPassPopulator);
}

