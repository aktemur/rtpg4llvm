#ifndef _CODE_GENERATION
#define _CODE_GENERATION

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/PassManager.h"

#include <string>
#include <vector>
#include "scope.h"
#include "cval.h"

typedef llvm::IRBuilder<> BuilderT;

namespace codeGen {

  class CompileUnit;
  
  class OptimizationPassPopulator {
  public:
    virtual void addOptimizationPasses(llvm::PassManagerBase &MPM, llvm::FunctionPassManager &FPM) = 0;
  };
  
  class DefaultOptimizationPassPopulator : public OptimizationPassPopulator {
  public:
    virtual void addOptimizationPasses(llvm::PassManagerBase &MPM, llvm::FunctionPassManager &FPM);
  };
    
  class CodeGeneration {  
  public:
    virtual ~CodeGeneration();
    static llvm::LLVMContext *C;
    static scope *env;
    static BuilderT *Builder;
    static llvm::Module* TheModule;
    static llvm::ExecutionEngine* TheExecutionEngine;
    
    static void promote(cval& cv1, cval& cv2);
    static void promoteValToType(cval& cv1, llvm::Type *ty2, bool isSigned);
    static void compile(vector<CompileUnit*> *c);
    static void setOptimizationPassPopulator(OptimizationPassPopulator *populator);
    static void setUseDefaultOptimizationPassPopulator();
    static void* getFunctionPointer(string funcName);
  protected:
    static void codeGenerationInit();
    static void optimizeGeneratedCode();
    
    static OptimizationPassPopulator *optimizationPassPopulator;
  };

}

#endif
