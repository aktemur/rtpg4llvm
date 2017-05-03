#include "codeGenConstructors.h"
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

Matrix* CSRbyNZ::getMatrixForGeneration() {
  return mmMatrix->toCSRbyNZMatrix();
}

CompileUnit* CSRbyNZ::getGlobalDecls(Matrix *matrix) {
  string a = newName("a", stage);
  string colptr = newName("colptr", stage);
  string mval = newName("mval", stage);
  string cols = newName("cols", stage);
  string rows = newName("rows", stage);
  string vals = newName("vals", stage);

  return $compunit<
    int `a = 0;
    int * `colptr;
    double * `mval;
    int `cols[`int(matrix->nz`)] = `list(matrix->cols, matrix->nz`);
    int `rows[`int(matrix->n`)]  = `list(matrix->rows, matrix->n`);
    double `vals[`int(matrix->nz`)] = `list(matrix->vals, matrix->nz`);
  >$;
}

StmtCode* CSRbyNZ::getLocalDecls() {
  return $stmt< int row; double ww; int i; >$;
}

std::queue<WeightedStmt>* CSRbyNZ::getBody(Matrix *matrix) {
  string a = newName("a", stage);
  string colptr = newName("colptr", stage);
  string mval = newName("mval", stage);
  string cols = newName("cols", stage);
  string vals = newName("vals", stage);

  RowLengthView &rowMap = mmMatrix->getRowsByNZ();
  RowLengthView::reverse_iterator it = rowMap.rbegin(),
    itEnd = rowMap.rend();
  std::queue<WeightedStmt>* stmts = new std::queue<WeightedStmt>;
  WeightedStmt stmt($stmt<
                      `a = 0;
                      `mval = `vals;
                      `colptr = `cols; >$, 0);
  stmts->push(stmt);

  for(; it != itEnd; ++it) {
    int rowLength = it->first;
    int numRows = it->second.size();
    WeightedStmt wstmt(getSingleLoop(numRows, rowLength), rowLength);
    stmts->push(wstmt);
  }
  return stmts;
}

StmtCode* CSRbyNZ::getSingleLoop(int numRows, int rowLength) {
  if(rowLength == 0) return $stmt< ; >$;

  string a = newName("a", stage);
  string colptr = newName("colptr", stage);
  string mval = newName("mval", stage);
  string cols = newName("cols", stage);
  string rows = newName("rows", stage);
  string vals = newName("vals", stage);

  StmtCode *unrolledStmts = $stmt< ; >$;
  for(int i = 0; i < rowLength; ++i) {
    unrolledStmts = $stmt<
      `stmt(unrolledStmts`)
       ww += *`mval++ * v[*`colptr++];
    >$;
  }

  StmtCode *forLoop = $stmt<
    for (i=0; i < `int(numRows`); i++) {
      row = `rows[`a];
      `a++;
      ww = 0.0;
      `stmt(unrolledStmts`)
      w[row] += ww;
    }
  >$;
  return forLoop;
}

class CSRbyNZOptimizationPassPopulator: public OptimizationPassPopulator {
public:
  void addOptimizationPasses(llvm::PassManagerBase &MPM, llvm::FunctionPassManager &FPM) {
    FPM.add(llvm::createScalarReplAggregatesPass());

    MPM.add(llvm::createBasicAliasAnalysisPass());
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
    MPM.add(llvm::createInstructionCombiningPass());
    MPM.add(llvm::createDeadStoreEliminationPass());
    MPM.add(llvm::createCFGSimplificationPass());
  }
};

void CSRbyNZ::setOptimizationPasses() {
  CodeGeneration::setOptimizationPassPopulator(new CSRbyNZOptimizationPassPopulator);
}

#include "lib/Target/X86/X86.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/Target/TargetLoweringObjectFile.h"

using namespace llvm;

#ifdef PROF
#include <sys/time.h>
int timediff(struct timeval *res, struct timeval *x, struct timeval *y);
extern int timingLevel;
#endif 

void CSRbyNZ::dumpAssemblyText(llvm::MCStreamer *Str) {
  MCContext &Ctx = Str->getContext();
  START_TIME_PROFILE(getRowsByNZ);
  RowLengthView &rowMap = mmMatrix->getRowsByNZ();
  END_TIME_PROFILE(getRowsByNZ);
  RowLengthView::reverse_iterator it = rowMap.rbegin(), itEnd = rowMap.rend();

  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R8));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R9));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R10));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R11));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::RAX));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::RCX));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::RDX));
 
  // xorl %ecx, %ecx
  Str->EmitInstruction(MCInstBuilder(X86::XOR32rr).addReg(X86::ECX).addReg(X86::ECX).addReg(X86::ECX));
  // movq "_rows1"(%rip), %r8
  Str->EmitInstruction(MCInstBuilder(X86::MOV64rm).addReg(X86::R8).addReg(X86::RIP).addImm(1).addReg(0)
		       .addExpr(MCSymbolRefExpr::Create(newName("_rows", stage), MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
  // movq "_cols1"(%rip), %r9
  Str->EmitInstruction(MCInstBuilder(X86::MOV64rm).addReg(X86::R9).addReg(X86::RIP).addImm(1).addReg(0)
		       .addExpr(MCSymbolRefExpr::Create(newName("_cols", stage), MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
  // movq "_vals1"(%rip), %r10
  Str->EmitInstruction(MCInstBuilder(X86::MOV64rm).addReg(X86::R10).addReg(X86::RIP).addImm(1).addReg(0)
		       .addExpr(MCSymbolRefExpr::Create(newName("_vals", stage), MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
  
  int rowLength, numRows;
  int loopIndex = 0;
  for (; it != itEnd; ++it) {
    rowLength = it->first;
    numRows = it->second.size();
    dumpSingleLoop(Str, numRows, rowLength, loopIndex++);
  }

  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::RDX));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::RCX));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::RAX));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R11));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R10));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R9));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R8));
}

