#include "spMVgen.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace spMVgen;

void TakeAllSplitter::split() {
  mmMatrixOne = mmMatrix;
  mmMatrixTwo = new MMMatrix(mmMatrix->n, true);
}

void ByCountSplitter::split() {
  mmMatrixOne = new MMMatrix(mmMatrix->n, true);
  mmMatrixTwo = new MMMatrix(mmMatrix->n, true);

  std::vector<MMElement*>::iterator it = mmMatrix->elts.begin(),
    itEnd = mmMatrix->elts.end();
  int numElts = 1;
  for(; it != itEnd; ++it, ++numElts) {
    if(numElts <= count)
      mmMatrixOne->add(*it);
    else
      mmMatrixTwo->add(*it);
  }
}


void ByBandSplitter::split() {
  mmMatrixOne = new MMMatrix(mmMatrix->n, true);
  mmMatrixTwo = new MMMatrix(mmMatrix->n, true);

  for(int i = -lo; i <= hi; ++i)
    mmMatrixOne->addDiagonal(i);

  std::vector<MMElement*>::iterator it = mmMatrix->elts.begin(),
    itEnd = mmMatrix->elts.end();
  int numElts = 1;
  for(; it != itEnd; ++it, ++numElts) {
    int row = (*it)->row;
    int col = (*it)->col;
    int diag = col - row;
    if(-lo <= diag && diag <= hi) {
      mmMatrixOne->add(*it);
    } else {
      mmMatrixTwo->add(*it);
    }
  }
}

void ByDenseBandSplitter::split() {
  int n = mmMatrix->n;
  mmMatrixOne = new MMMatrix(n, true);
  mmMatrixTwo = new MMMatrix(n, true);

  GroupByDiagonalView &view = mmMatrix->getGroupByDiagonals(n-1, n-1); // all diagonals
  GroupByDiagonalView::iterator viewIt = view.begin(), 
    viewItEnd = view.end();
  
  for(; viewIt != viewItEnd; ++viewIt) {
    int diag = viewIt->first;
    int diagNZ = viewIt->second.size();
    double diagDensity = diagNZ * 100.0 / (n - abs(diag));
    // Add all elements of the diagonal to either matrixOne or matrixTwo
    if(diagDensity >= percent && diag >= -lo && diag <= hi) {
      std::vector<MMElement*>::iterator it = viewIt->second.begin(),
        itEnd = viewIt->second.end();
      for(; it != itEnd; ++it) {
        mmMatrixOne->add(*it);
      }
      mmMatrixOne->addDiagonal(diag);
    } else {
      std::vector<MMElement*>::iterator it = viewIt->second.begin(),
        itEnd = viewIt->second.end();
      for(; it != itEnd; ++it) {
        mmMatrixTwo->add(*it);
      }
    }
  }
  mmMatrixOne->normalize();
  mmMatrixTwo->normalize();
}


void ByBlockSplitter::split() {
  int n = mmMatrix->n;
  mmMatrixOne = new MMMatrix(n, true);
  mmMatrixTwo = new MMMatrix(n, true);

  int bottomRow = n / b_r * b_r;
  int rightmostCol = n / b_c * b_c;

  std::vector<MMElement*>::iterator it = mmMatrix->elts.begin(),
    itEnd = mmMatrix->elts.end();
  int numElts = 1;
  for(; it != itEnd; ++it, ++numElts) {
    int row = (*it)->row;
    int col = (*it)->col;
    if(row < bottomRow && col < rightmostCol)
      mmMatrixOne->add(*it);
    else
      mmMatrixTwo->add(*it);
  }
}


