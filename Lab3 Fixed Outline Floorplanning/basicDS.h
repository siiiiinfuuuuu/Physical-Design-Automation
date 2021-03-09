#include <vector>
#include <string>
#include "RBT.h"

struct Point{
    int x, y;
    Point() : x(0), y(0){}
    Point(int i, int j) : x(i), y(j){}
    void operator=(const Point& p){
        x = p.x;
        y = p.y;
    }
    bool operator==(const Point& p) const{
        return p.x == x && p.y == y;
    }
    Point operator-(const Point &p) const{
        Point temp;
        temp.x = x - p.x;
        temp.y = y - p.y;
        return temp;
    }

};

class Terminal{
private:
    
public:
    std::string name;
    Point position;
};

class Block{
private:

public:
    std::string name;
    Point size;
};

class Net{
private:

public:
    int degree;
    std::vector<Block*> netBlocks;
    std::vector<Terminal*> netTerminals;
};

class SequencePair{
private:
    void DiscardTest(RBT &BST, TreeNode* curr);
public:
    SequencePair(int s);
    void operator=(const SequencePair &sp);
    void CalPosition(std::vector<Block*> blocks);
    int Area(std::vector<Block*> blocks);
    void Print();
    int size;
    Point outline;
    std::vector<int> posGamma;
    std::vector<int> negGamma;
    std::vector<int> posMatch;
    std::vector<int> negMatch;
    
    std::vector<int> posRGamma;
    std::vector<int> posRMatch;
    std::vector<Point> blockPosition;
};