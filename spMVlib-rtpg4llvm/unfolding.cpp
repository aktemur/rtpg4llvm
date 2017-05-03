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

Unfolding::Unfolding(MMMatrix *mmMatrix):
  SpMVSpecializer(mmMatrix) {
  b_r = 1;
  b_c = mmMatrix->n;
}

Matrix* Unfolding::getMatrixForGeneration() {
  return mmMatrix->toUnfoldingMatrix(); 
}

CompileUnit* Unfolding::getGlobalDecls(Matrix *matrix) {
  return $compunit< >$;
}

StmtCode* Unfolding::getLocalDecls() {
  return $stmt< ; >$;
}

// If we care about full unfolding only, we can get rid of the 
// call to groupByBlocks, which in the case of full unfolding,
// will result in one group having all the elements of the matrix.
// This will increase the efficiency of this specializer a bit.
// For the moment, we keep this blocked approach here
// for the sake of more generality.
std::queue<WeightedStmt>* Unfolding::getBody(Matrix *matrix) {
  std::queue<WeightedStmt>* stmts = new std::queue<WeightedStmt>;
  GroupByBlockView &blocks = mmMatrix->getGroupByBlocks(b_r, b_c);
  GroupByBlockView::iterator blockIt = blocks.begin(), 
    blockItEnd = blocks.end();

  for(; blockIt != blockItEnd; ++blockIt) {
    std::vector<MMElement*> &elts = blockIt->second;
    addBlockStmts(*stmts, elts);
  }

  return stmts;
}

// Generate per-row statements for the elements in elts,
void Unfolding::addBlockStmts(std::queue<WeightedStmt> &destination,
                              std::vector<MMElement*> &elts) {
  // Have an expression for each row in the block.
  ExprCode *exprs[b_r];
  int rowLengths[b_r];
  int rows[b_r]; // real row indices for each row in this block
  for(int i = 0; i < b_r; ++i) {
    exprs[i] = NULL;
    rowLengths[i] = 0;
    rows[i] = 0;
  }

  // Traverse the elements in the block,
  // add the contribution of each element to the 
  // corresponding expression
  std::vector<MMElement*>::iterator vecIt = elts.begin(), 
    vecItEnd = elts.end();
  for(; vecIt != vecItEnd; ++vecIt) {
    int row = (*vecIt)->row;
    int col = (*vecIt)->col;
    double val = (*vecIt)->val;
    int blockRow = row % b_r;
    if(exprs[blockRow] == NULL)
      exprs[blockRow] = $expr< `double(val`) * v[`int(col`)] >$;
    else
      exprs[blockRow] = $expr< `expr(exprs[blockRow]`) +
                               `double(val`) * v[`int(col`)] >$;
    rowLengths[blockRow] += 1;
    rows[blockRow] = row; 
  }

  // Construct the statement for each row in the block
  for(int i = 0; i < b_r; ++i) {
    if(rowLengths[i] == 0) // no element existed for this row
      continue;
    int row = rows[i];
    StmtCode *rowStmt = $stmt< w[`int(row`)] += `expr(exprs[i]`); >$;
    WeightedStmt wstmt(rowStmt, rowLengths[i]*2);
    destination.push(wstmt);
  }
}

class UnfoldingOptimizationPassPopulator: public OptimizationPassPopulator {
public:
  void addOptimizationPasses(llvm::PassManagerBase &MPM, llvm::FunctionPassManager &FPM) {
    FPM.add(llvm::createScalarReplAggregatesPass());
    FPM.add(llvm::createEarlyCSEPass());
    
    MPM.add(llvm::createBasicAliasAnalysisPass());
    MPM.add(llvm::createAlwaysInlinerPass());
    MPM.add(llvm::createFunctionAttrsPass());
    MPM.add(llvm::createInstructionCombiningPass());
    MPM.add(llvm::createTailCallEliminationPass());
    MPM.add(llvm::createReassociatePass());
    MPM.add(llvm::createLoopRotatePass());
    MPM.add(llvm::createLoopIdiomPass());
    MPM.add(llvm::createLoopDeletionPass());
    MPM.add(llvm::createLoopUnrollPass());
    MPM.add(llvm::createGVNPass());
    MPM.add(llvm::createInstructionCombiningPass());
  }
};

