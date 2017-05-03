#include "whileStmt.h"

using namespace codeGen;

WhileStmt::~WhileStmt() {
  delete loopCondition;
  delete loopBody;
}

void WhileStmt::build(){
   llvm::Function* function = Builder->GetInsertBlock()->getParent();

   llvm::BasicBlock* condBlock = llvm::BasicBlock::Create(*C, "", function);
   llvm::BasicBlock* loopBlock = llvm::BasicBlock::Create(*C, "", function);

   Builder->CreateBr(condBlock);
   Builder->SetInsertPoint(condBlock);

   cval condCheck = loopCondition->build();
   Builder->SetInsertPoint(loopBlock);
   
   env->push_scope();
   loopBody->build();
   env->pop_scope();

   if(Builder->GetInsertBlock()->getTerminator() == NULL)
      Builder->CreateBr(condBlock);
  
   llvm::BasicBlock* afterLoopBlock = llvm::BasicBlock::Create(*C, "", function);
   Builder->SetInsertPoint(condBlock);
   Builder->CreateCondBr(condCheck.val, loopBlock, afterLoopBlock); 
   Builder->SetInsertPoint(afterLoopBlock);
}

void WhileStmt::print(int level) {
  indent(level);
  std::cout << "while(";
  loopCondition->print();
  std::cout << ") {\n";
  loopBody->print(level+2);
  indent(level);
  std::cout << "}\n";
}