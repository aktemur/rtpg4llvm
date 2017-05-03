#include "returnStmt.h"

using namespace codeGen;

ReturnStmt::~ReturnStmt() {
  delete exp;
}

void ReturnStmt::build(){
   if (exp == NULL) {
      Builder->CreateRetVoid();
   }else  {
      cval retVal = exp->build();
      Builder->CreateRet(retVal.val);
   }
}

void ReturnStmt::print(int level) {
  indent(level);
  std::cout << "return";
  if(exp != NULL) {
    std::cout << " ";
    exp->print();
  }
  std::cout << ";\n";
}
  