void Unfolding::setOptimizationPasses() {
  CodeGeneration::setOptimizationPassPopulator(new UnfoldingOptimizationPassPopulator);
}

#include "lib/Target/X86/X86.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCObjectStreamer.h"

#define REGISTER_LIMIT 15
//#define UNIQUEING
#define EMITTING

using namespace llvm;

extern bool DUMP_ASM;

int numElemsProcessed = 0;
int currentXMMIndex = 1;
map<int64_t, int> seenValues;
vector<int64_t> allValues;

#ifdef PROF
#include <sys/time.h>
int timediff(struct timeval *res, struct timeval *x, struct timeval *y);
extern int timingLevel;
#endif 

SmallVectorImpl<char> *DFOS;

void Unfolding::dumpAssemblyText(llvm::MCStreamer *Str) {
  MCContext &Ctx = Str->getContext();
  if (!DUMP_ASM) {
    MCObjectStreamer *objstr = dyn_cast<MCObjectStreamer>(Str);
    MCDataFragment *DF = objstr->getOrCreateDataFragment();
    DFOS = &(DF->getContents());
  }
#ifdef EMITTING
  Str->EmitInstruction(MCInstBuilder(X86::PUSH64r).addReg(X86::R9));
  //  movq "vals"(%rip), %r9
  Str->EmitInstruction(MCInstBuilder(X86::MOV64rm).addReg(X86::R9).addReg(X86::RIP).addImm(1).addReg(0)
                       .addExpr(MCSymbolRefExpr::Create(newName("_vals", stage), MCSymbolRefExpr::VK_None, Ctx))
                       .addReg(0));
#endif  

  dumpElements(Str, mmMatrix->elts);

#ifdef EMITTING
  Str->EmitInstruction(MCInstBuilder(X86::POP64r).addReg(X86::R9));
#endif
}

bool hadToSplitRow = false;

void Unfolding::dumpElements(llvm::MCStreamer *Str, std::vector<MMElement*> &elts) {
  MCContext &Ctx = Str->getContext();
  std::vector<MMElement*>::iterator vecIt = elts.begin(), 
    vecItEnd = elts.end();
  if(vecIt == vecItEnd) return;
  int currentRow = (*vecIt)->row;
  int regNumber = 0;
  std::vector<int> colIndices;
  std::vector<int> valIndices;

  hadToSplitRow = false;

  for(; vecIt != vecItEnd; ++vecIt) {
    int row = (*vecIt)->row;
    int col = (*vecIt)->col;
    double val = (*vecIt)->val;

    if(row != currentRow) {
      // this is the first element of the row
      // Finish up the previous row first.
      doMathOperations(Str, regNumber, colIndices, valIndices);
      dumpRowConclusion(Str, currentRow, regNumber);
      regNumber = 0;
      colIndices.clear();
      valIndices.clear();
      currentRow = row;
      hadToSplitRow = false;
    }
    colIndices.push_back(col);
    int valueIndex = allValues.size();
    int64_t rawValue = *((int64_t*) &(val));
#ifdef UNIQUEING
    map<int64_t,int>::iterator valueIt = seenValues.find(rawValue);
    if(valueIt == seenValues.end()) {
      seenValues[rawValue] = valueIndex;
#endif
      allValues.push_back(rawValue);
#ifdef UNIQUEING
    } else {
      valueIndex = (*valueIt).second;
    }
#endif
    valIndices.push_back(valueIndex);

    regNumber++;

    if (regNumber >= REGISTER_LIMIT) {
      doMathOperations(Str, regNumber, colIndices, valIndices);
      dumpPartialRowConclusion(Str, currentRow, regNumber);
      regNumber = 0;
      colIndices.clear();
      valIndices.clear();
    }
    numElemsProcessed++;
  }

  if(currentRow >= 0) {
    // finish up the last row
    doMathOperations(Str, regNumber, colIndices, valIndices);
    dumpRowConclusion(Str, currentRow, regNumber);
  }
}

