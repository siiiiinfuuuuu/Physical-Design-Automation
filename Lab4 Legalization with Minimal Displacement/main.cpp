#include "placement.h"
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char** argv){
    time_t start(time(NULL));
    Placement placement(argv[1]);
    placement.CutRow();
    //placement.TestCutRow();
    placement.Abacus();
    // placement.TestAbacus();
    placement.OutputFile();
    time_t end(time(NULL));
    cout << end - start << endl;
    return 0;
}
