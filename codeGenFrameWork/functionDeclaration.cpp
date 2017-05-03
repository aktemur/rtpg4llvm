#include "functionDeclaration.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Analysis/Verifier.h"

using namespace codeGen;

FuncDeclaration::~FuncDeclaration() {
  delete returnType;
  for (typeof(params->begin())s = params->begin(); s != params->end(); ++s) {
    delete (*s);
  }
  delete params;
  delete body;
}

void FuncDeclaration::build(){
  llvm::Function *function = TheModule->getFunction(funcName);
   if(function != NULL) {
      std::cerr << "Redefinition of function " << funcName << ".\n";
      exit(1);
   }
  
   /*
    * create function using LLVM library Function class
    * signature -> returnType funcName (params)
    */
   vector<llvm::Type*> parameters;

   for (typeof(params->begin())s = params->begin(); s != params->end(); ++s) {
      parameters.push_back((*s)->getType()->getLLVMType());
   }

   llvm::FunctionType *FT = llvm::FunctionType::get(returnType->getLLVMType(), parameters, false);

   function = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, funcName, TheModule);

   // Set names for all arguments.
   unsigned Idx = 0;
   for (llvm::Function::arg_iterator AI = function->arg_begin(); Idx != params->size(); ++AI, ++Idx) {
      AI->setName((*params)[Idx]->getName());    
      if((*params)[Idx]->isRestrict()) {
        llvm::Attribute::AttrKind newAttr[1] = {llvm::Attribute::NoAlias}; 
        AI->addAttr(llvm::AttributeSet::get(*C, 0, newAttr));
      }
   }
  
   // creating block
   llvm::BasicBlock* BB = llvm::BasicBlock::Create(*C, "entry", function);
   Builder->SetInsertPoint(BB);
  
   env->push_scope();
   
   // allocate and store arguments
   llvm::Function::ArgumentListType::iterator it = function->arg_begin();
   for (typeof(params->begin())s = params->begin(); s != params->end(); ++s) {
      cval ret = _make_signed(Builder->CreateAlloca((*s)->getType()->getLLVMType()));
      ret.val->setName((*s)->getName() + ".addr");
      Builder->CreateStore(it++, ret.val);
      env->put((*s)->getName(), ret);
   }

   // Build funCode
   body->build();
   
   // Insert a terminator for void functions if needed
   llvm::BasicBlock* currentBlock = Builder->GetInsertBlock();   
   if (currentBlock->getTerminator() == NULL){
      if (llvm::Type::VoidTyID == returnType->getLLVMType()->getTypeID()){
         Builder->CreateRetVoid();
      }
   }

   verifyFunction(*function);
   env->pop_scope();
}

void FuncDeclaration::print() {
  returnType->print();
  std::cout << " " << funcName << "(";
  typeof(params->begin()) paramIt = params->begin(), paramEnd =  params->end();
  
  while(true) {
    (*paramIt)->print();
    ++paramIt;
    if(paramIt != paramEnd) 
      std::cout << ", ";
    else 
      break;    
  }
  std::cout << ") {\n";
  body->print(2);
  std::cout << "}\n\n";
}
