#include "dm.h"
#include "algo.h"
#include <fstream>
#include <queue>
#include <algorithm>

//-----------------------------------------------------------------------------------------------------------//
// shortestPath_C
//-----------------------------------------------------------------------------------------------------------//
void shortestPath_C::initializeSingleSource(node_C* p_pSource)
{
    p_pSource->setCost(0);
}
//-----------------------------------------------------------------------------------------------------------//
void shortestPath_C::relax(node_C* p_pU,node_C* p_pV, int p_nW)
{
    if(p_pU->getCost() == INT_MAX)
        return;
    if (p_pV->getCost() > p_pU->getCost() + p_nW)
    {
        p_pV->setCost(p_pU->getCost() + p_nW);
         p_pV->setParent(p_pU);
    }
}
//-----------------------------------------------------------------------------------------------------------//
bool shortestPath_C::BellmanFord()
{
    vector<node_C*> vNode;
    vector<edge_C*> vEdge;
    m_pDmMgr->getEdge(vEdge);
    m_pDmMgr->getNode(vNode);
    initializeSingleSource(m_pDmMgr->getSrcNode());
    for (unsigned int nNodeIter = 0; nNodeIter < vNode.size() - 1; ++nNodeIter)
        for(unsigned int nEdgeIter = 0; nEdgeIter < vEdge.size(); ++nEdgeIter)
            relax(vEdge[nEdgeIter]->getSrcNode(), vEdge[nEdgeIter]->getTgtNode(), vEdge[nEdgeIter]->getWeight());
}
//-----------------------------------------------------------------------------------------------------------//
void shortestPath_C::dump() const
{
    ofstream output;
    output.open("short.txt");
    vector<node_C*> vNode;
    m_pDmMgr->getNode(vNode);
    for (unsigned int nNodeIter = 0; nNodeIter < vNode.size(); ++nNodeIter)
        output << vNode[nNodeIter]->getName() << ":" << vNode[nNodeIter]->getCost() << endl;
    output.close();
}
//-----------------------------------------------------------------------------------------------------------//
// maxflowMinCut_C
//-----------------------------------------------------------------------------------------------------------//
bool maxflowMinCut_C::bfs(node_C* p_pSrcNode, node_C* p_pTgtNode, vector<node_C*>& p_rvPath)
{
    p_rvPath.resize(0);
    vector<node_C *> vNode;
    m_pDmMgr->getNode(vNode);
    for (unsigned int nNodeIter = 0; nNodeIter < vNode.size(); ++nNodeIter){
        vNode[nNodeIter]->setUnvisit();
        vNode[nNodeIter]->setParent(0);
    }
    queue<node_C *> queue;
    queue.push(p_pSrcNode);
    p_pSrcNode->setVisit();
    while(!queue.empty()){
        node_C *u = queue.front();
        queue.pop();
        vector<edge_C *> vAllEdge;
        u->getAllEdge(vAllEdge);
        for (unsigned int nEdgeIter = 0; nEdgeIter < vAllEdge.size(); ++nEdgeIter){
            //*************
            edge_C *edgeUV = vAllEdge[nEdgeIter];
            //int capacity = edgeUV->getCapacity(u);
            //*************
            if(vAllEdge[nEdgeIter]->getSrcNode()!=u 
            && !vAllEdge[nEdgeIter]->getSrcNode()->isVisit()
            && vAllEdge[nEdgeIter]->getCapacity(u) > 0){
                queue.push(vAllEdge[nEdgeIter]->getSrcNode());
                vAllEdge[nEdgeIter]->getSrcNode()->setVisit();
                vAllEdge[nEdgeIter]->getSrcNode()->setParent(u);
            }
            else if(vAllEdge[nEdgeIter]->getTgtNode()!=u 
            && !vAllEdge[nEdgeIter]->getTgtNode()->isVisit()
            && vAllEdge[nEdgeIter]->getCapacity(u) > 0){
                queue.push(vAllEdge[nEdgeIter]->getTgtNode());
                vAllEdge[nEdgeIter]->getTgtNode()->setVisit();
                vAllEdge[nEdgeIter]->getTgtNode()->setParent(u);
            }
        }
    }
    //back track
    node_C *backTrack = p_pTgtNode;
    if(p_pTgtNode->getParent() == 0)
        return false;
    while(backTrack!=p_pSrcNode){
        p_rvPath.push_back(backTrack);
        backTrack = backTrack->getParent();
    }
    p_rvPath.push_back(p_pSrcNode);
    reverse(p_rvPath.begin(), p_rvPath.end());
    return true;
}
//-----------------------------------------------------------------------------------------------------------//
bool maxflowMinCut_C::FordFulkerson()
{
    vector<node_C *> augmentingPath;
    while (bfs(m_pDmMgr->getSrcNode(), m_pDmMgr->getTgtNode(), augmentingPath)){
        int min = minCapacity(augmentingPath);
        maxflow += min;
        for (unsigned int nNodeIter = 0; nNodeIter < augmentingPath.size()-1; ++nNodeIter){
            node_C *u = augmentingPath[nNodeIter];
            node_C *v = augmentingPath[nNodeIter+1];
            u->getEdge(v)->addFlow(u, min);
        }
    } 
}
//-----------------------------------------------------------------------------------------------------------//
int maxflowMinCut_C::minCapacity(vector<node_C *> & augmentingPath){
    int min = INT_MAX;
    for (unsigned int nNodeIter = 0; nNodeIter < augmentingPath.size()-1; ++nNodeIter){
        node_C *u = augmentingPath[nNodeIter];
        node_C *v = augmentingPath[nNodeIter+1];

        if(u->getEdge(v)->getCapacity(u) < min)
            min = u->getEdge(v)->getCapacity(u);
    }
    return min;
}
//-----------------------------------------------------------------------------------------------------------//
void maxflowMinCut_C::dump() const
{
    ofstream output;
    output.open("max.txt");
    vector<edge_C*> vEdge;
    m_pDmMgr->getEdge(vEdge);
    for(unsigned int nEdgeIter = 0; nEdgeIter < vEdge.size(); ++nEdgeIter){
        if(vEdge[nEdgeIter]->getWeight()<0){
            output << "Max Flow: 0" << endl;
            output.close();
            return;
        }
    }
    output << "Max Flow: " << maxflow << endl;
    output.close();
}
//-----------------------------------------------------------------------------------------------------------//
