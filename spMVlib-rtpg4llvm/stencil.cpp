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

Matrix* Stencil::getMatrixForGeneration() {
  return mmMatrix->toStencilMatrix();
}
   
CompileUnit* Stencil::getGlobalDecls(Matrix *matrix) {
  string mvals = newName("mvals", stage);
  string mvalues = newName("mvalues", stage);
  return $compunit<
    double *`mvals;
    double `mvalues[`int(matrix->numVals`)] = `list(matrix->vals, matrix->numVals`);
  >$;
}

StmtCode* Stencil::getLocalDecls() {
  return $stmt< 
    double *vv;
    int row;
    int i;
  >$; 
}

std::queue<WeightedStmt>* Stencil::getBody(Matrix *matrix) {
  string mvals = newName("mvals", stage);
  string mvalues = newName("mvalues", stage);
  std::queue<WeightedStmt>* stmts = new std::queue<WeightedStmt>;
  WeightedStmt stmt($stmt< `mvals = `mvalues; >$, 0);
  stmts->push(stmt);

  StencilView &stencilView = mmMatrix->getStencils();
  StencilView::iterator stencilIt = stencilView.begin(),
    stencilItEnd = stencilView.end();
  for(; stencilIt != stencilItEnd; ++stencilIt) {
    const std::vector<int> *stencil = (*stencilIt)->getEltIndices(); 
    std::vector<int> &rows = (*stencilIt)->getRows(); 
    WeightedStmt wstmt(getStencilLoop(*stencil, rows), stencil->size());
    stmts->push(wstmt);
  }

  return stmts;
}

StmtCode* Stencil::getStencilLoop(const std::vector<int> &stencil, 
                                  std::vector<int> &rows) {
  string mvals = newName("mvals", stage);
  int popularity = rows.size();
  int stencilSize = stencil.size();

  if(popularity == 1) {
    int row = rows[0];
    ExprCode *unrolledExprs = $expr< w[`int(row`)] >$;
    for(int i = 0; i < stencilSize; ++i) {
      unrolledExprs = $expr< `expr(unrolledExprs`) + 
       	 v[`int(row+stencil[i]`)] * (*(`mvals + `int(i`)))
      >$;
    }
    return $stmt< 
       w[`int(row`)] = `expr(unrolledExprs`);
      `mvals += `int(stencilSize`);
    >$;
  } else {
    int row = rows[0];
    string stencilArrayName = newName("stencil_", row);

    ExprCode *unrolledExprs = $expr< w[row] >$;
    for(int i = 0; i < stencilSize; ++i) {
      unrolledExprs = $expr< `expr(unrolledExprs`) + 
       	 vv[`int(stencil[i]`)] * `mvals[`int(i`)]
      >$;
    }

    StmtCode *forLoop = $stmt<
      int `name(stencilArrayName`)[`int(popularity`)] = `list(&rows[0], popularity`);
      for(i=0; i < `int(popularity`); i++) {
        row = `name(stencilArrayName`)[i];
        vv = v+row;
        w[row] = `expr(unrolledExprs`);
        `mvals += `int(stencilSize`);
      }
    >$;
    return forLoop;
  }
}


class StencilOptimizationPassPopulator: public OptimizationPassPopulator {
public:
  void addOptimizationPasses(llvm::PassManagerBase &MPM, llvm::FunctionPassManager &FPM) {
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

void Stencil::setOptimizationPasses() {
  CodeGeneration::setOptimizationPassPopulator(new StencilOptimizationPassPopulator);
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


void Stencil::dumpAssemblyText(llvm::MCStreamer *Str) {
  MCContext &Ctx = Str->getContext();
  START_TIME_PROFILE(getStencils);
  StencilView &stencilView = mmMatrix->getStencils();
  END_TIME_PROFILE(getStencils);

  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R8));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R9));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R11));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::RCX));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::RDX));

  //  movq "vals"(%rip), %r9
  Str->EmitInstruction(MCInstBuilder(X86::MOV64rm).addReg(X86::R9).addReg(X86::RIP).addImm(1).addReg(0)
                       .addExpr(MCSymbolRefExpr::Create(newName("_vals", stage), MCSymbolRefExpr::VK_None, Ctx))
                       .addReg(0));    
  //  movq "rows"(%rip), %r8
  Str->EmitInstruction(MCInstBuilder(X86::MOV64rm).addReg(X86::R8).addReg(X86::RIP).addImm(1).addReg(0)
		       .addExpr(MCSymbolRefExpr::Create(newName("_rows", stage), MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));    
  StencilView::iterator stencilIt = stencilView.begin(),
    stencilItEnd = stencilView.end();
  int baseValsIndex = 0;
  for(; stencilIt != stencilItEnd; ++stencilIt) {
    const std::vector<int> *stencil = (*stencilIt)->getEltIndices(); 
    std::vector<int> &rows = (*stencilIt)->getRows(); 
    dumpStencilAssemblyText(Str, *stencil, rows, baseValsIndex);
    baseValsIndex += stencil->size() * rows.size();
  }
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::RDX));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::RCX));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R11));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R9));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R8));
}

