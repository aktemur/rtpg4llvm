# rtpg4llvm

**rtpg4llvm** is a compiler for runtime program generation using the quotation/antiquotation syntax where the host language is C++ and the target language is (a subset of) C.

Code generated at runtime is [LLVM](http://llvm.org/) IR code.
It can be executed immediately after runtime compilation.

The classical "power" example, which specializes the exponentiation function on the value of the exponent, is implemented as follows.

```cpp
#include "codeGenConstructors.h"
#include <iostream>

using namespace codeGen;

int main(int argc, const char *argv[]) {
  int power = 5; // Specialize for the fifth power

  ExprCode *c = $expr< 1 >$;
  for(int i=0; i < power; i++) {
    c = $expr< `expr(c`) * x >$;
  }
  // c is now the code "1*x*x*x*x*x"

  CompileUnit *powFunCode = $compunit<
    int pow(int x) { // capture the free variable x in c
      return `expr(c`);
    }
  >$;

  // Compile the function and grab a pointer
  powFunCode->compile();
  delete powFunCode; // Safe to delete after compilation
  typedef int(*powFunType)(int);
  powFunType powFunPtr =
    (powFunType) codeGen::getFunctionPointer("pow");

  std::cout << "2^5=" << powFunPtr(2) << "\n";
  std::cout << "3^5=" << powFunPtr(3) << "\n";
  std::cout << "4^5=" << powFunPtr(4) << "\n";
  
  return 0;
}
```

## People
The following people have been involved in this project: Baris Aktemur, Sam Kamin, Umit Akgun, Maria Garzaran, Asim Yildiz.

## Acknowledgment
This project received funding from [Tubitak](http://tubitak.gov.tr/) and [NSF](nsf.gov).

## Download and Installation
**Environment:** The compiler has been tested with LLVM/Clang version 3.2 (subversion revision 161611) on Linux (see Mac OS note below). Because the LLVM base is frequently refactored, it is possible that the code will not compile with the up-to-date version of LLVM. JRE 1.6 or above and Python (tested ver. 2.7.3) are required to run the preprocessor and the parser.

**Installing:** Download the [rtpg4llvm.tar.gz](dist/rtpg4llvm.tar.gz) file,
extract it to a folder, then make.

```
    $ tar -xzf rtpg4llvm.tar.gz
    $ cd rtpg4llvm
    $ make
```

**Using:** To test your installation, download the [power.cpp](dist/power.cpp)
example given on the front page. Run the following commands:

```
    $ export CLASSPATH=$CLASSPATH:PATH_TO_rtpg4llvm_FOLDER/parser.jar
    $ python PATH_TO_rtpg4llvm_FOLDER/preprocess.py power
```
    
This will create a `power-pp.cpp` file.
Now compile this file by linking to the rtpg4llvm library.

```
    $ clang++ -c -IPATH_TO_rtpg4llvm_FOLDER \
        `llvm-config --cxxflags` power-pp.cpp -o power-pp.o
    $ clang++ PATH_TO_rtpg4llvm_FOLDER/build/*.o power-pp.o \
	`llvm-config --ldflags` `llvm-config --libs all` -o power
    $ ./power
    2^5=32
    3^5=243
    4^5=1024
```

See the [spMVlib](spMVlib-rtpg4llvm) library for a much bigger use case.


### Mac OS
As of time of writing this document, MCJIT of LLVM does not work on Mac OS.
To install, follow the steps above, but before running `make`,
comment out the following lines in the `codeGeneration.cpp` file:

```
    engineBuilder.setUseMCJIT(true);
    LLIMCJITMemoryManager *JMM = new LLIMCJITMemoryManager();
    engineBuilder.setJITMemoryManager(JMM);
```

Note that disabling MCJIT will likely result in performance loss in the runtime-compiled program.

## API
This section explains the programming interface.

### Quotations
Using quotations, one can define expression, statement, and compilation unit objects. A compilation unit contains function and/or global variable declarations.
A quoted expression is an object of type `ExprCode`; a quoted statement is an object of type `StmtCode`; a quoted compilation unit is an object of type `CompileUnit`. E.g:

```cpp
ExprCode *anExpr = $expr< 42 + x >$;
StmtCode *aStmt = $stmt< y = foo(x, 42); >$;
CompileUnit *aFunction = $compunit< int bar() { return 42; } >$;
CompileUnit *aGlobalVar = $compunit< int counter = 0; >$;
```

### Anti-quotations
Anti-quotations are used for splicing a code object into another one. For the sake of parsing, a syntactic category must be provided when antiquoting. The recognized categories are `int`, `double`, `expr`, `stmt`, `list`, `unit` and `name`. `int`, `double`, and `list` are used for lifting constant values. `list` expects an `int` or a `double` array, together with its length as the second argument. It is useful when declaring an initialized array. `unit` can be used to splice compilation units into a program. `name` is useful when giving a variable or a function a generated name. Examples:

```cpp
//*** `int 
int n = 42;
ExprCode *e = $expr< n + 5 + `int(n`) >$ // $expr< n + 5 + 42 >$ 
 
//*** `double 
double d = 3.14;
$stmt< double pi = `double(d`); >$ // double pi = 3.14;
 
//*** `expr 
StmtCode *s1 = $stmt< int y = `expr(e`) * 2; >$ 
// int y = (n+5+42)*2;
 
//*** `list 
int array[5];
for(int i=0; i < 5; i++) array[i] = i;
CompileUnit *glbl = $compunit< 
   int cells[`int(5`)] = `list(array, 5`); >$ 
