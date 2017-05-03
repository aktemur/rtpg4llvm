#include "stmtCode.h"

using namespace codeGen;

StmtCode::~StmtCode() {}

void StmtCode::indent(int level) {
  for(int i=0; i < level; ++i) {
    std::cout << " ";
  }
}

void StmtCode::print() {
  print(0);
}