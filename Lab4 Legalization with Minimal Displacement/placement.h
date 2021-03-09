#include "basicDS.h"
#define TESTNUM 1000

class Placement{
private:
    int PlaceRow(int rIndex);
    void NodeParser(const char* nodeFileName);
    void PlParser(const char* plFileName);
    void SclParser(const char* sclFileName);
    int RowIndex(int y);
    void RowFlagInit();
    void SubRowFlagInit();

    Cell* currCell;
    int bestCost;

    SubRow* NextRow();
    int currRowIndex;
    int upRow;
    int downRow;
    bool nextRow;
    bool up;
    bool upStop;
    bool downStop;

    SubRow* NextSubRow(Row* currRow);
    int currSubRowIndex;
    int leftRow;
    int rightRow;
    bool right;
    bool rightStop;
    bool leftStop;


public:
    Placement(char* auxFileName);
    void CutRow();
    void TestCutRow();
    void Abacus();
    void TestAbacus();
    void OutputFile();

    int numCells, numTerminals;
    std::vector<Cell> cells;
    int numRows;
    int height;
    std::vector<Row *> rows;
};