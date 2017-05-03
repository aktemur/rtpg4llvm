#include "../codeGenConstructors.h"
#include <stdio.h>

using namespace codeGen;

void mult(int n, int *rows, int *cols, double *vals, double *v, double *w) { 
  for(int i = 0; i < n; i++) {
    double y = w[i];
    for(int k = rows[i]; k < rows[i+1]; k++)
      y += vals[k] * v[cols[k]]; 
    w[i] = y;
  }
}

int main(int argc, char* argv[]) {
  CompileUnit *p = $compunit< 
    void genedMult(int n, int *rows, int *cols, double *vals, double *v, double *w) { 
    for(int i = 0; i < n; i++) {
      double y = w[i];
      for(int k = rows[i]; k < rows[i+1]; k++)
	y += vals[k] * v[cols[k]]; 
      w[i] = y;
    }
  }
  >$;
  p->compile();

  void (*fPtr)(int,int*,int*,double*,double*,double*);
  fPtr = (void (*)(int,int*,int*,double*,double*,double*))codeGen::getFunctionPointer("genedMult");

  int rows[] = {0,2,4,7,9,11};
  int cols[] = {1,2,2,3,0,3,4,0,2,1,3};
  double vals[] = {0.01, 0.6, 0.2, 0.7, 0.4, 0.02, 0.3, 0.1, 0.8, 0.5, 0.9};
  double v[] = {6.45, 3.12, 0.78, 1, 6.03};
  double w1[] = {0,0,0,0,0};
  double w2[] = {0,0,0,0,0};

  mult(5, rows, cols, vals, v, w1);
  fPtr(5, rows, cols, vals, v, w2);

  if(w1[0] != w2[0])
    printf("!!! FAIL !!! w[0]\n");  
  if(w1[1] != w2[1])
    printf("!!! FAIL !!! w[1]\n");  
  if(w1[2] != w2[2])
    printf("!!! FAIL !!! w[2]\n");  
  if(w1[3] != w2[3])
    printf("!!! FAIL !!! w[3]\n");  
  if(w1[4] != w2[4])
    printf("!!! FAIL !!! w[4]\n");  
}
