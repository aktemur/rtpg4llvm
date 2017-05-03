#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include "codeGenConstructors.h"
#include <stdarg.h>

using namespace codeGen;

vector<ExprCode*>* codeGen::mkExprList(int n, ...) {
  vector<ExprCode*> *vec = new vector<ExprCode*>;
  va_list ap;
  va_start(ap, n); 
  for(int j=0; j < n; j++) {
    vec->push_back(va_arg(ap, ExprCode*)); 
  }
  va_end(ap);
  return vec;
}

StmtCode* codeGen::mkSeqStmt(int n, ...) {
  vector<StmtCode*> *vec = new vector<StmtCode*>;
  va_list ap;
  va_start(ap, n); 
  for(int j=0; j < n; j++) {
    vec->push_back(va_arg(ap, StmtCode*)); 
  }
  va_end(ap);
  if(n == 1) return vec->at(0);
  else return new SeqStmt(vec);
}

StmtCode* codeGen::mkScopedStmt(int n, ...) {
  vector<StmtCode*> *vec = new vector<StmtCode*>;
  va_list ap;
  va_start(ap, n); 
  for(int j=0; j < n; j++) {
    vec->push_back(va_arg(ap, StmtCode*)); 
  }
  va_end(ap);
  return new ScopedStmt(vec);
}

vector<FuncParam*>* codeGen::mkFormals(int n, ...) {
  vector<FuncParam*> *vec = new vector<FuncParam*>;
  va_list ap;
  va_start(ap, n); 
  for(int j=0; j < n; j++) {
    vec->push_back(va_arg(ap, FuncParam*)); 
  }
  va_end(ap);
  return vec;  
}

Program* codeGen::mkProgram(int n, ...) {
  vector<CompileUnit*> *vec = new vector<CompileUnit*>;
  va_list ap;
  va_start(ap, n); 
  for(int j=0; j < n; j++) {
    vec->push_back(va_arg(ap, CompileUnit*)); 
  }
  va_end(ap);
  return new Program(vec);
}

ExprCode* codeGen::mkUnaryOp(Operator op, ExprCode *exp) {
  switch (op) {
    case Minus:
      return new UnaryMinusExpr(exp);
    case Star:
      return new DerefExpr(exp);
    case Ampersand:
      return new AddressOfExpr(exp);
    case Bang:
      return new LogicalNotExpr(exp);
    case PlusPlus:
      return new PreIncrExpr(exp);
    case MinusMinus:
      return new PreDecrExpr(exp);
    default:
      std::cerr << "Unsupported unary op." << std::endl;
      exit(1);
  }
}

ExprCode* codeGen::mkUnaryPostOp(Operator op, ExprCode *exp) {
  switch (op) {
    case PlusPlus:
      return new PostIncrExpr(exp);
    case MinusMinus:
      return new PostDecrExpr(exp);
    default:
      std::cerr << "Unsupported unary post op." << std::endl;
      exit(1);
  }  
}

ExprCode* codeGen::mkBinOp(Operator op, ExprCode *exp1, ExprCode *exp2) {
  switch (op) {
    case Plus:
      return new AddExpr(exp1, exp2);
    case Minus:
      return new SubtractionExpr(exp1, exp2);
    case Star:
      return new MultExpr(exp1, exp2);
    case Slash:
      return new DivisionExpr(exp1, exp2);
    case Mod:
      return new ModExpr(exp1, exp2);
    case Greater:
      return new GreaterThanExpr(exp1, exp2);
    case GreaterEq:
      return new GreaterThanEqExpr(exp1, exp2);
    case Less:
      return new LessThanExpr(exp1, exp2);
    case LessEq:
      return new LessThanEqExpr(exp1, exp2);
    case EqEq:
      return new EqualExpr(exp1, exp2);
    case NotEq:
      return new NotEqualExpr(exp1, exp2);
    case LogicalOr:
      return new LogicalOrExpr(exp1, exp2);
    case LogicalAnd:
      return new LogicalAndExpr(exp1, exp2);
    case Equal:
      return new AssignExpr(exp1, exp2);
    case PlusEqual:
      return new PlusEqualExpr(exp1, exp2);
    case MinusEqual:
      return new MinusEqualExpr(exp1, exp2);
    default:
      std::cerr << "Unsupported binary op." << std::endl;
      exit(1);
  }    
}

namespace codeGen {
template<>
ConstExpr<int>* mkConst(int val) {
  return new ConstExpr<int>(val);
}

template<>
ConstExpr<double>* mkConst(double val) {
  return new ConstExpr<double>(val);
}

template<>
ConstExpr<bool>* mkConst(bool val) {
  return new ConstExpr<bool>(val);
}
}

ListExpr* codeGen::mkConstList(int elements[], int numElts) {
  return codeGen::Util::mkList<int>(elements, numElts);
}

ListExpr* codeGen::mkConstList(double elements[], int numElts) {
  return codeGen::Util::mkList<double>(elements, numElts);
}

ListExpr* codeGen::mkConstList(bool elements[], int numElts) {
  return codeGen::Util::mkList<bool>(elements, numElts);
}

template<class T>
ListExpr* Util::mkList(T elements[], int numElts) {
  vector<ExprCode*>* elts = new vector<ExprCode*>;
  for (int i=0; i < numElts; ++i) {
    elts->push_back(new ConstExpr<T>(elements[i]));
  }
  return new ListExpr(elts);
}

