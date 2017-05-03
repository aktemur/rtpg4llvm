#include "scope.h"
#include <iostream>
#include <stdlib.h>

using namespace codeGen;

scope::scope() {
   push_scope();
}

void scope::push_scope() {
   scopes.push_back(map< const string, cval>());
}

void scope::pop_scope() {
   scopes.pop_back();
}

cval& scope::operator[] (const string &var) {
   for (typeof(scopes.rbegin())s = scopes.rbegin(); s != scopes.rend(); ++s) {
      map<const string, cval>::iterator i = s->find(var);
      if (i != s->end()) {
         return i->second;
      }
   }
   std::cerr << "Variable " << var << " not found in scope.\n";
   exit(1);
}

void scope::put(const string &var, cval &val) {
  // check if the variable already exists. 
  // If it does, raise an error because redefinitions and shadowing
  // are not allowed.
  for (typeof(scopes.rbegin())s = scopes.rbegin(); s != scopes.rend(); ++s) {
    map<const string, cval>::iterator i = s->find(var);
    if (i != s->end()) {
      std::cerr << "Variable " << var << " cannot be redefined.\n";
      exit(1);      
    }
  }

  scopes.back()[var] = val;
}
