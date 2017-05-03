#include "seqStmt.h"

using namespace codeGen;

SeqStmt::~SeqStmt() {
  for (typeof(stmts->begin())s = stmts->begin(); s != stmts->end(); ++s) {
    delete (*s);
  }  
  delete stmts;
}

void SeqStmt::build() {
   for (typeof(stmts->begin())s = stmts->begin(); s != stmts->end(); ++s) {
      (*s)->build();           
   }
}

void ScopedStmt::build() {
   env->push_scope();
   SeqStmt::build();
   env->pop_scope();
}

void SeqStmt::print(int level) {
  for (typeof(stmts->begin())s = stmts->begin(); s != stmts->end(); ++s) {
    (*s)->print(level);
  }  
}

void ScopedStmt::print(int level) {
  indent(level);
  std::cout << "{\n";
  SeqStmt::print(level+2);
  indent(level);
  std::cout << "}\n";
}