void emitMOVSDmr(llvm::MCStreamer *Str, int memOffset, int XMMNumber) {
  if (DUMP_ASM) {
    Str->EmitInstruction(MCInstBuilder(X86::MOVSDrm).addReg(X86::XMM0+XMMNumber)
                         .addReg(X86::RDI).addImm(1).addReg(0)
                         .addImm(memOffset).addReg(0));
    return;
  }
  unsigned char data[9];
  unsigned char *dataPtr = data;
  *(dataPtr++) = 0xf2;
  if (XMMNumber >= 8)
    *(dataPtr++) = 0x44;
  *(dataPtr++) = 0x0f;
  *(dataPtr++) = 0x10;
  unsigned char regNumber = 0x07 + (XMMNumber % 8) * 8;
  if (memOffset > 0)
    regNumber += 0x40;
  if (memOffset >= 128) 
    regNumber += 0x40;
  *(dataPtr++) = regNumber;
  
  // memOffset
  if (memOffset > 0) {
    *(dataPtr++) = (unsigned char)memOffset;
  }
  if (memOffset >= 128) {
    *(dataPtr++) = (unsigned char)(memOffset >> 8);
    *(dataPtr++) = (unsigned char)(memOffset >> 16); 
    *(dataPtr++) = (unsigned char)(memOffset >> 24); 
  }

  DFOS->append(data, dataPtr);    
}

void emitADDSDrr(llvm::MCStreamer *Str, int XMMfrom, int XMMto) {
  if (DUMP_ASM) {
    Str->EmitInstruction(MCInstBuilder(X86::ADDSDrr)
                         .addReg(X86::XMM0+XMMto).addReg(X86::XMM0+XMMto).addReg(X86::XMM0+XMMfrom));
    return;
  }

  unsigned char data[5];
  unsigned char *dataPtr = data;
  *(dataPtr++) = 0xf2;
  if (XMMfrom >= 8 && XMMto < 8) {
    *(dataPtr++) = 0x41;
  } else if (XMMfrom < 8 && XMMto >= 8) {
    *(dataPtr++) = 0x44;
  } else if (XMMfrom >= 8 && XMMto >= 8) {
    *(dataPtr++) = 0x45;
  } 
  *(dataPtr++) = 0x0f;
  *(dataPtr++) = 0x58;
  unsigned char regNumber = 0xc0 + (XMMfrom % 8) + (XMMto % 8) * 8;
  *(dataPtr++) = regNumber;

  DFOS->append(data, dataPtr);    
}

void emitADDSDmr(llvm::MCStreamer *Str, int memOffset, int XMMNumber) {
  if (DUMP_ASM) {
    Str->EmitInstruction(MCInstBuilder(X86::ADDSDrm)
                         .addReg(X86::XMM0+XMMNumber).addReg(X86::XMM0+XMMNumber)
                         .addReg(X86::RSI).addImm(1).addReg(0)
                         .addImm(memOffset).addReg(0));
    return;
  }
  unsigned char data[9];
  unsigned char *dataPtr = data;
  *(dataPtr++) = 0xf2;
  if (XMMNumber >= 8)
    *(dataPtr++) = 0x44;
  *(dataPtr++) = 0x0f;
  *(dataPtr++) = 0x58;
  unsigned char regNumber = 0x06 + (XMMNumber % 8) * 8;
  if (memOffset > 0)
    regNumber += 0x40;
  if (memOffset >= 128) 
    regNumber += 0x40;
  *(dataPtr++) = regNumber;
  
  // memOffset
  if (memOffset > 0) {
    *(dataPtr++) = (unsigned char)memOffset;
  }
  if (memOffset >= 128) {
    *(dataPtr++) = (unsigned char)(memOffset >> 8);
    *(dataPtr++) = (unsigned char)(memOffset >> 16); 
    *(dataPtr++) = (unsigned char)(memOffset >> 24); 
  }

  DFOS->append(data, dataPtr);    
}

void emitMULSDmr(llvm::MCStreamer *Str, int memOffset, int XMMNumber) {
  if (DUMP_ASM) {
    Str->EmitInstruction(MCInstBuilder(X86::MULSDrm)
                         .addReg(X86::XMM0+XMMNumber).addReg(X86::XMM0+XMMNumber)
                         .addReg(X86::R9).addImm(1).addReg(0)
                         .addImm(memOffset).addReg(0));    
    return;
  }
  unsigned char data[9];
  unsigned char *dataPtr = data;
  *(dataPtr++) = 0xf2;
  if (XMMNumber >= 8)
    *(dataPtr++) = 0x45;
  else
    *(dataPtr++) = 0x41;
  *(dataPtr++) = 0x0f;
  *(dataPtr++) = 0x59;
  unsigned char regNumber = 0x01 + (XMMNumber % 8) * 8;
  if (memOffset > 0)
    regNumber += 0x40;
  if (memOffset >= 128) 
    regNumber += 0x40;
  *(dataPtr++) = regNumber;
  
  // memOffset
  if (memOffset > 0) {
    *(dataPtr++) = (unsigned char)memOffset;
  }
  if (memOffset >= 128) {
    *(dataPtr++) = (unsigned char)(memOffset >> 8);
    *(dataPtr++) = (unsigned char)(memOffset >> 16); 
    *(dataPtr++) = (unsigned char)(memOffset >> 24); 
  }

  DFOS->append(data, dataPtr);    
}

