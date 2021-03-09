#include <iostream>

class dmMgr_C;

//-----------------------------------------------------------------------------------------------------------//
class shortestPath_C
{
    public:
        shortestPath_C(dmMgr_C* p_pDmMgr) {m_pDmMgr = p_pDmMgr;}
        bool BellmanFord();
        void dump() const;

    private:
        void initializeSingleSource(node_C*);
        void relax(node_C*,node_C*,int);
        dmMgr_C* m_pDmMgr;
};
//-----------------------------------------------------------------------------------------------------------//
class maxflowMinCut_C
{
    public:
        maxflowMinCut_C(dmMgr_C* p_pDmMgr) {m_pDmMgr = p_pDmMgr;maxflow=0;}
        bool FordFulkerson();
        void dump() const;
        int maxflow;

    private:
        bool bfs(node_C*, node_C*,vector<node_C*>&);
        int minCapacity(vector<node_C *> &);
        dmMgr_C *m_pDmMgr;
};
//-----------------------------------------------------------------------------------------------------------//