// int cells[5] = {0, 1, 2, 3, 4};
 
//*** `stmt 
StmtCode *s2 = $stmt< int n = x*10; `stmt(s1`) >$
// int n = x*10; int y = (n+5+42)*2;
 
//*** `unit
CompileUnit *fun = $compunit<
   `unit(glbl`)     
   void changeCell(int i) {
     cells[i] = 42;
   }
>$;
 
//*** `name 
string f = "factorial";
CompileUnit *recFun = $compunit<
   int `name(f`) (int n) {
      if(n==0) return 1;
      else return n * `name(f`)(n-1);
   }
>$;
// int factorial(int n) {
//    if(n==0) return 1;
//    else return n * factorial(n-1);
// }
```

### Grammar
The target language (i.e. the language inside the quotations) is a subset of C. Its grammar is (semi-formally) given below.

```
    Program        ::= CompileUnit*
    CompileUnit    ::= GlobalDecl | FunctionDecl
    GlobalDecl     ::= 'extern' Type Identifier ArrayType? ';'
                     | Type Identifier ArrayType? ('=' ConstInit)? ';'
    Type           ::= 'int' | 'double' | 'void' | Type '*'
    ArrayType      ::= '[' intLiteral ']' | '[' ']'
    ConstInit      ::= intLiteral | doubleLiteral | ConstArrayInit
    ConstArrayInit ::= '{' intOrDoubleLiteral* '}'
    FunctionDecl   ::= Type Identifier '(' Parameter* ')' '{' Stmt* '}'
    Parameter      ::= Type '__restrict__'? Identifier ArrayType?
    Stmt           ::= VarDecl | IfStmt | ForLoop | WhileLoop
                     | return Exp ';' | return ';' 
                     | Exp ';'
                     | '{' Stmt* '}'
    VarDecl        ::= Type Identifier ArrayType? ('=' Exp)? ';'
    Exp            ::= Identifier '(' Exp* ')' // function call
                     | Exp BinaryOp Exp 
                     | UnaryPreOp Exp | Exp UnaryPostOp
                     | Exp '[' Exp ']'         // array access
                     | Identifier | intLiteral | doubleLiteral
    BinaryOp       ::= '+' | '-' | '/' | '*' | '%'
                     | '>' | '<' | '<=' | '>=' 
                     | '!=' | '==' | '&&' | '||' 
                     | '=' | '+=' | '-='       
    UnaryPreOp     ::= '-' | '!' | '*' | '&' | '++' | '--'  
    UnaryPostOp    ::= '++' | '--'
```

### Include
To be able to use quoted code fragments, you must put the following to the header of your C++ files:

```cpp
#include "codeGenConstructors.h"
using namespace codeGen;
```

### Runtime Compilation
`CompileUnit` objects have a set of compile methods that can be invoked to compile the code at runtime.

```cpp
compile() // simply compiles the program with no optimization
 
compileAndDump() // compiles the program with no optimization
                 // and then dumps the llvm code
 
compile(int optLevel) // compiles the program, then optimizes it
                      // at level optLevel
 
compileAndDump(int optLevel) // compiles the program, 
                             // optimizes it at level optLevel, 
                             // and finally dumps the llvm code
```

After compiling the program, grab a pointer to a function in the code using the `codeGen::getFunctionPointer(...)` method and execute it. See the "power" example above for an example.

### Pretty Printing
An expression, statement, or a compile unit object can be printed to standard output using its `print()` method. This will print the program fragment in a C-like format. This feature may be found useful for debugging purposes. The output will typically contain many redundant braces, parentheses and newlines. Some syntactic transformation will have been applied; for instance, for-loops will have been converted to while-loops, combined assignments (e.g. `a += b;`) will have been expanded (e.g. `a = b + a;`), etc.

**Warning:** The array type is printed as `int[5] arr` instead of `int arr[5]`. This should be manually corrected if one wants to compile the output of the pretty print using a static compiler such as clang.

### Memory Management
A quotation dynamically creates a heap object (via `new`). To avoid memory leak, unused code objects must be deleted. After compiling a program, it is safe to `delete` it. For deallocation, simply delete the root object; it will recursively destruct all the code objects underneath. **IMPORTANT:** No code object should be reused in multiple places (i.e. anti-quoted in various places) to avoid double free'ing of the memory. Essentially, the generated code must be built to form a tree, never a DAG.

### Capabilities/Limitations
The following is a list of capabilities and limitations of the language that can be used inside quotations.
* Only one-dimensional array types are supported.
* Variables cannot be redefined or shadowed.
* Extern's and global decls are supported.
* No semantic analysis is performed; make sure that the code being generated is type-correct.
* Elements of an array initialization list must be literals. This is handled at the parsing phase.
* Global variable initalizations can be made using literal expressions only. This is handled at the parsing phase.
* Dimension of an array must be specified even when initialized using the list notation. E.g.
    ```cpp
    int rows[3] = {1,2,3}; // OK
    int rows[] = {1,2,3}; // OK in C, but rejected by our parser.
    ```
  This rule does not apply to externs, of course.
* The __restrict__ keyword can be used in formal parameter declaration of functions. This keyword can also be used in local var declarations, but it's ignored because Clang ignores it, too.
* To enable more optimizations, declare all local variables of a function manually at the beginning of the function.
