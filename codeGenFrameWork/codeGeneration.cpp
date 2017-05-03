#include <iostream>
#include "codeGeneration.h"
#include "compileUnit.h"

#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/CodeGen/LinkAllCodegenComponents.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/JITEventListener.h"
#include "llvm/ExecutionEngine/JITMemoryManager.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/IRReader.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/Memory.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Target/TargetLibraryInfo.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Vectorize.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/ManagedStatic.h"

#ifdef __CYGWIN__
#include <cygwin/version.h>
#if defined(CYGWIN_VERSION_DLL_MAJOR) && CYGWIN_VERSION_DLL_MAJOR<1007
#define DO_NOTHING_ATEXIT 1
#endif
#endif

using namespace codeGen;

BuilderT *CodeGeneration::Builder = NULL;
llvm::LLVMContext *CodeGeneration::C = NULL;
scope *CodeGeneration::env = NULL;
llvm::Module* CodeGeneration::TheModule = NULL;
llvm::ExecutionEngine* CodeGeneration::TheExecutionEngine = NULL;
OptimizationPassPopulator *CodeGeneration::optimizationPassPopulator = NULL;

CodeGeneration::~CodeGeneration() {}

void CodeGeneration::codeGenerationInit(){
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmPrinters();
  llvm::InitializeAllAsmParsers();
  CodeGeneration::TheModule = new llvm::Module("codegen", llvm::getGlobalContext());
  
  std::string ErrorMsg;
  llvm::Module *Mod = CodeGeneration::TheModule;
  if (Mod->MaterializeAllPermanently(&ErrorMsg)) {
    std::cerr << "Could not materialize module.\n";
    std::cerr << "Reason: " << ErrorMsg << "\n";
    exit(1);
  }
  
  llvm::EngineBuilder engineBuilder(Mod);
  engineBuilder.setErrorStr(&ErrorMsg);
  engineBuilder.setEngineKind(llvm::EngineKind::JIT);
  
  // Enable MCJIT
  engineBuilder.setUseMCJIT(true);
  llvm::JITMemoryManager *JMM = new llvm::SectionMemoryManager();
  engineBuilder.setJITMemoryManager(JMM);
  llvm::TargetOptions options;
  options.PrintMachineCode = true;
  options.PositionIndependentExecutable = true;
  engineBuilder.setTargetOptions(options);
  engineBuilder.setOptLevel(llvm::CodeGenOpt::Aggressive);
  
  llvm::ExecutionEngine *EE = engineBuilder.create();
  if (!EE) {
    if (!ErrorMsg.empty())
      std::cerr << "Error creating EE: " << ErrorMsg << "\n";
    else
      std::cerr << "Unknown error creating EE!\n";
    exit(1);
  }
  
  CodeGeneration::TheExecutionEngine = EE;
  // TODO: delete TheExecutionEngine during shutdown  
  
  llvm::TargetMachine *targetMachine = engineBuilder.selectTarget();
  CodeGeneration::TheModule->setDataLayout(targetMachine->getDataLayout()->getStringRepresentation());
  CodeGeneration::TheModule->setTargetTriple(targetMachine->getTargetTriple());
}

void* CodeGeneration::getFunctionPointer(string funcName) {
  llvm::Function *func = CodeGeneration::TheModule->getFunction(funcName);
  if(func == NULL) {
    std::cerr << "Function " << funcName << " does not exist.\n";
    exit(1);
  }
  
  llvm::ExecutionEngine *EE = CodeGeneration::TheExecutionEngine;
  // Clear instruction cache before code will be executed.
  // TODO: JMM->invalidateInstructionCache();

  void* fPtr = EE->getPointerToFunction(func);  
  if(!fPtr) {
    std::cerr << "Function pointer is null!\n";
  }
  return fPtr;
}

void CodeGeneration::compile(vector<CompileUnit*> *c) {
  CodeGeneration::codeGenerationInit();
  
  BuilderT* B = new llvm::IRBuilder<>(llvm::getGlobalContext());
  CodeGeneration::Builder = B;  
  CodeGeneration::C = &llvm::getGlobalContext();  
  CodeGeneration::env = new scope();
  
  for (typeof(c->begin())unit = c->begin(); unit != c->end(); ++unit) {
    (*unit)->build();           
  }
  
  verifyModule(*CodeGeneration::TheModule, llvm::PrintMessageAction);
  
  if(optimizationPassPopulator != NULL) {
    optimizeGeneratedCode();
  }
}