void Unfolding::doMathOperations(llvm::MCStreamer *Str, int lastRegIndex, vector<int> &colIndices, vector<int> &valIndices) {
  MCContext &Ctx = Str->getContext();
  for (int vRegIndex = 0; vRegIndex < lastRegIndex; vRegIndex++) {
    int colIndex = colIndices[vRegIndex];
#ifdef EMITTING
    //  movsd "sizeof(double)*colIndex"(%rdi), %xmm"vRegIndex"
    emitMOVSDmr(Str, sizeof(double)*colIndex, vRegIndex);
#endif
  }
  for (int vRegIndex = 0; vRegIndex < lastRegIndex; vRegIndex++) {
    int valIndex = valIndices[vRegIndex];
#ifdef EMITTING
    // mulsd "sizeof(double)*valIndex"(%r9), %xmm"vRegIndex"
    emitMULSDmr(Str, sizeof(double)*valIndex, vRegIndex);
#endif
  }
}

void Unfolding::dumpRowConclusion(llvm::MCStreamer *Str, int rowIndex, int lastRegIndex) {
  int registerOffset = 0;
  if(lastRegIndex > 0) {
    dumpRegisterReduce(Str, lastRegIndex);
  }
  if (hadToSplitRow) {
    registerOffset = 15; 
    if(lastRegIndex > 0) {
#ifdef EMITTING
      //  addsd %xmm0, %xmm15
      emitADDSDrr(Str, 0, 15);
#endif
    }
  }  
   
#ifdef EMITTING
  //  addsd "sizeof(double)*rowIndex"(%rsi), %xmm"0|15"
  emitADDSDmr(Str, sizeof(double)*rowIndex, registerOffset);

  //  movsd %xmm0, "sizeof(double)*currentRow"(%rsi)
  Str->EmitInstruction(MCInstBuilder(X86::MOVSDmr).addReg(X86::RSI).addImm(1).addReg(0)
		       .addImm(sizeof(double)*rowIndex).addReg(0).addReg(X86::XMM0+registerOffset));
#endif
}

void Unfolding::dumpPartialRowConclusion(llvm::MCStreamer *Str, int rowIndex, int lastRegIndex) {
  if(lastRegIndex > 0) {
    if (!hadToSplitRow) {
#ifdef EMITTING
      //  xorps %xmm15, %xmm15
      Str->EmitInstruction(MCInstBuilder(X86::XORPSrr).addReg(X86::XMM15).addReg(X86::XMM15).addReg(X86::XMM15));
#endif
    }
    dumpRegisterReduce(Str, lastRegIndex);
#ifdef EMITTING
    //  addsd %xmm0, %xmm15
    emitADDSDrr(Str, 0, 15);
#endif
    hadToSplitRow = true;
  }
}

void Unfolding::dumpRegisterReduce(llvm::MCStreamer *Str, int lastRegIndex) {
  for(int inc=1; inc <= 8; inc *= 2) {
    for(int i=0; i+inc < lastRegIndex; i+=inc*2) {
#ifdef EMITTING
      //  addsd %xmm(i+inc), %xmm(i)
      emitADDSDrr(Str, i+inc, i);
#endif
    }
  }
}

void Unfolding::dumpAssemblyConstData(llvm::MCStreamer *Str) {
  // TODO: This is an ugly hack!!!
  Matrix *matrix = getMatrix();
  matrix->vals = (double*)&(allValues[0]);
  matrix->numVals = allValues.size();
  seenValues.clear();
}

void Unfolding::dumpAssemblyData(llvm::MCStreamer *Str) {
  // do nothing here
}