void CSRbyNZ::dumpSingleLoop(llvm::MCStreamer *Str, int numRows, int rowLength, int loopIndex) {
  MCContext &Ctx = Str->getContext();
  string label = newName("LBB", stage, loopIndex);

  //movq %rcx, %r11
  Str->EmitInstruction(MCInstBuilder(X86::MOV64rr).addReg(X86::R11).addReg(X86::RCX));
  //movq %rcx, %rdx
  Str->EmitInstruction(MCInstBuilder(X86::MOV64rr).addReg(X86::RDX).addReg(X86::RCX));
  //.align 16, 0x90
  Str->EmitCodeAlignment(16);
  //.LBB0_1:
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(label)));
  //xorps %xmm0, %xmm0
  Str->EmitInstruction(MCInstBuilder(X86::XORPSrr).addReg(X86::XMM0).addReg(X86::XMM0).addReg(X86::XMM0));
  
  for(int i = 0 ; i < rowLength ; i++){
    //movslq "i*4"(%r9,%r11,4), %rax
    Str->EmitInstruction(MCInstBuilder(X86::MOVSX64rm32).addReg(X86::RAX).addReg(X86::R9)
                         .addImm(4).addReg(X86::R11).addImm(i*4).addReg(0));
    //movsd "i*8"(%r10,%r11,8), %xmm1
    Str->EmitInstruction(MCInstBuilder(X86::MOVSDrm).addReg(X86::XMM1).addReg(X86::R10)
			 .addImm(8).addReg(X86::R11).addImm(i*8).addReg(0));
    //mulsd (%rdi,%rax,8), %xmm1
    Str->EmitInstruction(MCInstBuilder(X86::MULSDrm).addReg(X86::XMM1).addReg(X86::XMM1)
			 .addReg(X86::RDI).addImm(8).addReg(X86::RAX).addImm(0).addReg(0));
    //addsd %xmm1, %xmm0
    Str->EmitInstruction(MCInstBuilder(X86::ADDSDrr).addReg(X86::XMM0).addReg(X86::XMM0).addReg(X86::XMM1));
  }
  
  // movslq (%r8,%rdx,4), %rax
  Str->EmitInstruction(MCInstBuilder(X86::MOVSX64rm32).addReg(X86::RAX).addReg(X86::R8)
                       .addImm(4).addReg(X86::RDX).addImm(0).addReg(0));
  //addq $rowLength, %r11
  Str->EmitInstruction(MCInstBuilder(X86::ADD64ri8).addReg(X86::R11).addReg(X86::R11).addImm(rowLength));
  //addq $1, %rdx
  Str->EmitInstruction(MCInstBuilder(X86::ADD64ri8).addReg(X86::RDX).addReg(X86::RDX).addImm(1));
  //addsd (%rsi,%rax,8), %xmm0
  Str->EmitInstruction(MCInstBuilder(X86::ADDSDrm).addReg(X86::XMM0).addReg(X86::XMM0).addReg(X86::RSI)
		       .addImm(8).addReg(X86::RAX).addImm(0).addReg(0));
  //cmpl numRows, %edx
  Str->EmitInstruction(MCInstBuilder(X86::CMP32ri).addReg(X86::EDX).addImm(numRows));

  //movsd %xmm0, (%rsi,%rax,8)
  Str->EmitInstruction(MCInstBuilder(X86::MOVSDmr).addReg(X86::RSI).addImm(8).addReg(X86::RAX)
		       .addImm(0).addReg(0).addReg(X86::XMM0));
  //jne	.LBB0_1
  Str->EmitInstruction(MCInstBuilder(X86::JNE_1)
		       .addExpr(MCSymbolRefExpr::Create(label, MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
  //addq $numRows*4, %r8
  Str->EmitInstruction(MCInstBuilder(X86::ADD64ri32).addReg(X86::R8).addReg(X86::R8).addImm(numRows*4));
  //addq $numRows*rowLength*4, %r9
  Str->EmitInstruction(MCInstBuilder(X86::ADD64ri32).addReg(X86::R9).addReg(X86::R9).addImm(numRows*rowLength*4));
  //addq $numRows*rowLength*8, %r10
  Str->EmitInstruction(MCInstBuilder(X86::ADD64ri32).addReg(X86::R10).addReg(X86::R10).addImm(numRows*rowLength*8));
}

void CSRbyNZ::dumpAssemblyConstData(llvm::MCStreamer *Str){
  // Do nothing
}

void CSRbyNZ::dumpAssemblyData(llvm::MCStreamer *Str){
  // Do nothing
}
