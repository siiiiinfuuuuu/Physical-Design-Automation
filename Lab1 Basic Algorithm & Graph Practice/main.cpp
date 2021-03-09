#include "parser.h"
#include "dm.h"
#include "algo.h"
#include <time.h>
using namespace std;

//-------------------------------------------------------------------------------------------------------//
int main(int argc, char** argv)
{
    /*Preliminary Information*/
    cout << "     ##############################################################" << endl;
    cout << "     #                         [PDA 2020] Lab1                    #" << endl; 
    cout << "     #------------------------------------------------------------#" << endl;
    cout << "     #    1. Single Source Shortest Path                          #" << endl; 
    cout << "     #    2. Max-Flow Min Cut Problem                             #" << endl; 
    cout << "     #------------------------------------------------------------#" << endl;
    cout << "     # Compiled in " << sizeof(void*)*8 << " bits on " << COMPILETIME 
         << "        #"<<endl;
    #ifdef __GNUC__
    cout << "     # with GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__
         << "                                             #"<<endl;
    #endif
    cout << "     ###############################################################" << endl;
    cout << endl;
    
    /*Begin Code Execution*/
    clock_t start,end;
    start = clock();
    graphParser_C *pParser = new graphParser_C(argv[1]);
    pParser->dump();
    dmMgr_C* pDMmgr = new dmMgr_C(pParser->getGraphData());
    pDMmgr->dump();
    
    //Shortest Path
    shortestPath_C* pShortestPath = new shortestPath_C(pDMmgr);
    pShortestPath->BellmanFord();
    pShortestPath->dump();
    //Max Flow Min Cut
    maxflowMinCut_C* pMaxflowMincut = new maxflowMinCut_C(pDMmgr);
    pMaxflowMincut->FordFulkerson();
    pMaxflowMincut->dump();
    end = clock();
}
