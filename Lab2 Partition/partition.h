#include <vector>
#define ratioMax 0.55
#define ratioMin 0.45


class Node{
public:
    Node() { group = 1; }
    bool group;
};

class Net{
private:
public:
    bool operator<(const Net& n)const{
        return nodes.size() < n.nodes.size();
    }
    void Print();
    std::vector<int> nodes;
};

class DisjointSet{
private:
public:
    DisjointSet(int n);
    int Find(int x);
    void Union(int x, int y);
    int Size(int x);
    void SaveState();
    void RecoverState();
    void ClearState();
    
    int* p;
    int* s;
    int* tmpP;
    int* tmpS;
    int nodeNum;
};

class Partition{
private:
    bool RatioCheck();
    DisjointSet MinQuitUnion();
    DisjointSet MinQuitUnion_2();
    DisjointSet MinQuitUnion_3();
    void Dset2Vector(DisjointSet dSet);
    void BestPartition(std::vector<std::pair<int, std::vector<int>>>&,int);

public:
    Partition();
    void Parsing(const char* fileName);
    void ParserTest();
    void Initialize();
    void FM();
    int CutSize(std::vector<Node>&);
    void OutputFile();
    void OutputFileTest(const char* fileName);

    int netNum;
    int nodeNum;
    int leftNum;
    std::vector<Node> nodes;
    std::vector<Node> p;
    std::vector<Node> bestPartition;
    std::vector<Net> nets;
    std::vector<std::pair<int, std::vector<int>>> cVector;
};