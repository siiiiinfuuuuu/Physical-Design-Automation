#include "basicDS.h"
#include <map>


class Floorplan{
private:

public:
    void BlockPaser(char* blockName);
    void NetPaser(char* netName);
    void TestPaser();
    SequencePair SA();
    SequencePair FX(SequencePair sq);
    SequencePair IN(SequencePair sq);
    SequencePair RT(SequencePair sq);
    SequencePair Neighbor(SequencePair sq);
    void OutputFile(SequencePair sq, char* outputName);
    void OutputTest(char *outputName);

    Point outline;
    int numBlocks;
    int numTerminals;
    int numNets;
    std::vector<Block*> blocks;
    std::map<std::string, int> btMap;
    std::vector<Terminal*> terminals;
    std::vector<Net*> nets;

};