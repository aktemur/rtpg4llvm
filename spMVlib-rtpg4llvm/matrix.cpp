#include "matrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace spMVgen;
using std::vector;
using std::map;
using std::pair;
using std::cout;

Matrix::Matrix(int *rows, int *cols, double *vals, int n, int nz):
  rows(rows), cols(cols), vals(vals), n(n), nz(nz) { 
  numRows = n;
  numCols = nz;
  numVals = nz;
}

Matrix::~Matrix() {
  delete[] rows;
  delete[] cols;
  delete[] vals;
}

void Matrix::print() {
  cout << "int matrixCols[" << numCols << "] = {\n";
  for(int i = 0; i < numCols; ++i) {
    cout << cols[i] << ", \n";
  }
  cout << "};\n";
  cout << "int matrixRows[" << numRows << "] = {\n";
  for(int i = 0; i < numRows; ++i) {
    cout << rows[i] << ", \n";
  }
  cout << "};\n";
  cout << "double matrixVals[" << numVals << "] = {\n";
  for(int i = 0; i < numVals; ++i) {
    if(vals[i] == 0) 
      cout << "0.0, \n";
    else 
      cout << vals[i] << ", \n";
  }
  cout << "};\n";      
}  

bool MMElement::compare(MMElement *elt1, MMElement *elt2) {
  if (elt1->row < elt2->row) return true;
  else if (elt2->row < elt1->row) return false;
  else return elt1->col < elt2->col;
}

void RowStencil::setEltIndices(const vector<int> *elts) {
  eltIdx = elts;
}

void RowStencil::addRow(int row) {
  rowIdx.push_back(row);
}

vector<int>& RowStencil::getRows() {
  return rowIdx;
}

const vector<int>* RowStencil::getEltIndices() {
  return eltIdx;
}


MMMatrix::MMMatrix(int n, bool isView) {
  this->n = n;
  this->isView = isView;
}

MMMatrix::~MMMatrix() {
  if(!isView) {
    // Explicitly destruct MMElements
    vector<MMElement*>::iterator it=elts.begin(),
      itEnd = elts.end();
    for(; it != itEnd; ++it) {
      delete (*it);
    }
  }
}

void MMMatrix::sort() {
  std::sort(elts.begin(), elts.end(), MMElement::compare);
}

void MMMatrix::add(int row, int col, double val) {
  if(val != 0)
    elts.push_back(new MMElement(row, col, val));
}

void MMMatrix::add(MMElement *elt) {
  if(elt->val != 0)
    elts.push_back(elt);
}

void MMMatrix::normalize() {
  sort();
}
    
void MMMatrix::print() {
  vector<MMElement*>::size_type sz = elts.size();
  cout << n << " " << n << " " << sz << "\n";
  for(unsigned int i=0; i < sz; ++i) {
    cout << elts[i]->row << " " 
         << elts[i]->col << " "
         << elts[i]->val << "\n";
  }
}

void MMMatrix::printMTX() {
  vector<MMElement*>::size_type sz = elts.size();
  cout << n << " " << n << " " << sz << "\n";
  for(unsigned int i=0; i < sz; ++i) {
    cout << elts[i]->row+1 << " " 
         << elts[i]->col+1 << " "
         << elts[i]->val << "\n";
  }
}

GroupByRowView& MMMatrix::getGroupByRows() {
  if(groupByRowView.size() > 0)
    return groupByRowView;

  // Initialize the map. This makes sure that rows with no elements
  // will have an entry in the map.
  for(int i = 0; i < n; ++i) {
    groupByRowView[i];
  }

  vector<MMElement*>::iterator it = elts.begin(), itEnd = elts.end();
  for(; it != itEnd; ++it) {
    int currentRow = (*it)->row;
    groupByRowView[currentRow].push_back(*it);
  }
  return groupByRowView;
}
    
Matrix* MMMatrix::toCSRMatrix() {
  vector<MMElement*>::size_type sz = elts.size();
  int *rows = new int[n+1];
  rows[0] = 0;
  int *cols = new int[sz]; 
  double *vals = new double[sz]; 
  GroupByRowView &view = getGroupByRows();
  GroupByRowView::iterator viewIt = view.begin(), 
    viewItEnd = view.end();
  vector<MMElement*>::iterator eltIt = elts.begin(), 
    eltItEnd = elts.end(); 
  
  for(; viewIt != viewItEnd; ++viewIt) {
    int rowIndex = viewIt->first;
    int rowLength = viewIt->second.size();
    rows[rowIndex+1] = rows[rowIndex] + rowLength;
  }
  
  int eltIndex = 0;
  for(; eltIt != eltItEnd; ++eltIt, ++eltIndex) {
    cols[eltIndex] = (*eltIt)->col;
    vals[eltIndex] = (*eltIt)->val;
  }
  
  Matrix *matrix = new Matrix(rows, cols, vals, n, sz);
  matrix->numRows = n + 1;
  return matrix;
}

