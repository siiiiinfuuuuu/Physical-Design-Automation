#ifndef _PARSER_H_
#define _PARSER_H_
#include <vector>
#include <string>
#include <cstring>
using namespace std;


//-------------------------------------------------------------------------------------------------------//
struct net_S
{
    public:
        net_S(char* p_caNodeTarget, char* p_caNodeSource, int p_nWeight)
        {
            strcpy(nodeTarget,p_caNodeTarget);
            strcpy(nodeSource,p_caNodeSource);
            nWeight = p_nWeight;
        }
        char nodeSource[32];
        char nodeTarget[32];
        int nWeight;
};
//-------------------------------------------------------------------------------------------------------//
struct graphData_S
{
    public:
        char caSrcNode[32];
        char caTgtNode[32];
        vector<string> vNode;
        vector<net_S*> vNet;
};
//-------------------------------------------------------------------------------------------------------//
class graphParser_C
{
    public:
        graphData_S* getGraphData()
        {
            return m_pGraphData;
        };
    
        graphParser_C(const char* p_sInFileName);
        ~graphParser_C();
        bool parse();
        void dump();
    private:
        const char* m_sInFileName;
        graphData_S* m_pGraphData;
};
//-------------------------------------------------------------------------------------------------------//
#endif