void Stencil::dumpStencilAssemblyText(llvm::MCStreamer *Str, 
                              const std::vector<int> &stencil, 
                              std::vector<int> &rows, 
                              int baseValsIndex) {
  MCContext &Ctx = Str->getContext();
  string mvalues = newName("_mvalues", stage);
  int popularity = rows.size();
  int stencilSize = stencil.size();
  if(stencilSize == 0) return;

  int row = rows[0];
  if(popularity == 1) {
    //  movsd  "8*(row+stencil[0])"(%rdi), %xmm1
    Str->EmitInstruction(MCInstBuilder(X86::MOVSDrm).addReg(X86::XMM1).addReg(X86::RDI)
                         .addImm(1).addReg(0).addImm(8*(row+stencil[0])).addReg(0));    
    //  mulsd "sizeof(double)*(baseValsIndex)"(%r9), %xmm1
    Str->EmitInstruction(MCInstBuilder(X86::MULSDrm).addReg(X86::XMM1).addReg(X86::XMM1).addReg(X86::R9)
                         .addImm(1).addReg(0).addImm(0).addReg(0));
    
    for(int i = 1; i < stencilSize; ++i) {
      //  movsd "8*(row+stencil[i])"(%rdi), %xmm0
      Str->EmitInstruction(MCInstBuilder(X86::MOVSDrm).addReg(X86::XMM0).addReg(X86::RDI)
                           .addImm(1).addReg(0).addImm(8*(row+stencil[i])).addReg(0));    
      //  mulsd "sizeof(double)*(baseValsIndex + i)"(%r9), %xmm0
      Str->EmitInstruction(MCInstBuilder(X86::MULSDrm).addReg(X86::XMM0).addReg(X86::XMM0).addReg(X86::R9)
                           .addImm(1).addReg(0).addImm(8*(i)).addReg(0));
      //  addsd %xmm0, %xmm1
      Str->EmitInstruction(MCInstBuilder(X86::ADDSDrr).addReg(X86::XMM1).addReg(X86::XMM1).addReg(X86::XMM0));
    }
    //  leaq "sizeof(double)*stencilSize"(%r9), %r9
    Str->EmitInstruction(MCInstBuilder(X86::LEA64r).addReg(X86::R9).addReg(X86::R9)
                         .addImm(1).addReg(0).addImm(sizeof(double)*stencilSize).addReg(0));
    //  addsd "sizeof(double)*row"(%rsi), %xmm1
    Str->EmitInstruction(MCInstBuilder(X86::ADDSDrm).addReg(X86::XMM1).addReg(X86::XMM1).addReg(X86::RSI)
                         .addImm(1).addReg(0).addImm(sizeof(double)*row).addReg(0));
    //  movsd %xmm1, "sizeof(double)*row"(%rsi)
    Str->EmitInstruction(MCInstBuilder(X86::MOVSDmr).addReg(X86::RSI).addImm(1).addReg(0)
                         .addImm(sizeof(double)*row).addReg(0).addReg(X86::XMM1));
  } else {
    string stencilArrayName = newName("_stencil", stage, row);
    string blockLabelName = newName("LBB", stage, row);

    //  xorl %ecx, %ecx
    Str->EmitInstruction(MCInstBuilder(X86::XOR32rr).addReg(X86::ECX).addReg(X86::ECX).addReg(X86::ECX));
    //  movq %rcx, %r11
    Str->EmitInstruction(MCInstBuilder(X86::MOV64rr).addReg(X86::R11).addReg(X86::RCX));
    //  .align 4, 0x90
    Str->EmitCodeAlignment(16);
    //  LBB_"row":
    Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(blockLabelName)));
    //  movslq (%r11,%r8), %rdx
    Str->EmitInstruction(MCInstBuilder(X86::MOVSX64rm32).addReg(X86::RDX).addReg(X86::R11)
                         .addImm(1).addReg(X86::R8).addImm(0).addReg(0));    
    //  movsd "8*(stencil[0])"(%rdi,%rdx,8), %xmm1
    Str->EmitInstruction(MCInstBuilder(X86::MOVSDrm).addReg(X86::XMM1).addReg(X86::RDI)
                         .addImm(8).addReg(X86::RDX).addImm(8*(stencil[0])).addReg(0));    
    //  mulsd (%r9), %xmm1
    Str->EmitInstruction(MCInstBuilder(X86::MULSDrm).addReg(X86::XMM1).addReg(X86::XMM1)
                         .addReg(X86::R9).addImm(1).addReg(0).addImm(0).addReg(0));
    
    for(int i = 1; i < stencilSize; ++i) {
      //  movsd "8*(stencil[i])"(%rdi,%rdx,8), %xmm0
      Str->EmitInstruction(MCInstBuilder(X86::MOVSDrm).addReg(X86::XMM0).addReg(X86::RDI)
                           .addImm(8).addReg(X86::RDX).addImm(8*(stencil[i])).addReg(0));    
      //  mulsd "8*i"(%r9), %xmm0
      Str->EmitInstruction(MCInstBuilder(X86::MULSDrm).addReg(X86::XMM0).addReg(X86::XMM0)
                           .addReg(X86::R9).addImm(1).addReg(0).addImm(8*i).addReg(0));
      //  addsd %xmm0, %xmm1
      Str->EmitInstruction(MCInstBuilder(X86::ADDSDrr).addReg(X86::XMM1).addReg(X86::XMM1).addReg(X86::XMM0));      
    }
    //  addsd (%rsi,%rdx,8), %xmm1
    Str->EmitInstruction(MCInstBuilder(X86::ADDSDrm).addReg(X86::XMM1).addReg(X86::XMM1).addReg(X86::RSI)
                         .addImm(8).addReg(X86::RDX).addImm(0).addReg(0));
    //  addq $"stencilSize", %rcx
    Str->EmitInstruction(MCInstBuilder(X86::ADD64ri8).addReg(X86::RCX).addReg(X86::RCX).addImm(stencilSize));      
    //  addq $"sizeof(int)", %r11
    Str->EmitInstruction(MCInstBuilder(X86::ADD64ri8).addReg(X86::R11).addReg(X86::R11).addImm(sizeof(int)));      
    //  movsd %xmm1, (%rsi,%rdx,8)
    Str->EmitInstruction(MCInstBuilder(X86::MOVSDmr).addReg(X86::RSI).addImm(8).addReg(X86::RDX)
                         .addImm(0).addReg(0).addReg(X86::XMM1));
    //  leaq "sizeof(double)*stencilSize"(%r9), %r9
    Str->EmitInstruction(MCInstBuilder(X86::LEA64r).addReg(X86::R9).addReg(X86::R9)
                         .addImm(1).addReg(0).addImm(sizeof(double)*stencilSize).addReg(0));
    //  cmpl $"popularity*sizeof(int)", %r11d
    Str->EmitInstruction(MCInstBuilder(X86::CMP32ri).addReg(X86::R11D).addImm(popularity*sizeof(int)));
    //  jne LBB_"row"
    Str->EmitInstruction(MCInstBuilder(X86::JNE_1)
                         .addExpr(MCSymbolRefExpr::Create(blockLabelName, MCSymbolRefExpr::VK_None, Ctx))
                         .addReg(0));        
    //  leaq "sizeof(int)*popularity"(%r8), %r8
    Str->EmitInstruction(MCInstBuilder(X86::LEA64r).addReg(X86::R8).addReg(X86::R8)
                         .addImm(1).addReg(0).addImm(sizeof(int)*popularity).addReg(0));
  } 
}

void Stencil::dumpAssemblyConstData(llvm::MCStreamer *Str) {
  // Do nothing
}

void Stencil::dumpAssemblyData(llvm::MCStreamer *Str) {
  // Do nothing
}
