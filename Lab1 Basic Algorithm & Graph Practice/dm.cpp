#include <fstream>
#include "dm.h"



//-----------------------------------------------------------------------------------------------------------//
void dmMgr_C::establishLink()
{
    printf("[DM] - Establishing Link ...");
    node_C* pNode(0),*pSrcNode(0),*pTgtNode(0);
    edge_C* pEdge(0);
    net_S* pNet(0);
    char caEdgeName[64];
    string strTgtNode,strSrcNode,strEdgeName;
    int nNodeCount=0, nEdgeCount=0;
    
    for(unsigned int nNodeIter=0;nNodeIter<m_pGraphData->vNode.size();++nNodeIter)
    {
        pNode = new node_C(m_pGraphData->vNode[nNodeIter]);
        if(strcmp(m_pGraphData->vNode[nNodeIter].c_str(),m_pGraphData->caSrcNode)==0) m_pSrcNode = pNode;
        if(strcmp(m_pGraphData->vNode[nNodeIter].c_str(),m_pGraphData->caTgtNode)==0) m_pTgtNode = pNode;
        m_vNode.push_back(pNode);
        nNodeCount ++;
    }
    for(unsigned int nNetIter=0;nNetIter<m_pGraphData->vNet.size();++nNetIter)
    {
        pNet = m_pGraphData->vNet[nNetIter];
        sprintf(caEdgeName,"%s->%s",pNet->nodeSource,pNet->nodeTarget);
        strEdgeName = caEdgeName;
        strSrcNode = pNet->nodeSource;
        strTgtNode = pNet->nodeTarget;
        for(unsigned int nNodeIter=0;nNodeIter<m_vNode.size();++nNodeIter)
        {
            if(m_vNode[nNodeIter]->getName()==strSrcNode) pSrcNode = m_vNode[nNodeIter];
            if(m_vNode[nNodeIter]->getName()==strTgtNode) pTgtNode = m_vNode[nNodeIter];
        }
        pEdge = new edge_C(strEdgeName,pSrcNode,pTgtNode,pNet->nWeight);
        pSrcNode->addOutEdge(pEdge);
        pTgtNode->addInEdge(pEdge);
        m_vEdge.push_back(pEdge);
        nEdgeCount++;
    }
    printf("Complete! %d Nodes and %d Edges Established!\n",nNodeCount,nEdgeCount);
}
//-----------------------------------------------------------------------------------------------------------//
void dmMgr_C::dump() const
{
    node_C* pNode(0);
    ofstream output;
    output.open("dumpDM.txt");
    vector<edge_C*> vInEdge;
    vector<edge_C*> vOutEdge;
    for(unsigned int nNodeIter=0; nNodeIter<m_vNode.size();++nNodeIter)
    {
        pNode = m_vNode[nNodeIter];
        pNode->getInEdge(vInEdge);
        pNode->getOutEdge(vOutEdge);
        output<<pNode->getName()<<endl;
        output<<"{"<<endl;
        output<<"    //In Edge"<<endl;
        for(unsigned int nEdgeIter=0;nEdgeIter<vInEdge.size();++nEdgeIter)
        {
            output<<"    "<<vInEdge[nEdgeIter]->getName()<<":"
                          <<vInEdge[nEdgeIter]->getSrcNode()->getName()<<"->"
                          <<vInEdge[nEdgeIter]->getTgtNode()->getName()<<":"
                          <<vInEdge[nEdgeIter]->getWeight()<<endl;
        }
        output<<"    //Out Edge"<<endl;
        for(unsigned int nEdgeIter=0;nEdgeIter<vOutEdge.size();++nEdgeIter)
        {
            output<<"    "<<vOutEdge[nEdgeIter]->getName()<<":"
                          <<vOutEdge[nEdgeIter]->getSrcNode()->getName()<<"->"
                          <<vOutEdge[nEdgeIter]->getTgtNode()->getName()<<":"
                          <<vOutEdge[nEdgeIter]->getWeight()<<endl;
        }
        output<<"}"<<endl;
    }
    output.close();
}
//-----------------------------------------------------------------------------------------------------------//
