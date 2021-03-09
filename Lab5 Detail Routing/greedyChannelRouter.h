#include <vector>
#include <set>
#include <map>

enum NET_STATE{rising, falling, steady, complete};

class Job{
private:
public:
    Job(int n, int s, int f) : netNum(n), start(s), finish(f){}
    int netNum;
    int start;
    int finish;
    bool operator<(const Job& j) const{
        return finish < j.finish;
    }
};

class Net{
private:
public:
    std::set<int> topCol, bottomCol;
};

class Column{
private:
public:

    Column() : topFail(false), bottomFail(false){}
    bool noFreeTrack();

    std::vector<int> curr;
    std::vector<int> next;
    std::map<int, std::set<int>> PendingNet;
    bool topFail;
    bool bottomFail;
};

class Line{
public:
    Line(char id, int ia, int ib, int ic):d(id),a(ia),b(ib),c(ic){}
    char d;
    int a, b, c;
};

class GCR{
private:
    void Step1();
    void Step2();
    void Step3();
    void Step4();
    void Step5();
    void Step6();
    NET_STATE NetState(int n);
    bool NotComplete(int currCol);
    void extendCol();

    //Step1
    int findTopTrack(int top, std::vector<int> currH);
    int findBottomTrack(int bottom, std::vector<int> currH);
    void connectTop(int currCol, int topTrack);
    void connectBottom(int currCol, int bottomTrack);

    //Step2
    bool isEmptyV(Job j);
    std::set<int> mergeNet;

    //Step3
    bool movable(int ta, int tb);
    void move(int ta, int tb);

    int currCol;
    int initialWidth;
    int currWidth;
    int MJL;
    int SNC;

public:
    GCR(int iW, int m, int s);
    void parser(char* fileName);
    void reverse_input();
    std::pair<int,int> greedyChannelRouter();
    void PrintCurrentColumn();
    void PrintNextColumn();
    void ANS();
    void Output();
    void OutputGDT();

    int netNum;
    int colNum;
    std::vector<int> top, bottom;
    std::map<int, Net> nets;
    std::vector<Column> colH;
    std::vector<std::vector<int>> colV;
    std::map<int, std::vector<Line>> ans;
};

class Result{
public:
    Result(){}
    Result(int i, int m,int s):inital(i),MJL(m),SNC(s){}
    int inital;
    int MJL;
    int SNC;
};

