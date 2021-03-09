#ifndef _DMMGR_H_
#define _DMMGR_H_
#include "parser.h"
#include <climits>

class node_C;

//-----------------------------------------------------------------------------------------------------------//
class edge_C
{
    public:
        edge_C(string p_strName,node_C* p_pSrcNode, node_C* p_pTgtNode,int p_nWeight)
            : m_strName(p_strName), m_pSrcNode(p_pSrcNode), m_pTgtNode(p_pTgtNode), m_nWeight(p_nWeight), m_nFlow(0) {}
        //Accessor
        string  getName() const {return m_strName;}
        node_C* getSrcNode() const {return m_pSrcNode;}
        node_C* getTgtNode() const {return m_pTgtNode;}
        int     getWeight() const {return m_nWeight;}
        int     getCapacity(const node_C*) const;
        int     getFlow() const {return m_nFlow;}
        
        //Modifier 
        void setFlow(int p_nFlow) {m_nFlow = p_nFlow;}
        void addFlow(node_C*,int p_nFlow);
    
    private:
        string  m_strName;
        node_C* m_pSrcNode;
        node_C* m_pTgtNode;
        int     m_nWeight; 
        int     m_nFlow;
};
//-----------------------------------------------------------------------------------------------------------//
class node_C
{
    public:
        node_C(string p_strName)
           : m_strName(p_strName), m_vOutEdge(), m_vInEdge(), m_nCost(INT_MAX), m_bVisit(false), m_pParentNode(0) {}
        //Accessor
        string  getName() const {return m_strName;}
        int     getCost() const {return m_nCost;}
        node_C* getParent() const {return m_pParentNode;}
        bool    isVisit() const {return m_bVisit;}
        int     getCapacity(node_C*) const;
        edge_C* getEdge(node_C*) const;
        void    getAdjNode(vector<node_C*>& p_rvNode) const; 
        void    getAllEdge(vector<edge_C*>& p_rvEdge) const; 
        
        //Modifier  
        void setCost(int p_nCost) {m_nCost = p_nCost;}
        void setVisit() {m_bVisit = true;}
        void setUnvisit() {m_bVisit = false;}

        //Modifier - for initialization
        void addInEdge(edge_C* p_pInEdge) {m_vInEdge.push_back(p_pInEdge);}
        void addOutEdge(edge_C* p_pOutEdge) {m_vOutEdge.push_back(p_pOutEdge);}
        void getOutEdge(vector<edge_C*>& p_rOutEdge) {p_rOutEdge = m_vOutEdge;}
        void getInEdge(vector<edge_C*>& p_rInEdge) {p_rInEdge = m_vInEdge;}
        void setParent(node_C* p_pParent) {m_pParentNode = p_pParent;}

    private:
        string          m_strName;
        vector<edge_C*> m_vOutEdge;
        vector<edge_C*> m_vInEdge;
        int             m_nCost;
        bool            m_bVisit;
        node_C*         m_pParentNode;
};
//-----------------------------------------------------------------------------------------------------------//
class dmMgr_C
{
    public:
        dmMgr_C(graphData_S* p_pGraphData) 
            : m_pSrcNode(), m_pTgtNode(), m_vNode(), m_vEdge(), m_pGraphData(p_pGraphData) { establishLink(); }
        void    establishLink();
        void    getNode(vector<node_C*>& p_rNode) const {p_rNode = m_vNode;}
        void    getEdge(vector<edge_C*>& p_rEdge) const {p_rEdge = m_vEdge;}
        node_C* getSrcNode() const {return m_pSrcNode;}
        node_C* getTgtNode() const {return m_pTgtNode;}
        void    dump() const;
    private:
        node_C*         m_pSrcNode;
        node_C*         m_pTgtNode;
        vector<node_C*> m_vNode;
        vector<edge_C*> m_vEdge;
        graphData_S*    m_pGraphData;
};
//-----------------------------------------------------------------------------------------------------------//



#endif
