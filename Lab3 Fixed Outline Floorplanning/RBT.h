#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

class RBT;
class TreeNode{
private:
    TreeNode *leftchild;
    TreeNode *rightchild;
    TreeNode *parent;
    int key;
    int value;
    int color;             // for color: 0: Red, 1: Black
    friend class RBT;
public:
    TreeNode():leftchild(0),rightchild(0),parent(0),key(0),value(0),color(0){}
    TreeNode(int key, int value):leftchild(0),rightchild(0),parent(0),key(key),value(value),color(0){}
    
    int GetKey() const{return key;};
    int GetValue() const{return value;};
};

class RBT{
private:
    TreeNode *root;
    

    void LeftRotation(TreeNode *current);
    void RightRotation(TreeNode *current);
    void InsertFixedUpRBT(TreeNode *current);
    void DeleteFixedUpRBT(TreeNode *current);
    TreeNode* Successor(TreeNode *current);       // called by DeleteRBT()
    TreeNode* Leftmost(TreeNode *current);        // called by Successor()
public:
    RBT(){
        neel = new TreeNode;    // neel 需要配置記憶體
        neel->color = 1;        // neel 是黑色
        root = neel;            // 為了insert, 要把root 初始化成neel
        root->parent = neel;
    };
    
    TreeNode* InsertRBT(int key, int value);
    void DeleteRBT(int KEY);
    TreeNode* Successor_2(TreeNode *current);       // called by DeleteRBT()
    TreeNode* FindRBT(int key);
    TreeNode *neel;
    TreeNode* Search(int KEY);   // called by DeleteRBT()
    
};