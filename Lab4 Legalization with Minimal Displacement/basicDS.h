#include <vector>
#include <map>
#include <string>

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

class Cell{
private:
public:
    bool operator<(const Cell& c)const{
        return gPos.x < c.gPos.x;
    }
    int Cost();
    std::string name;
    Point gPos;
    Point lPos;
    Point size;

};

class Cluster{
private:
public:
    Cluster() : eC(0),wC(0),qC(0){}
    std::vector<Cell *> cells;
    int xC;
    int eC;
    int wC;
    int qC;
};

class SubRow{
private:
public:
    SubRow(int min, int max, int yPos) : xMin(min),xMax(max),y(yPos){}
    void PlaceRow();
    void AddCell(Cluster* c, Cell* cell);
    void AddCluster(int cIndex, int ccIndex);
    void Collapse(int cIndex);
    int xMin;
    int xMax;
    int y;
    bool placeFail;
    std::vector<Cell *> cells;
    std::vector<Cluster> clusters;

    
};

class Row{
private:
public:
    bool operator<(const Row& r)const{
        return y < r.y;
    }
    int x;
    int y;
    int numSites;
    std::map<int, int> cutPoint;
    std::vector<SubRow*> subRows;
};

