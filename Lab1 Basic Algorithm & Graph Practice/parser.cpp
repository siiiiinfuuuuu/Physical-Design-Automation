#include "parser.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <numeric>
#include <map>
using namespace std;

//-------------------------------------------------------------------------------------------------------//
graphParser_C::graphParser_C(const char* p_sInFileName) : m_pGraphData(new graphData_S)
{
    string tmpStrHolder(p_sInFileName);
    m_sInFileName=tmpStrHolder.c_str();
    parse();
}
//-------------------------------------------------------------------------------------------------------//
graphParser_C::~graphParser_C()
{
    if (m_pGraphData)
    {
        delete m_pGraphData;
        m_pGraphData=0;
    }
}
//-------------------------------------------------------------------------------------------------------//
bool graphParser_C::parse()
{
    char buffer[80];
    ifstream input;
    input.open(m_sInFileName, ios::in);
    char nodeSource[32], nodeTarget[32];
    int nWeight;
    net_S* pNet(0);
    if (!input)
    {
        printf("File %s Can Not Be Opened! Exit\n", m_sInFileName);
        return -1;
    }
    else
    {
        printf("[Parser] - File %s Open Success.\n", m_sInFileName);
        printf("[Parser] - Parsing! ... ");
    }
    input.getline(buffer,80);
    sscanf(buffer,"source: %s",m_pGraphData->caSrcNode);
    input.getline(buffer,80);
    sscanf(buffer,"target: %s",m_pGraphData->caTgtNode);
    while(input.peek()!=EOF)
    {
        input.getline(buffer, 80);
        string s(buffer);
        if (s.length() == 0)
            continue;
        sscanf(buffer, "%s %s %d", nodeTarget, nodeSource, &nWeight);
        pNet = new net_S(nodeTarget,nodeSource,nWeight);
    
        m_pGraphData->vNode.push_back(nodeTarget);
        m_pGraphData->vNode.push_back(nodeSource);
        m_pGraphData->vNet.push_back(pNet);
    }
    sort(m_pGraphData->vNode.begin(),m_pGraphData->vNode.end());
    m_pGraphData->vNode.erase(unique(m_pGraphData->vNode.begin(),m_pGraphData->vNode.end()),
                              m_pGraphData->vNode.end());
    printf("Parsing Complete!\n");
    input.close();
    return true;
}
//-------------------------------------------------------------------------------------------------------//
void graphParser_C::dump()
{
    ofstream output;
    output.open("dumpParser.txt");
    for(unsigned int nNetIter=0;nNetIter<m_pGraphData->vNet.size();++nNetIter)
    {
        output<<m_pGraphData->vNet[nNetIter]->nodeTarget<<"\t"<<m_pGraphData->vNet[nNetIter]->nodeSource
              <<"\t"<<m_pGraphData->vNet[nNetIter]->nWeight<<endl;
    }
    output<<endl;
    for(unsigned int nNodeIter=0;nNodeIter<m_pGraphData->vNode.size();++nNodeIter)
    {
        output<<m_pGraphData->vNode[nNodeIter]<<endl;
    }
    output.close();
}
//-------------------------------------------------------------------------------------------------------//
