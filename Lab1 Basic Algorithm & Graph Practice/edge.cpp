#include <cassert>
#include "dm.h"




//-----------------------------------------------------------------------------------------------------------//
int edge_C::getCapacity(const node_C* p_pNode) const
{
    if(p_pNode == m_pSrcNode) return (m_nWeight - m_nFlow);
    if(p_pNode == m_pTgtNode) return (m_nFlow);
    assert(0);
}
//-----------------------------------------------------------------------------------------------------------//
void edge_C::addFlow(node_C* p_pNode,int p_nFlow)
{
    if(p_pNode == m_pSrcNode) m_nFlow = m_nFlow + p_nFlow;
    else if(p_pNode == m_pTgtNode) m_nFlow = m_nFlow - p_nFlow;
    else assert(0);
}
//-----------------------------------------------------------------------------------------------------------//
