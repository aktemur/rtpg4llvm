#include "types.h"
#include "codeGeneration.h"

using namespace codeGen;

Type::~Type() {}

llvm::Type* Type::getLLVMTypeForGlobal() {
  return getLLVMType();
}

llvm::Type* IntType::getLLVMType(){
    return llvm::Type::getInt32Ty(*CodeGeneration::C);
}

void IntType::print() {
  std::cout << "int";
}

llvm::Type* DoubleType::getLLVMType(){
    return llvm::Type::getDoubleTy(*CodeGeneration::C);
}

void DoubleType::print() {
  std::cout << "double";
}

llvm::Type* BoolType::getLLVMType(){
    return llvm::Type::getInt1Ty(*CodeGeneration::C);
}

void BoolType::print() {
  std::cout << "bool";
}

llvm::Type* VoidType::getLLVMType(){
  return llvm::Type::getVoidTy(*CodeGeneration::C);
}

void VoidType::print() {
  std::cout << "void";
}

PtrType::PtrType(Type* baseTy) : baseTy(baseTy) {}

PtrType::~PtrType() {
  delete baseTy;
}

llvm::Type* PtrType::getLLVMType(){
    llvm::Type* baseType = baseTy->getLLVMType();
    return llvm::PointerType::get(baseType, 0);
}

void PtrType::print() {
  baseTy->print();
  std::cout << "*";
}

ArrayType::ArrayType(Type *baseType): baseType(baseType){
  size = -1;
}

ArrayType::ArrayType(Type *baseType, int size): 
  baseType(baseType), size(size) {}

ArrayType::ArrayType(Type *baseType, ConstExpr<int> *sizeAst): 
  baseType(baseType) {
  size = sizeAst->getVal();
}

ArrayType::~ArrayType() {
  delete baseType;
}

llvm::Type* ArrayType::getLLVMType(){
    llvm::Type* base = baseType->getLLVMType();
    if(size < 0) {
        return llvm::PointerType::get(base, 0);
    } else {
        return llvm::ArrayType::get(base, size);
    }
}

llvm::Type* ArrayType::getLLVMTypeForGlobal() {
  llvm::Type* base = baseType->getLLVMTypeForGlobal();
  if(size < 0) {
    return llvm::ArrayType::get(base, 0);
  } else {
    return llvm::ArrayType::get(base, size);
  }
}

void ArrayType::print() {
  baseType->print();
  std::cout << "[";
  if(size >= 0)
    std::cout << size;
  std::cout << "]";
}

llvm::Constant* IntType::getLLVMDefaultValue(){
  return llvm::ConstantInt::getNullValue(getLLVMType());
}

llvm::Constant* DoubleType::getLLVMDefaultValue(){
  return llvm::ConstantFP::getNullValue(getLLVMType());
}

llvm::Constant* BoolType::getLLVMDefaultValue(){
  return llvm::ConstantInt::getNullValue(getLLVMType());
}

llvm::Constant* VoidType::getLLVMDefaultValue(){
  return llvm::ConstantInt::getNullValue(getLLVMType());
}

llvm::Constant* PtrType::getLLVMDefaultValue(){
  return llvm::ConstantPointerNull::getNullValue(getLLVMType());
}

llvm::Constant* ArrayType::getLLVMDefaultValue(){
  return llvm::ConstantArray::getNullValue(getLLVMType());
}