RowLengthView& MMMatrix::getRowsByNZ() {
  if(rowLengthMap.size() > 0) // already computed
    return rowLengthMap;

  GroupByRowView &view = getGroupByRows();
  GroupByRowView::iterator viewIt = view.begin(), 
    viewItEnd = view.end();
  
  for(; viewIt != viewItEnd; ++viewIt) {
    int rowIndex = viewIt->first;
    int rowLength = viewIt->second.size();
    rowLengthMap[rowLength].push_back(rowIndex);
  }
  return rowLengthMap;
}

Matrix* MMMatrix::toCSRbyNZMatrix() {
  RowLengthView &rowMap = getRowsByNZ();
  GroupByRowView &byRowMap = getGroupByRows();
  vector<int> colsVector;
  vector<double> valsVector;
  
  int *rows = new int[n];
  int *cols = new int[elts.size()];
  double *vals = new double[elts.size()];
  int *rowsPtr = rows;
  int *colsPtr = cols;
  double *valsPtr = vals;
  RowLengthView::reverse_iterator it = rowMap.rbegin(), itEnd = rowMap.rend();
  vector<int>::iterator vecIt, vecItEnd;
  vector<MMElement*>::iterator eltIt, eltItEnd;
  
  for (; it != itEnd; ++it) {
    vecIt = it->second.begin();
    vecItEnd = it->second.end();
    for (; vecIt != vecItEnd; ++vecIt) {
      int rowIdx = (*vecIt);
      *rowsPtr++ = rowIdx;
      vector<MMElement*> &rowElts = byRowMap[rowIdx];
      eltIt = rowElts.begin();
      eltItEnd = rowElts.end();
      for(; eltIt != eltItEnd; ++eltIt) {
        *colsPtr++ = (*eltIt)->col;
        *valsPtr++ = (*eltIt)->val;
      }
    }
  }
  
  return new Matrix(rows, cols, vals, n, elts.size());
}

GroupByBlockView& MMMatrix::getGroupByBlocks(int b_r, int b_c) {
  if(groupByBlockView.size() > 0) // already computed
    return groupByBlockView;

  vector<MMElement*>::iterator it = elts.begin(), itEnd = elts.end();
  
  for(; it != itEnd; ++it) {
    int currentRow = (*it)->row;
    int currentCol = (*it)->col;
    pair<int,int> blockIdx(currentRow/b_r, currentCol/b_c);
    groupByBlockView[blockIdx].push_back(*it);
  }

  return groupByBlockView;
}

Matrix* MMMatrix::toOSKIMatrix(int b_r, int b_c) {
  GroupByBlockView &blocks = getGroupByBlocks(b_r, b_c);
  GroupByBlockView::iterator it = blocks.begin(), itEnd = blocks.end();
  map<int,int> blockRowLengths;
  int *cols = new int[blocks.size()];
  double *vals = new double[blocks.size() * (b_r*b_c)];
  int numCols = 0;
  int numVals = 0;
  // Build cols and vals, meanwhile compute block row lengths
  for(; it != itEnd; ++it) {
    pair<int,int> blockIdx = it->first;
    int blockRowIdx = blockIdx.first;
    int blockColIdx = blockIdx.second;
    blockRowLengths[blockRowIdx] += 1;
    cols[numCols++] = blockColIdx;
    vector<MMElement*>::iterator vecIt = it->second.begin(), 
      vecItEnd = it->second.end();
    // fill zeroes as needed
    for(int i=0; i < b_r; ++i) {
      for(int j=0; j < b_c; ++j) {
        if(vecIt != vecItEnd && 
           (*vecIt)->row % b_r == i && 
           (*vecIt)->col % b_c == j) {
          vals[numVals++] = (*vecIt)->val;
          vecIt++;
        } else {
          vals[numVals++] = 0;
        }
      }
    }
  }

  // Now build the rows array.
  int *rows = new int[n];
  for(int i = 0; i < n; ++i) {
    rows[i] = blockRowLengths[i];
  }

  // Return the OSKI matrix. Note that row, col and val array lengths
  // may be unrelated to n and nz due to zero-fills and empty blocks
  Matrix *result = new Matrix(rows, cols, vals, n, elts.size());
  result->numRows = blockRowLengths.size();
  result->numCols = numCols;
  result->numVals = numVals;
  return result;
}

// A diagonal is represented by the column index of the element in the upper
// left corner, there are (n-1) + n diagonals ranging from -n+1 to n-1.
GroupByDiagonalView& MMMatrix::getGroupByDiagonals(int lo, int hi) {
  if(groupByDiagonalView.size() > 0) //already computed
    return groupByDiagonalView;

  // Initialize the map. This makes sure that diagonals with no elements
  // will have an entry in the map.
  for(int i = -lo; i <= hi; ++i) {
    groupByDiagonalView[i];
  }

  vector<MMElement*>::iterator it = elts.begin(), itEnd = elts.end();
  for(; it != itEnd; ++it) {
    int row = (*it)->row;
    int col = (*it)->col;
    int diag = col-row;
    if(diag >= -lo && diag <= hi)
      groupByDiagonalView[diag].push_back(*it);
  }
  return groupByDiagonalView;
}

void MMMatrix::addDiagonal(int diag) {
  diagonals.insert(diag);
}

