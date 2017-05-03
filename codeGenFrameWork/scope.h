#ifndef _SCOPE_
#define _SCOPE_

#include <string>
#include <vector>
#include <map>
#include "cval.h"

using namespace std;

/**
 * Code scopes
 * vector<map<string cval>>
 *
 *  scope vector
 *              |
 *    scope1    -------> hash >>->  i --- > cval
 *      |                                  |->  j ----> cval2
 *              |
 *    scope2    -------> hash >>->  i ----> cval3
 */
namespace codeGen {

class scope {
   vector< map<const string, cval> > scopes;
public:
   scope();
   void push_scope();
   void pop_scope();
   cval& operator[] (const string &var);
   void put(const string &var, cval &val);
};

}

#endif