#include "codeGenConstructors.h"
#include "spMVgen.h"
#include <iostream>
#include <fstream>
#include <sstream>

extern bool DEBUG;
extern bool DUMP_ASM;
extern bool DUMP_OBJECT;

// Global constant determined by experiments with compilation times
#define WEIGHT_PER_FUNCTION 200

typedef void(*matrixValueSetterFun)(int*, int*, double*);

using namespace codeGen;
using namespace spMVgen;

int SpMVSpecializer::numFunctionsGenerated = 0;

int SpMVSpecializer::stage = 1;

StmtCode* WeightedStmt::getStmt() {
  return stmt;
}

int WeightedStmt::getWeight() {
  return weight;
}

MMMatrix* SpMVSpecializer::getMMMatrix() {
  return mmMatrix;
}

Matrix* SpMVSpecializer::getMatrix() {
  if(matrix == NULL) {
    matrix = getMatrixForGeneration();
  }
  return matrix;
}

string SpMVSpecializer::newName(const string &str, int i) {
  std::ostringstream oss;
  oss << i;
  string name(str);
  return name + oss.str();
}

string SpMVSpecializer::newName(const string &str, int i, int j) {
  std::ostringstream oss;
  oss << i << '_' << j;
  string name(str);
  return name + oss.str();
}

multByMFun SpMVSpecializer::gen() {
  numFunctionsGenerated = 0;
  stage = 1;
  CompileUnit *allDecls = getAllDecls();

  // Generate calls for generated functions
  StmtCode *funCalls = $stmt< ; >$;
  for(int i=1; i <= numFunctionsGenerated; ++i) {
    string name = newName("generated_", i);
    funCalls = $stmt< 
      `stmt(funCalls`)
      `name(name`) (v, w);
    >$;
  }

  // Generate the multByM function
  CompileUnit *program = $compunit<
    `unit(allDecls`)
    void multByM(double *v, double *w) {
      int i;
      for (i=0; i< `int(mmMatrix->n`); i++) w[i] = 0.0;
      `stmt(funCalls`)
    }
  >$;

  //program->print();
  setOptimizationPasses();
  program->compile();

  //exit(1);
  delete program;

  multByMFun funPtr = (multByMFun) CodeGeneration::getFunctionPointer("multByM");

  return funPtr;
}

void SpMVSpecializer::setOptimizationPasses() {
  CodeGeneration::setUseDefaultOptimizationPassPopulator();
}

CompileUnit* SpMVSpecializer::getAllDecls() {
  if(mmMatrix->elts.size() == 0) return $compunit< >$;
 
  Matrix *matrix = getMatrix();
  CompileUnit *gDecls = getGlobalDecls(matrix);
  std::queue<WeightedStmt> *stmts = getBody(matrix);
  CompileUnit *genFuns = splitIntoFunctions(stmts);
  CompileUnit *allDecls = $compunit<
    `unit(gDecls`)
    `unit(genFuns`)
  >$;
  delete matrix;
  delete stmts;
  return allDecls;
}

