#include <cstdlib> // defines NULL
#include <vector>
#include <map>
#include <set>
#include <utility>

namespace spMVgen {
  class Matrix {
  public:
    int *cols;
    int *rows;
    double *vals;
    int n;
    int nz;
    // For some representations, numRows, numCols, numVals
    // may not be the same as n, nz.
    int numRows, numCols, numVals;
    
    Matrix(int *rows, int *cols, double *vals, int n, int nz);

    ~Matrix();
    
    void print();
  };

  class MMElement {
  public:
    int row; int col; double val;
    
    MMElement(int row, int col, double val):
      row(row), col(col), val(val) {}
    
    static bool compare(MMElement *elt1, MMElement *elt2);
  };


  class RowStencil {
  private:
    std::vector<int> rowIdx;
    const std::vector<int> *eltIdx;
    
  public:
    void setEltIndices(const std::vector<int> *elts);
    void addRow(int row);
    std::vector<int>& getRows();
    const std::vector<int>* getEltIndices();
  };

  typedef std::map<int, std::vector<MMElement*> > GroupByRowView;
  typedef std::map<int, std::vector<MMElement*> > GroupByDiagonalView;
  typedef std::map<int, std::vector<int> > RowLengthView;
  typedef std::map<std::pair<int,int>, std::vector<MMElement*> > GroupByBlockView;
  // RowStencils in StencilView point to stencil objects in StencilPool.
  typedef std::vector<RowStencil*> StencilView;
  typedef std::map<std::vector<int>, RowStencil > StencilPool;
  typedef std::set<int> DiagonalIndexSet;

  class MMMatrix {
  private:
    // Is this a view of another matrix?
    // Views are created after splitting a matrix.
    // Views are NOT the owners of MMElements, hence
    // they do not destruct MMElement objects.
    bool isView;

    GroupByRowView groupByRowView;
    GroupByDiagonalView groupByDiagonalView;
    RowLengthView rowLengthMap;
    GroupByBlockView groupByBlockView;
    StencilView stencilView;
    StencilPool stencilPool;
    DiagonalIndexSet diagonals;

  public:
    std::vector<MMElement*> elts;
    int n;

    MMMatrix(int n, bool isView=false);

    ~MMMatrix();

    void add(int row, int col, double val);
    void add(MMElement *elt);
    void normalize();
    void print();
    void printMTX();
    GroupByRowView& getGroupByRows();
    StencilView& getStencils();
    
    // Return a matrix in the CSR format
    Matrix* toCSRMatrix();

    // Return a map from the number of elements 
    // to the vector of rows that has that many elements.
    RowLengthView& getRowsByNZ();

    // Return a matrix to be used by CSRbyNZ
    // rows: row indices, sorted by row lengths
    // cols: indices of elements, 
    //       sorted according to the order used in rows array
    // vals: values as usual,
    //       sorted according to the order used in rows array
    Matrix* toCSRbyNZMatrix();

    // Return a map from the block coordinate
    // to the elements covered in that block
    // By definition of std::pair and std::map,
    // blocks will be sorted in row-major order.
    // The elements within blocks will be sorted in row-major
    // order as well because that's the original order in
    // a normalized MMMatrix -- the order is preserved when
    // inserting the elements.
    GroupByBlockView& getGroupByBlocks(int b_r, int b_c);

    // Return a matrix to be used by OSKI
    // rows: length of each non-zero block row
    // cols: block column indices of blocks
    // vals: values, filled with zeroes as needed,
    //       sorted according to row-major block order
    Matrix* toOSKIMatrix(int b_r, int b_c);

    // Return a matrix to be used by stencil-specialized code.
    Matrix* toStencilMatrix();

    // Return a matrix to be used by unfolding-specialized code.
    Matrix* toUnfoldingMatrix();

    // Return a map from the diagonal coordinate to the elements
    // contained in the diagonal.
    // A diagonal coordinate is the index of the column of the 
    // top-left element. Thus, it ranges from -n+1 to n-1.
    // There are (n-1) + n diagonals in total.
    GroupByDiagonalView& getGroupByDiagonals(int lo, int hi);

    // Add a diagonal index to denote that this diagonal is
    // contained in the matrix. This method is to be
    // used by the band splitters.
    void addDiagonal(int diag);

    DiagonalIndexSet& getDiagonals();
    
    // Return a matrix to be used by diagonal specializer.
    Matrix* toDiagonalMatrix();

  private:
    void sort();
  };

}


