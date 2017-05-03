#ifndef _FUNC_PARAM_
#define _FUNC_PARAM_

#include <string>
#include "types.h"

namespace codeGen {

class FuncParam {
private:
  Type* type;
  std::string name;
  bool restricted;
public:
  FuncParam(Type* type, std::string name, bool isRestrict = false): 
     type(type), name(name), restricted(isRestrict) {}

  ~FuncParam();
  
  std::string getName();
  
  Type* getType();  
  
  bool isRestrict();
  
  virtual void print();
};

}

#endif
