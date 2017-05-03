#ifndef codeGenFrameWork_Types_h
#define codeGenFrameWork_Types_h

#include <string>
#include "llvm/IR/User.h"
#include "constExpr.h"

namespace codeGen {
   
   class Type {      
   public:
      virtual ~Type();
      virtual llvm::Type* getLLVMType() = 0;
      virtual llvm::Constant* getLLVMDefaultValue() = 0;
      virtual llvm::Type* getLLVMTypeForGlobal();
      virtual void print() = 0;
   };
   
   class PtrType: public Type {
   private:
      Type* baseTy;
   public:
      PtrType(Type* baseTy);
      ~PtrType();
      llvm::Type* getLLVMType();
      llvm::Constant* getLLVMDefaultValue();
      virtual void print();
   };
   
   class IntType: public Type {      
   public:
      llvm::Type* getLLVMType();
      llvm::Constant* getLLVMDefaultValue();
      virtual void print();
   };
   
   class DoubleType: public Type {      
   public:
      llvm::Type* getLLVMType();
      llvm::Constant* getLLVMDefaultValue();
      virtual void print();
  };
   
   class BoolType: public Type {      
   public:
      llvm::Type* getLLVMType();
      llvm::Constant* getLLVMDefaultValue();
      virtual void print();
  };
   
   class VoidType: public Type {    
   public:
      llvm::Type* getLLVMType();
      llvm::Constant* getLLVMDefaultValue();
      virtual void print();
   };
  
   class ArrayType: public Type {
   private:
      Type *baseType; 
      int size;
   public:
      ArrayType(Type *baseType);
      ArrayType(Type *baseType, int size);
      ArrayType(Type *baseType, ConstExpr<int> *sizeAst);
      ~ArrayType();
     
      llvm::Type* getLLVMType();
      llvm::Constant* getLLVMDefaultValue();
      llvm::Type* getLLVMTypeForGlobal();
      virtual void print();
   };
}


#endif