CompileUnit* SpMVSpecializer::getGlobalDecls(Matrix *matrix) {
  string colsName = newName("cols", stage);
  string rowsName = newName("rows", stage);
  string valsName = newName("vals", stage);

  return $compunit<
    int `colsName [`int(matrix->numCols`)] = `list(matrix->cols, matrix->numCols`);
    int `rowsName [`int(matrix->numRows`)]  = `list(matrix->rows, matrix->numRows`);
    double `valsName [`int(matrix->numVals`)] = `list(matrix->vals, matrix->numVals`);
  >$;
}

CompileUnit* SpMVSpecializer::splitIntoFunctions(std::queue<WeightedStmt> *stmts) {
  CompileUnit *units = $compunit< >$;
  StmtCode *body = $stmt< ; >$;
  int weight = 0;

  while(true) {
    if(weight >= WEIGHT_PER_FUNCTION || stmts->empty()) {
      string name = newName("generated_", ++numFunctionsGenerated);
      units = $compunit<
        `unit(units`)
	void `name(name`) (double * __restrict__ v, double * __restrict__ w) {
        `stmt(getLocalDecls()`)
	  `stmt(body`)
        }
      >$;
      weight = 0;
      body = $stmt< ; >$;
    }
    if(stmts->empty()) break;

    body = $stmt<
      `stmt(body`)
      `stmt(stmts->front().getStmt()`)
    >$;
    weight += stmts->front().getWeight();
    stmts->pop();
  }
  
  return units;
}


// Caller of this method is responsible for destructing the 
// returned matrix. 
MMMatrix* SpMVSpecializer::readMatrixFromFile(string fileName) {
  ifstream mmFile(fileName.c_str());
  if (!mmFile.is_open()) {
    std::cerr << "Problem with file " << fileName << ".\n";
    exit(1);
  }
  string headerLine = "";
  // consume the comments until we reach the size info
  while (mmFile.good()) {
    getline (mmFile, headerLine);
    if (headerLine[0] != '%') break;
  }
  
  // Read N, M, NZ
  stringstream header(headerLine, ios_base::in);
  int n, m, nz;
  header >> n >> m >> nz;
  if (n != m) {
    std::cerr << "Only square matrices are accepted.\n";
    exit(1);
  }
  
  // Read rows, cols, vals
  MMMatrix *matrix = new MMMatrix(n);
  int row; int col; double val;
  
  for (int i = 0; i < nz; ++i) {
    mmFile >> row >> col >> val;
    // adjust to zero index
    matrix->add(row-1, col-1, val);
  }
  mmFile.close();
  matrix->normalize();
  return matrix;
}

#include "llvm/ADT/OwningPtr.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCParser/AsmLexer.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSectionMachO.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCTargetAsmParser.h"
#include "llvm/MC/SubtargetFeature.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileUtilities.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/system_error.h"
#include "lib/Target/X86/X86.h"
#include "lib/Target/X86/X86TargetObjectFile.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/ExecutionEngine/JITMemoryManager.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/ExecutionEngine/RuntimeDyld.h"
#include "llvm/ExecutionEngine/ObjectImage.h"

using namespace llvm;

// A trivial memory manager that doesn't do anything fancy, just uses the
// support library allocation routines directly.
class TrivialMemoryManager : public RTDyldMemoryManager {
public:
  SmallVector<sys::MemoryBlock, 16> FunctionMemory;
  SmallVector<sys::MemoryBlock, 16> DataMemory;

  uint8_t *allocateCodeSection(uintptr_t Size, unsigned Alignment,
                               unsigned SectionID);
  uint8_t *allocateDataSection(uintptr_t Size, unsigned Alignment,
                               unsigned SectionID, bool IsReadOnly);

  virtual void *getPointerToNamedFunction(const std::string &Name,
                                          bool AbortOnFailure = true) {
    return 0;
  }

  bool applyPermissions(std::string *ErrMsg) { return false; }

  // Invalidate instruction cache for sections with execute permissions.
  // Some platforms with separate data cache and instruction cache require
  // explicit cache flush, otherwise JIT code manipulations (like resolved
  // relocations) will get to the data cache but not to the instruction cache.
  virtual void invalidateInstructionCache();
};

uint8_t *TrivialMemoryManager::allocateCodeSection(uintptr_t Size,
                                                   unsigned Alignment,
                                                   unsigned SectionID) {
  sys::MemoryBlock MB = sys::Memory::AllocateRWX(Size, 0, 0);
  FunctionMemory.push_back(MB);
  return (uint8_t*)MB.base();
}

uint8_t *TrivialMemoryManager::allocateDataSection(uintptr_t Size,
                                                   unsigned Alignment,
                                                   unsigned SectionID,
                                                   bool IsReadOnly) {
  sys::MemoryBlock MB = sys::Memory::AllocateRWX(Size, 0, 0);
  DataMemory.push_back(MB);
  return (uint8_t*)MB.base();
}

void TrivialMemoryManager::invalidateInstructionCache() {
  for (int i = 0, e = FunctionMemory.size(); i != e; ++i)
    sys::Memory::InvalidateInstructionCache(FunctionMemory[i].base(),
                                            FunctionMemory[i].size());

  for (int i = 0, e = DataMemory.size(); i != e; ++i)
    sys::Memory::InvalidateInstructionCache(DataMemory[i].base(),
                                            DataMemory[i].size());
}

std::string SpMVSpecializer::mainFunctionName = "multByM";
std::string SpMVSpecializer::matrixValueSetterFunctionName = "setMatrixData";
std::vector<void*> SpMVSpecializer::generatedFunctions;

// adapted from llvm-rtdyld.cpp
void SpMVSpecializer::loadBuffer(ObjectBuffer *Buffer) {
  // Instantiate a dynamic linker.
  TrivialMemoryManager *MemMgr = new TrivialMemoryManager;
  RuntimeDyld *Dyld = new RuntimeDyld(MemMgr);

  START_OPTIONAL_TIME_PROFILE(LoadObjectToMemory);
  // Load the input memory buffer.
  ObjectImage *LoadedObject = Dyld->loadObject(Buffer);
  if (!LoadedObject) {
    std::cerr << "Dyld error:" << Dyld->getErrorString().str() << "\n";
    exit(1);
  }
  END_OPTIONAL_TIME_PROFILE(LoadObjectToMemory);

  // Resolve all the relocations we can.
  Dyld->resolveRelocations();
  // Clear instruction cache before code will be executed.
  MemMgr->invalidateInstructionCache();
  // FIXME: Error out if there are unresolved relocations.

  void *multByMFunction = Dyld->getSymbolAddress(mainFunctionName);
  if (multByMFunction == 0) {
    std::cerr << "no definition for '" << mainFunctionName << "'\n";
    exit(1);
  }
  generatedFunctions.push_back(multByMFunction);

  for (int i = 1; i <= stage; ++i) {
    std::string setterFunctionName = newName(matrixValueSetterFunctionName, i);
    void *matrixValueSetterFunction = Dyld->getSymbolAddress(setterFunctionName);
    if (matrixValueSetterFunction != 0) {
      generatedFunctions.push_back(matrixValueSetterFunction);
    }
  }

  // Invalidate the instruction cache for each loaded function.
  for (unsigned i = 0, e = MemMgr->FunctionMemory.size(); i != e; ++i) {
    sys::MemoryBlock &Data = MemMgr->FunctionMemory[i];
    // Make sure the memory is executable.
    std::string ErrorStr;
    sys::Memory::InvalidateInstructionCache(Data.base(), Data.size());
    if (!sys::Memory::setExecutable(Data, &ErrorStr)) {
      std::cerr << "unable to mark function executable: '" << ErrorStr << "'\n";
      exit(1);
    }
  }
}


// This method is adapted from llvm-mc.cpp
// TODO: Memory management.
multByMFun SpMVSpecializer::genByEmittingAssembly() {
  // Initialize targets and assembly printers/parsers.
  
  START_OPTIONAL_TIME_PROFILE(InitializeLLVMTarget);
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllDisassemblers();
  
  // Figure out the target triple.
  Triple TheTriple(Triple::normalize(sys::getDefaultTargetTriple()));
  // Get the target specific parser.
  std::string Error;
  const Target *TheTarget = TargetRegistry::lookupTarget(/*ArchName*/ "", TheTriple, Error);
  if (!TheTarget) {
    std::cerr << "Target cannot be found: " << Error;
    exit(1);
  }
  std::string TripleName = TheTriple.getTriple();
  END_OPTIONAL_TIME_PROFILE(InitializeLLVMTarget);
  
  SourceMgr SrcMgr;
  SrcMgr.AddNewSourceBuffer(MemoryBuffer::getMemBuffer(""), SMLoc());
  
  llvm::OwningPtr<MCAsmInfo> MAI(TheTarget->createMCAsmInfo(TripleName));
  assert(MAI && "Unable to create target asm info!");
  
  llvm::OwningPtr<MCRegisterInfo> MRI(TheTarget->createMCRegInfo(TripleName));
  assert(MRI && "Unable to create target register info!");
  
  TargetLoweringObjectFile *mcObjectFileInfo = NULL;
  if(TheTriple.getArch() == Triple::x86_64 && TheTriple.getOS() == Triple::Darwin) {
    mcObjectFileInfo = new X86_64MachoTargetObjectFile();
  } else if(TheTriple.getArch() == Triple::x86_64 && TheTriple.getOS() == Triple::Linux) {
    mcObjectFileInfo = new X86LinuxTargetObjectFile();
  } else {
    std::cerr << "Only X86_64 on Linux or MacOS is supported.\n";
    exit(1);
  }
  
  START_OPTIONAL_TIME_PROFILE(MCObjectInit);
  OwningPtr<TargetLoweringObjectFile> MOFI(mcObjectFileInfo);
  MCContext Ctx(*MAI, *MRI, MOFI.get(), &SrcMgr);
  MOFI->InitMCObjectFileInfo(TripleName, Reloc::Default, CodeModel::Default, Ctx);
  END_OPTIONAL_TIME_PROFILE(MCObjectInit);
  
  START_OPTIONAL_TIME_PROFILE(MCInitOper);
  SmallVector<char, 1024*1024*4> *smallVector = new SmallVector<char, 1024*1024*4>();
  raw_svector_ostream svectorOS(*smallVector);
  {
    formatted_raw_ostream FOS(svectorOS);
    OwningPtr<MCStreamer> Str;
    
    OwningPtr<MCInstrInfo> MCII(TheTarget->createMCInstrInfo());
    OwningPtr<MCSubtargetInfo> STI(TheTarget->createMCSubtargetInfo(TripleName, /*MCPU*/ "", /*FeaturesStr*/ ""));
    
    bool emitAssemblyFile = DUMP_ASM; // If false, emits object file.
    if (emitAssemblyFile) {
      MCInstPrinter *IP = TheTarget->createMCInstPrinter(/*OutputAsmVariant*/ 0, *MAI, *MCII, *MRI, *STI);
      MCCodeEmitter *CE = 0;
      MCAsmBackend *MAB = 0;
      Str.reset(TheTarget->createAsmStreamer(Ctx, FOS, /*asmverbose*/true,
                                             /*useLoc*/ true,
                                             /*UseCFI*/ true,
                                             /*useDwarfDirectory*/ true,
                                             IP, CE, MAB, /*showInst*/ false));
    } else {
      MCCodeEmitter *CE = TheTarget->createMCCodeEmitter(*MCII, *MRI, *STI, Ctx);
      MCAsmBackend *MAB = TheTarget->createMCAsmBackend(TripleName, /*CPU*/ "");
      Str.reset(TheTarget->createMCObjectStreamer(TripleName, Ctx, *MAB,
                                                  FOS, CE, /*RelaxAll*/false,
                                                  /*NoExecStack*/false));
    }
    END_OPTIONAL_TIME_PROFILE(MCInitOper);
    
    START_OPTIONAL_TIME_PROFILE(MCOurCode);
    stage = 1;
    dumpAssemblyTextHeader(&*Str);
    dumpAssemblyText(&*Str); // Template method to be filled in by subclasses
    dumpAssemblyTextFooter(&*Str);

    stage = 1;
    dumpMatrixValueSetterFunction(&*Str);

    stage = 1;
    dumpAssemblyConstDataHeader(&*Str);
    dumpAssemblyConstData(&*Str); // Template method to be filled in by subclasses

    stage = 1;
    dumpAssemblyDataHeader(&*Str);
    dumpAssemblyData(&*Str); // Template method to be filled in by subclasses
    END_OPTIONAL_TIME_PROFILE(MCOurCode);
    
    OwningPtr<MCAsmParser> Parser(createMCAsmParser(SrcMgr, Ctx, *Str, *MAI));
    OwningPtr<MCTargetAsmParser> TAP(TheTarget->createMCAsmParser(*STI, *Parser));
    if (!TAP) {
      std::cerr << "Could not create TAP: this target does not support assembly parsing.\n";
      exit(1);
    }
    
    START_OPTIONAL_TIME_PROFILE(MCInitOperParserRun);
    Parser->setTargetParser(*TAP.get());
    int Res = Parser->Run(/*NoInitialTextSection*/ true);
    END_OPTIONAL_TIME_PROFILE(MCInitOperParserRun);
    
    svectorOS.flush();
  }
  
  if (DUMP_ASM || DUMP_OBJECT) {
    std::cout << svectorOS.str().str();
    exit(1);
  }
  
  MemoryBuffer *memBuffer = MemoryBuffer::getMemBuffer(svectorOS.str(), "", false);
  ObjectBuffer *Buffer = new ObjectBuffer(memBuffer);
  loadBuffer(Buffer);

  multByMFun fptr = (multByMFun)generatedFunctions[0];
  stage = 1;
  setMatrixValuesInGeneratedAssembly();
  generatedFunctions.clear();

  return fptr;
}

void SpMVSpecializer::setMatrixValuesInGeneratedAssembly() {
  if(getMMMatrix()->elts.size() > 0) {
    Matrix *matrix = getMatrix();
    matrixValueSetterFun fptr = (matrixValueSetterFun)generatedFunctions[stage];
    fptr(matrix->rows, matrix->cols, matrix->vals);
  }
}

void SpMVSpecializer::dumpMatrixValueSetterFunction(llvm::MCStreamer *Str) {
  MCContext &Ctx = Str->getContext();
  string uscore = "_";
  // .globl setMatrixData"stage"
  Str->EmitSymbolAttribute(Ctx.GetOrCreateSymbol(StringRef(newName(matrixValueSetterFunctionName, stage))), MCSA_Global);
  Str->EmitSymbolAttribute(Ctx.GetOrCreateSymbol(StringRef(newName(uscore + matrixValueSetterFunctionName, stage))), MCSA_Global);
  // .align 4, 0x90
  Str->EmitCodeAlignment(16);
  // setMatrixData"stage":
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(newName(matrixValueSetterFunctionName, stage))));
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(newName(uscore + matrixValueSetterFunctionName, stage))));
  // .cfi_startproc
  Str->EmitCFIStartProc();

  // _rows
  Str->EmitInstruction(MCInstBuilder(X86::MOV64mr).addReg(X86::RIP).addImm(1).addReg(0)
                       .addExpr(MCSymbolRefExpr::Create(newName("_rows", stage), MCSymbolRefExpr::VK_None, Ctx))
                       .addReg(0).addReg(X86::RDI));
  // _cols
  Str->EmitInstruction(MCInstBuilder(X86::MOV64mr).addReg(X86::RIP).addImm(1).addReg(0)
                       .addExpr(MCSymbolRefExpr::Create(newName("_cols", stage), MCSymbolRefExpr::VK_None, Ctx))
                       .addReg(0).addReg(X86::RSI));
  // _vals
  Str->EmitInstruction(MCInstBuilder(X86::MOV64mr).addReg(X86::RIP).addImm(1).addReg(0)
                       .addExpr(MCSymbolRefExpr::Create(newName("_vals", stage), MCSymbolRefExpr::VK_None, Ctx))
                       .addReg(0).addReg(X86::RDX));

  // ret
  Str->EmitInstruction(MCInstBuilder(X86::RET));
  // .cfi_endproc
  Str->EmitCFIEndProc();
}

void SpMVSpecializer::dumpAssemblyTextHeader(llvm::MCStreamer *Str) {
  MCContext &Ctx = Str->getContext();
  string uscore = "_";

  Str->setAutoInitSections(false);
  // .text 
  Str->SwitchSection(Ctx.getObjectFileInfo()->getTextSection());

  // .globl multByM
  Str->EmitSymbolAttribute(Ctx.GetOrCreateSymbol(StringRef(mainFunctionName)), MCSA_Global);
  Str->EmitSymbolAttribute(Ctx.GetOrCreateSymbol(StringRef(uscore + mainFunctionName)), MCSA_Global);
  // .align 4, 0x90
  Str->EmitCodeAlignment(16);
  // multByM:
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(mainFunctionName)));
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(uscore + mainFunctionName)));
  // .cfi_startproc
  Str->EmitCFIStartProc();
}

void SpMVSpecializer::dumpAssemblyTextFooter(llvm::MCStreamer *Str) {
  // ret
  Str->EmitInstruction(MCInstBuilder(X86::RET));
  // .cfi_endproc
  Str->EmitCFIEndProc();
}

void SpMVSpecializer::dumpAssemblyConstDataHeader(llvm::MCStreamer *Str) {
  MCContext &Ctx = Str->getContext();
  //  .section .rodata
  const TargetLoweringObjectFile *tlof = static_cast<const TargetLoweringObjectFile*>(Ctx.getObjectFileInfo()); 
  Str->SwitchSection(tlof->getSectionForConstant(SectionKind::getReadOnly()));  
  // .globl n
  Str->EmitSymbolAttribute(Ctx.GetOrCreateSymbol(StringRef("n")), MCSA_Global);
  Str->EmitSymbolAttribute(Ctx.GetOrCreateSymbol(StringRef("_n")), MCSA_Global);
  // .align 4
  Str->EmitCodeAlignment(4);
  // n:
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef("n")));
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef("_n")));
  // .long "N"
  Str->EmitIntValue(mmMatrix->n, sizeof(int));
}

void SpMVSpecializer::dumpAssemblyDataHeader(llvm::MCStreamer *Str) {
  MCContext &Ctx = Str->getContext();
  // .data section
  Str->SwitchSection(Ctx.getObjectFileInfo()->getDataSection());

  // .align 8
  Str->EmitCodeAlignment(8);
  // _rows
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(newName("_rows", stage))));
  // .quad 0
  Str->EmitIntValue(0, sizeof(int*));

  // .align 8
  Str->EmitCodeAlignment(8);
  // _cols
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(newName("_cols", stage))));
  // .quad 0
  Str->EmitIntValue(0, sizeof(int*));

  // .align 8
  Str->EmitCodeAlignment(8);
  // _vals
  Str->EmitLabel(Ctx.GetOrCreateSymbol(StringRef(newName("_vals", stage))));
  // .quad 0
  Str->EmitIntValue(0, sizeof(double*));
}

void SpMVSpecializer::dumpAssemblyText(llvm::MCStreamer *Str) {
  std::cout << "DumpAssemblyText for this specializer not implemented yet.\n";
  exit(1);
}

void SpMVSpecializer::dumpAssemblyConstData(llvm::MCStreamer *Str) {
  std::cout << "DumpAssemblyConstData for this specializer not implemented yet.\n";
  exit(1);
}

void SpMVSpecializer::dumpAssemblyData(llvm::MCStreamer *Str) {
  std::cout << "DumpAssemblyData for this specializer not implemented yet.\n";
  exit(1);
}


