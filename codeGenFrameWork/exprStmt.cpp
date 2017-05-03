#include "exprStmt.h"

using namespace codeGen;

ExprStmt::~ExprStmt() {
  delete e;
}

void ExprStmt::build() {
  e->build();
}

void ExprStmt::print(int level) {
  indent(level);
  e->print();
  std::cout << ";\n";
}