// This method is copied and adapted from opt.cpp
void CodeGeneration::optimizeGeneratedCode() {  
  // Initialize passes
  llvm::PassRegistry &Registry = *llvm::PassRegistry::getPassRegistry();
  llvm::initializeCore(Registry);
  llvm::initializeScalarOpts(Registry);
  llvm::initializeVectorization(Registry);
  llvm::initializeIPO(Registry);
  llvm::initializeAnalysis(Registry);
  llvm::initializeIPA(Registry);
  llvm::initializeTransformUtils(Registry);
  llvm::initializeInstCombine(Registry);
  llvm::initializeInstrumentation(Registry);
  llvm::initializeTarget(Registry);
  
  // Create a PassManager to hold and optimize the collection of passes we are
  // about to build.
  //
  llvm::PassManager Passes;
  llvm::FunctionPassManager FPasses(CodeGeneration::TheModule);
  
  // Add an appropriate TargetLibraryInfo pass for the module's triple.
  llvm::TargetLibraryInfo *TLI = new llvm::TargetLibraryInfo(llvm::Triple(CodeGeneration::TheModule->getTargetTriple()));
  Passes.add(TLI);
  
  // Add an appropriate TargetData instance for this module.
  const llvm::DataLayout *DL = CodeGeneration::TheExecutionEngine->getDataLayout();
  if (DL) {
    Passes.add(new llvm::DataLayout(*DL));
    FPasses.add(new llvm::DataLayout(*DL));
  }

  optimizationPassPopulator->addOptimizationPasses(Passes, FPasses);
  
  FPasses.doInitialization();
  for (llvm::Module::iterator F = CodeGeneration::TheModule->begin(), E = CodeGeneration::TheModule->end(); F != E; ++F)
    FPasses.run(*F);
  FPasses.doFinalization();
  
  // Now that we have all of the passes ready, run them.
  Passes.run(*CodeGeneration::TheModule);
}

void CodeGeneration::setOptimizationPassPopulator(OptimizationPassPopulator *populator) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
  if(optimizationPassPopulator != NULL)
    delete optimizationPassPopulator;
  optimizationPassPopulator = populator;
#pragma clang diagnostic pop
}

void CodeGeneration::setUseDefaultOptimizationPassPopulator() {
  setOptimizationPassPopulator(new DefaultOptimizationPassPopulator);
}

void DefaultOptimizationPassPopulator::addOptimizationPasses(llvm::PassManagerBase &MPM, 
                                                             llvm::FunctionPassManager &FPM) {
  std::cerr << "DefaultOptimizationPass populator not implemented yet!\n";
  exit(1);
}

void CodeGeneration::promote(cval& cv1, cval& cv2) {
  if (cv1.val == NULL || cv2.val == NULL) {
    return;
  }
  
  promoteValToType(cv1, cv2.val->getType(), cv2.is_signed);
  promoteValToType(cv2, cv1.val->getType(), cv1.is_signed);
}

void CodeGeneration::promoteValToType(cval& cv1, llvm::Type *ty2, bool isSigned) {
  llvm::Type* ty1 = (llvm::Type*)cv1.val->getType();
  
  // if either is double, convert to double
  if (ty1->isDoubleTy()) {
    // result will be double
    if (!(ty2->isDoubleTy() || ty2->isFloatTy() || ty2->isIntegerTy())) {
      std::cerr << "Cannot combine double and other" << endl;
    }
  }else if (ty2->isDoubleTy()) {
    // result will be double.
    if (ty1->isDoubleTy()) {
    } else if (ty1->isFloatTy()) {
      cv1.val = Builder->CreateFPExt(cv1.val, ty2);
    } else if (ty1->isIntegerTy()) {
      if (cv1.is_signed) {
        cv1.val = Builder->CreateSIToFP(cv1.val, ty2);
      } else {
        cv1.val = Builder->CreateUIToFP(cv1.val, ty2);
      }
    } else {
      std::cerr << "Cannot combine other and double" << endl;
    }
  }
  // if either is float, convert to float
  else if (ty1->isFloatTy()) {
    // result wil be float
    if (!(ty2->isFloatTy() || ty2->isIntegerTy())) {
      std::cerr << "Cannot combine float and other" << endl;
    }
  }else if (ty2->isFloatTy()) {
    // result will be float.
    if (ty1->isFloatTy()) {
    } else if (ty1->isIntegerTy()) {
      if (cv1.is_signed) {
        cv1.val = Builder->CreateSIToFP(cv1.val, ty2);
      } else {
        cv1.val = Builder->CreateUIToFP(cv1.val, ty2);
      }
    } else {
      std::cerr << "Cannot combine other and float" << endl;
    }
  }
  // otherwise, convert to type of
  // 1) larger type involved, if >= int
  // 2) unsigned type, if two types of equal size >= int
  // 3) int
  else if (ty1->isIntegerTy() && ty2->isIntegerTy()) {
    unsigned size1 = ty1->getPrimitiveSizeInBits();
    unsigned size2 = ty2->getPrimitiveSizeInBits();
    
    if (size1 < 32 && size2 < 32) {
      // convert to int.
      llvm::Type* int_ty = (llvm::Type*)llvm::TypeBuilder<int, false>::get(*C);
      if (cv1.is_signed) {
        cv1.val = Builder->CreateSExt(cv1.val, int_ty);
      } else {
        cv1.val = Builder->CreateZExt(cv1.val, int_ty);
      }
      cv1.is_signed = true;
    } else if (size2 > size1) {
      // convert to type 2
      if (cv1.is_signed) {
        cv1.val = Builder->CreateSExt(cv1.val, ty2);
      } else {
        cv1.val = Builder->CreateZExt(cv1.val, ty2);
      }
      cv1.is_signed = isSigned;
    } else {
      // convert to unsigned, if either is unsigned
      bool rsigned = cv1.is_signed && isSigned;
      cv1.is_signed = rsigned;
    }
  }
}

