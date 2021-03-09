#include "partition.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv){
    Partition* partition = new Partition;
    partition->Parsing(argv[1]);
    //partition->ParserTest();
    partition->Initialize();
    partition->FM();
    // for (int i = 0; i < partition->nodes.size(); i++)
    //     cout << partition->nodes[i].group << " ";
    
    cout << "Cut Size = " << partition->CutSize(partition->nodes) << endl;
    partition->OutputFile();
    //partition->OutputFileTest("output3_0.txt");
    return 0;
}