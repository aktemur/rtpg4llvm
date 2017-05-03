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

OSKI::OSKI(MMMatrix *mmMatrix, int b_r, int b_c):
  SpMVSpecializer(mmMatrix), b_r(b_r), b_c(b_c) 
{
  int n = mmMatrix->n;
  // if(n/b_r*b_r != n || n/b_c*b_c != n) {
  //   std::cerr << "WARNING: OSKI specializer only handles matrices "
  //             << "where block sizes divide n evenly;\n"
  //             << "you may need to run split_by_block first.\n";
  //   exit(1);
  // }
}

Matrix* OSKI::getMatrixForGeneration() {
  return mmMatrix->toOSKIMatrix(b_r, b_c);
}

StmtCode* OSKI::getLocalDecls() {
  string cols = newName("cols", stage);
  string vals = newName("vals", stage);

  return $stmt<
    int row; int c;
    double *mvals; double ww;
    int *col; int r; int nz;
    col = `cols;       
    row = 0;
    nz = 0;
    mvals = `vals;
  >$;
}

std::queue<WeightedStmt>* OSKI::getBody(Matrix *matrix) {
  string rows = newName("rows", stage);

  StmtCode *stmt = $stmt<
    for (row = 0; row < `int(matrix->n / b_r`); row++) {
      nz = `rows[row];
      `stmt(getWhileLoop()`)
    }
  >$;
  
  WeightedStmt body(stmt, 0);
  std::queue<WeightedStmt> *stmts = new std::queue<WeightedStmt>;
  stmts->push(body);
  return stmts;
}

StmtCode* OSKI::getWhileLoop() {
  StmtCode *whileLoopBody = $stmt<;>$;
  for(int i = 0; i < b_r; ++i) {  
    whileLoopBody = $stmt< 
      `stmt(whileLoopBody`) 
      ww = v[c] * (*mvals++); 
    >$;
    for(int j = 1; j < b_c; ++j) {
      whileLoopBody = $stmt< 
        `stmt(whileLoopBody`) 
        ww += v[c+`int(j`)] * (*mvals++);
      >$;
    }
    whileLoopBody = $stmt< 
      `stmt(whileLoopBody`) 
      w[r] += ww;
      r = r+1;
    >$;
  }

  StmtCode *whileLoop = $stmt<
    while(nz > 0) {
      r = row * `int(b_r`);
      c = (*col++) * `int(b_c`);
      `stmt(whileLoopBody`)
      nz--;
    }
  >$;
  return whileLoop; 
}    

class OSKIOptimizationPassPopulator: public OptimizationPassPopulator {
public:
  void addOptimizationPasses(llvm::PassManagerBase &MPM, llvm::FunctionPassManager &FPM) {
    std::cerr << "Implement OSKIOptPopulator!\n"; exit(1);

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

void OSKI::setOptimizationPasses() {
  CodeGeneration::setOptimizationPassPopulator(new OSKIOptimizationPassPopulator);
}

#include "lib/Target/X86/X86.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/Target/TargetLoweringObjectFile.h"

using namespace llvm;

void OSKI::dumpAssemblyText(llvm::MCStreamer *Str){
  MCContext &Ctx = Str->getContext();

  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R8));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R9));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R11));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R13));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::RAX));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::RCX));
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::RDX));

  // leaq "_rows1"(%rip), %r8
  Str->EmitInstruction(MCInstBuilder(X86::LEA64r).addReg(X86::R8).addReg(X86::RIP).addImm(1).addReg(0)
		       .addExpr(MCSymbolRefExpr::Create(newName("_rows", stage), MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
  // leaq "_cols1"(%rip), %r9
  Str->EmitInstruction(MCInstBuilder(X86::LEA64r).addReg(X86::R9).addReg(X86::RIP).addImm(1).addReg(0)
		       .addExpr(MCSymbolRefExpr::Create(newName("_cols", stage), MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
  // leaq "_mvalues1"(%rip), %r11
  Str->EmitInstruction(MCInstBuilder(X86::LEA64r).addReg(X86::R11).addReg(X86::RIP).addImm(1).addReg(0)
		       .addExpr(MCSymbolRefExpr::Create(newName("_mvalues", stage), MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
  // xorl %ecx, %ecx
  Str->EmitInstruction(MCInstBuilder(X86::XOR32rr).addReg(X86::ECX).addReg(X86::ECX).addReg(X86::ECX));
  //.align 16, 0x90
  Str->EmitCodeAlignment(16);

  dumpForLoop(Str);

  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::RDX));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::RCX));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::RAX));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R13));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R11));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R9));
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R8));
}

void OSKI::dumpForLoop(llvm::MCStreamer *Str){
  MCContext &Ctx = Str->getContext();

  string label1 = newName("LBB", stage, 1);
  string label2 = newName("LBB", stage, 2);
  string label3 = newName("LBB", stage, 3);

  // .LBB1_1:
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(label1)));
  // movl (%r8,%rcx,4), %r13d
  Str->EmitInstruction(MCInstBuilder(X86::MOV32rm).addReg(X86::R13D).addReg(X86::R8)
		       .addImm(4).addReg(X86::RCX).addImm(0).addReg(0));
  if (b_r == 2 || b_r == 3 || b_r == 5 || b_r == 9) {
    // leaq (%rcx,%rcx,"b_r-1"), %rdx
    Str->EmitInstruction(MCInstBuilder(X86::LEA64r).addReg(X86::RDX).addReg(X86::RCX).addImm(b_r-1)
                         .addReg(X86::RCX).addImm(0).addReg(0));
  } else if (b_r == 1) {
    // movsq %rcx, %rdx
    Str->EmitInstruction(MCInstBuilder(X86::MOV64rr).addReg(X86::RDX).addReg(X86::RCX));
  } else {
    std::cerr << "b_r value " << b_r << " is unsupported. Use 1,2,3,5,9.\n";
    exit(1);
  }
  // testl   %r13d, %r13d
  Str->EmitInstruction(MCInstBuilder(X86::TEST32rr).addReg(X86::R13D).addReg(X86::R13D));
  // je .LBB1_3
  Str->EmitInstruction(MCInstBuilder(X86::JE_1)
		       .addExpr(MCSymbolRefExpr::Create(label3, MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
  // .LBB1_2:
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(label2)));
  // imull "b_c", (%r9), %eax
  Str->EmitInstruction(MCInstBuilder(X86::IMUL32rmi8).addReg(X86::EAX).addReg(X86::R9).addImm(1)
		       .addReg(0).addImm(0).addReg(0).addImm(b_c));
  // movslq %eax, %rax
  Str->EmitInstruction(MCInstBuilder(X86::MOVSX64rr32).addReg(X86::RAX).addReg(X86::EAX));

  // Load v[c]'s
  int k=0;
  for(; k < b_c; ++k) { 
    // movsd k*8(%rdi,%rax,8), %xmm"k"
    Str->EmitInstruction(MCInstBuilder(X86::MOVSDrm).addReg(X86::XMM0+k).addReg(X86::RDI)
			 .addImm(8).addReg(X86::RAX).addImm(k*8).addReg(0));
  }
  int i,j;
  for(i=0; i < b_r; ++i) {
    // Load w[r]
    // movsd "i*8"(%rsi,%rdx,8), %xmm"15-i"
    Str->EmitInstruction(MCInstBuilder(X86::MOVSDrm).addReg(X86::XMM15-i).addReg(X86::RSI)
			 .addImm(8).addReg(X86::RDX).addImm(i*8).addReg(0));
    // Load mvalues
    for(j=0; j < b_c; ++j) {
      // movsd "(i*b_c+j)*8"(%r11), %xmm"k+j"
      Str->EmitInstruction(MCInstBuilder(X86::MOVSDrm).addReg(X86::XMM0+k+j).addReg(X86::R11)
                           .addImm(1).addReg(0).addImm((i * b_c + j)*8).addReg(0));
    }
    // Do the Math
    for(j=0; j < b_c; ++j) {
      // mulsd %xmm"j", %xmm"k+j"
      Str->EmitInstruction(MCInstBuilder(X86::MULSDrr)
                           .addReg(X86::XMM0+k+j).addReg(X86::XMM0+k+j).addReg(X86::XMM0+j));     
      // addsd %xmm"k+j", %xmm"15-i"
      Str->EmitInstruction(MCInstBuilder(X86::ADDSDrr)
                           .addReg(X86::XMM15-i).addReg(X86::XMM15-i).addReg(X86::XMM0+k+j));     
    }
    // Store w[r]
    // movsd %xmm"15-i", "i*8"(%rsi, %rdx, 8)
    Str->EmitInstruction(MCInstBuilder(X86::MOVSDmr).addReg(X86::RSI)
			 .addImm(8).addReg(X86::RDX).addImm(i*8).addReg(0).addReg(X86::XMM15-i));    
  } 

  // leaq "b_r * b_c * 8"(%r11), %r11
  Str->EmitInstruction(MCInstBuilder(X86::LEA64r).addReg(X86::R11).addReg(X86::R11)
		       .addImm(1).addReg(0).addImm(b_r * b_c * 8).addReg(0));
  // leaq 4(%r9), %r9
  Str->EmitInstruction(MCInstBuilder(X86::LEA64r).addReg(X86::R9).addReg(X86::R9)
		       .addImm(1).addReg(0).addImm(4).addReg(0));
  // decl %r13d
  Str->EmitInstruction(MCInstBuilder(X86::DEC64_32r).addReg(X86::R13D).addReg(X86::R13D));
  // testl   %r13d, %r13d
  Str->EmitInstruction(MCInstBuilder(X86::TEST32rr).addReg(X86::R13D).addReg(X86::R13D));
  // jg .LBB1_2
  Str->EmitInstruction(MCInstBuilder(X86::JG_1)
		       .addExpr(MCSymbolRefExpr::Create(label2, MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
  //.LBB1_3:
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(label3)));
  //incq %rcx
  Str->EmitInstruction(MCInstBuilder(X86::INC64r).addReg(X86::RCX).addReg(X86::RCX));
  //cmpl "n/b_r", %ecx
  Str->EmitInstruction(MCInstBuilder(X86::CMP32ri).addReg(X86::ECX).addImm(mmMatrix->n/b_r));
  //jne   .LBB1_1
  Str->EmitInstruction(MCInstBuilder(X86::JNE_1)
		       .addExpr(MCSymbolRefExpr::Create(label1, MCSymbolRefExpr::VK_None, Ctx))
		       .addReg(0));
}

void OSKI::dumpAssemblyConstData(llvm::MCStreamer *Str){
  MCContext &Ctx = Str->getContext();
  string mvalues = newName("_mvalues", stage);
  string cols = newName("_cols", stage);
  string rows = newName("_rows", stage);
  Matrix *matrix = getMatrix();

  if(matrix->numVals == 0) return;

  // .globl mvalues
  Str->EmitSymbolAttribute(Ctx.GetOrCreateSymbol(StringRef(mvalues)), MCSA_Global);
  // .align 16
  Str->EmitCodeAlignment(16);
  // mvalues:
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(mvalues)));

  for(int i=0; i < matrix->numVals; ++i) {
    // .quad "vals[i]"
    int64_t *value = (int64_t*) &(matrix->vals[i]);
    Str->EmitIntValue(*value, sizeof(double));
  }

  // .globl cols
  Str->EmitSymbolAttribute(Ctx.GetOrCreateSymbol(StringRef(cols)), MCSA_Global);
  // .align 16
  Str->EmitCodeAlignment(16);
  // cols:
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(cols)));

  for(int i=0; i < matrix->numCols; ++i) {
    // .long "cols[i]"
    Str->EmitIntValue(matrix->cols[i], sizeof(int));
  }

  // .globl rows
  Str->EmitSymbolAttribute(Ctx.GetOrCreateSymbol(StringRef(rows)), MCSA_Global);
  // .align 16
  Str->EmitCodeAlignment(16);
  // rows:
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(rows)));

  for(int i=0; i < matrix->numRows; ++i) {
    // .long "rows[i]"
    Str->EmitIntValue(matrix->rows[i], sizeof(int));
  }
}

void OSKI::dumpAssemblyData(llvm::MCStreamer *Str){
  //Do Nothing
}