DiagonalIndexSet& MMMatrix::getDiagonals() {
  return diagonals;
}

Matrix* MMMatrix::toDiagonalMatrix() {
  int numDiags = diagonals.size();
  int *diags = new int[numDiags];
  DiagonalIndexSet::iterator diagIdxIt = diagonals.begin();
  for(int i=0; i < numDiags; ++i, ++diagIdxIt) {
    diags[i] = *diagIdxIt;
  }
  int lo = abs(diags[0]);
  int hi = diags[numDiags-1];

  // A diagonal d contains n-d elements.
  int numMissingElements = 0;
  for(int i=0; i < numDiags; ++i)
    numMissingElements += abs(diags[i]);
  int numVals = numDiags*n - numMissingElements;

  // vals will contain the flattened diagonals, with zero fills as necessary
  double *vals = new double[numVals];
  double *valsPtr = vals;

  GroupByDiagonalView &view = getGroupByDiagonals(lo, hi);
  for(int i=0; i < numDiags; ++i) {
    int diag = diags[i]; // column of the (imaginary) top-left element
    std::vector<MMElement*>::iterator it = view[diag].begin(),
      itEnd = view[diag].end();
    int startRow = diag < 0 ? -diag : 0;
    int endRow = diag < 0 ? n-1 : n-1-diag;
    // Add the elements of the diagonal to vals array
    // Fill with zeroes for elements that don't exist. 
    for(int i = startRow; i <= endRow; ++i, ++valsPtr) {
      *valsPtr = 0; // fill zero
      if(it == itEnd) continue;
      int row = (*it)->row;
      int col = (*it)->col;
      double val = (*it)->val;
      if(row == i) {
        *valsPtr = val;
        ++it;
      }
    }
  }
  Matrix *matrix = new Matrix(NULL, diags, vals, n, elts.size());
  matrix->numVals = numVals;
  matrix->numCols = numDiags;
  matrix->numRows = 0;
  return matrix;
}

StencilView& MMMatrix::getStencils() {
  if(stencilView.size() > 0) //already computed
    return stencilView;

  GroupByRowView &view = getGroupByRows();
  GroupByRowView::iterator viewIt = view.begin(), 
    viewItEnd = view.end();
  
  for(; viewIt != viewItEnd; ++viewIt) {
    int rowIndex = viewIt->first;
    // calculate stencil.
    vector<int> stencil;
    vector<MMElement*>::iterator rowElt = viewIt->second.begin(),
      rowEltEnd = viewIt->second.end();
    for(; rowElt != rowEltEnd; ++rowElt) {
      int col = (*rowElt)->col;
      stencil.push_back(col-rowIndex);
    }

    pair<vector<int>, RowStencil> keyValuePair(stencil, RowStencil());
    // insert does not add a new entry if key already exists.
    pair<StencilPool::iterator, bool> ret = stencilPool.insert(keyValuePair);
    // ret will point to the corresponding RowStencil,
    // either newly added, or existing.
    RowStencil &rs = ret.first->second;
    rs.addRow(rowIndex);
    
    if(ret.second) {// newly added entry
      rs.setEltIndices(&(ret.first->first));
      stencilView.push_back(&rs);
    }
  }
  return stencilView;
}


Matrix* MMMatrix::toStencilMatrix() {
  vector<MMElement*>::size_type sz = elts.size();
  double *vals = new double[sz]; 
  std::vector<int> *rows = new vector<int>();

  // build vals array
  GroupByRowView &rowView = getGroupByRows();
  StencilView &stencils = getStencils();
  StencilView::iterator stencilIt = stencils.begin(),
    stencilItEnd = stencils.end();
  int valIdx = 0;
  for(; stencilIt != stencilItEnd; ++stencilIt) {
    vector<int> &stencilRows = (*stencilIt)->getRows(); 
    for(int i = 0; i < stencilRows.size(); ++i) {
      int row = stencilRows[i];
      vector<MMElement*> &rowElts = rowView[row];
      for(int j = 0; j < rowElts.size(); ++j) {
        vals[valIdx] = rowElts[j]->val;
        valIdx++;
      }
    }
  }

  // build rows array which contains row indices according to the order of stencils
  stencilIt = stencils.begin();
  stencilItEnd = stencils.end();
  for(; stencilIt != stencilItEnd; ++stencilIt) {
    vector<int> &stencilRows = (*stencilIt)->getRows();
    if(stencilRows.size() < 2)
      continue;
    for(int i = 0; i < stencilRows.size(); ++i) {
      rows->push_back(stencilRows[i]);
    }
  }

  Matrix *result = new Matrix(&((*rows)[0]), NULL, vals, n, sz);
  result->numRows = rows->size();
  result->numCols = 0;
  result->numVals = sz;
  return result;
}


Matrix* MMMatrix::toUnfoldingMatrix() {
  vector<MMElement*>::size_type sz = elts.size();  
  Matrix *result = new Matrix(NULL, NULL, NULL, n, sz);  
  result->numRows = 0;
  result->numCols = 0;
  result->numVals = 0;
  return result;
}
