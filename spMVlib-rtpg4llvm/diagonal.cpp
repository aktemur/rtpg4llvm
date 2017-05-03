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

Diagonal::Diagonal(MMMatrix *mmMatrix):
  SpMVSpecializer(mmMatrix)
{
  if(mmMatrix->getDiagonals().size() == 0) {
    std::cout << "Matrix must be split by a band splitter";
    std::cout << " before specializing on diagonals.\n";
    exit(1);
  }
}

Matrix* Diagonal::getMatrixForGeneration() {
  return mmMatrix->toDiagonalMatrix();
}

CompileUnit* Diagonal::getGlobalDecls(Matrix *matrix) {
  string diags = newName("diags", stage);
  string vals = newName("mvalues", stage);

  return $compunit<
    int `diags[`int(matrix->numCols`)]  = `list(matrix->cols, matrix->numCols`);
    double `vals[`int(matrix->numVals`)] = `list(matrix->vals, matrix->numVals`);
  >$;
}

StmtCode* Diagonal::getLocalDecls() {
  string mvalues = newName("mvalues", stage);
  return $stmt<
    int i, j, fstrow, fstcol, sz, diag;
    double * __restrict__ w1;
    double * __restrict__ v1;
    double * __restrict__ m1;
    m1 = `mvalues;
  >$;
}

std::queue<WeightedStmt>* Diagonal::getBody(Matrix *matrix) {
  string diags = newName("diags", stage);
  int numDiags = mmMatrix->getDiagonals().size();

  StmtCode *stmt = $stmt<
    for (i=0; i < `int(numDiags`); i++) {
      diag = `diags[i];
      if(diag < 0) {
        sz = `int(matrix->n`) + diag;
        fstrow = -diag;
        fstcol = 0;
      } else {
        sz = `int(matrix->n`) - diag;
        fstrow = 0;
        fstcol = diag;
      }
      w1 = w+fstrow;
      v1 = v+fstcol;
      for (j=0; j<sz; j++) {
         w1[j] += m1[j]*v1[j];
      }
      m1 += sz;
    }
  >$;
  
  WeightedStmt body(stmt, 0);
  std::queue<WeightedStmt> *stmts = new std::queue<WeightedStmt>;
  stmts->push(body);
  return stmts;
}

class DiagonalOptimizationPassPopulator: public OptimizationPassPopulator {
public:
  void addOptimizationPasses(llvm::PassManagerBase &MPM, llvm::FunctionPassManager &FPM) {
    std::cerr << "Implement DiagonalOptPopulator!\n"; exit(1);

    FPM.add(llvm::createScalarReplAggregatesPass());
    FPM.add(llvm::createEarlyCSEPass());
    
    MPM.add(llvm::createBasicAliasAnalysisPass());
    MPM.add(llvm::createInstructionCombiningPass());
    MPM.add(llvm::createCFGSimplificationPass());
    MPM.add(llvm::createAlwaysInlinerPass());
    MPM.add(llvm::createFunctionAttrsPass());
    MPM.add(llvm::createTailCallEliminationPass());
    MPM.add(llvm::createLoopRotatePass());
    MPM.add(llvm::createIndVarSimplifyPass());
    MPM.add(llvm::createLoopIdiomPass());
    MPM.add(llvm::createLoopDeletionPass());
    MPM.add(llvm::createLoopUnrollPass());
    MPM.add(llvm::createGVNPass());
    MPM.add(llvm::createDeadStoreEliminationPass());
    MPM.add(llvm::createInstructionCombiningPass());
    MPM.add(llvm::createDeadStoreEliminationPass());
    MPM.add(llvm::createCFGSimplificationPass());
    MPM.add(llvm::createGlobalDCEPass());
  }
};

void Diagonal::setOptimizationPasses() {
  CodeGeneration::setOptimizationPassPopulator(new DiagonalOptimizationPassPopulator);
}


