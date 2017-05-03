#include "ifElseStmt.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"

using namespace codeGen;

IfElseStmt::~IfElseStmt() {
  delete condition;
  delete thenBlock;
  delete elseBlock;
}

void IfElseStmt::build(){
   llvm::Function* function = Builder->GetInsertBlock()->getParent();
      
   llvm::BasicBlock* thenBasicBlock = llvm::BasicBlock::Create(*C, "", function);
   llvm::BasicBlock* mergeBasicBlock = llvm::BasicBlock::Create(*C, "", function);
   llvm::BasicBlock* elseBasicBlock = (elseBlock != NULL) ? 
       llvm::BasicBlock::Create(*C, "", function) : mergeBasicBlock;
   
   cval condVal = condition->build();
   Builder->CreateCondBr(condVal.val, thenBasicBlock, elseBasicBlock);
   
   Builder->SetInsertPoint(thenBasicBlock);
   
   env->push_scope();
   thenBlock->build();
   env->pop_scope();
   
   if(Builder->GetInsertBlock()->getTerminator() == NULL)
      Builder->CreateBr(mergeBasicBlock);
   
   if(elseBlock != NULL) {
      Builder->SetInsertPoint(elseBasicBlock);   
      env->push_scope();
      elseBlock->build();
      env->pop_scope();   
      if(Builder->GetInsertBlock()->getTerminator() == NULL)
        Builder->CreateBr(mergeBasicBlock);
   }  
  
   Builder->SetInsertPoint(mergeBasicBlock);
}

void IfElseStmt::print(int level) {
  indent(level);
  std::cout << "if(";
  condition->print();
  std::cout << ") {\n";
  thenBlock->print(level+2);
  indent(level);
  if(elseBlock != NULL) {
    std::cout << "} else {\n";
    elseBlock->print(level+2);
    indent(level);
  }
  std::cout << "}\n";
}
