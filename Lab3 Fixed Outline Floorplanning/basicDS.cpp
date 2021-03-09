#include "basicDS.h"
#include <algorithm>
#include <time.h>


using namespace std;

SequencePair::SequencePair(int s){
    size = s;
    posGamma.resize(size + 1);
    negGamma.resize(size + 1);
    posMatch.resize(size + 1);
    negMatch.resize(size + 1);
    posRMatch.resize(size + 1);
    blockPosition.resize(size + 1);
    for (int i = 1; i <= size; i++){
        posGamma[i] = i;
        negGamma[i] = i;
    }
    //srand(unsigned(time(0)));
    random_shuffle(posGamma.begin() + 1, posGamma.end());
    random_shuffle(negGamma.begin() + 1, negGamma.end());
    random_shuffle(posGamma.begin() + 1, posGamma.end());
    random_shuffle(negGamma.begin() + 1, negGamma.end());
    random_shuffle(posGamma.begin() + 1, posGamma.end());
    random_shuffle(negGamma.begin() + 1, negGamma.end());
    posRGamma = posGamma;
    reverse(posRGamma.begin() + 1, posRGamma.end());
    for (int i = 1; i <= size; i++){
        posMatch[posGamma[i]] = i;
        negMatch[negGamma[i]] = i;
        posRMatch[posRGamma[i]] = i;
    }
    

    for (int i = 1; i <= size; i++){
        cout << posGamma[i] << " ";
    }
    cout << endl;
    for (int i = 1; i <= size; i++){
        cout << posRGamma[i] << " ";
    }
    cout << endl;
    for (int i = 1; i <= size; i++){
        cout << negGamma[i] << " ";
    }
    cout << endl;
}

void SequencePair::operator=(const SequencePair& sp){
    posGamma = sp.posGamma;
    negGamma = sp.negGamma;
    posMatch = sp.posMatch;
    negMatch = sp.negMatch;
    posRGamma = sp.posRGamma;
    posRMatch = sp.posRMatch;
}

void SequencePair::CalPosition(std::vector<Block*> blocks){
    RBT BST_x;
    BST_x.InsertRBT(0, 0);
    for (int i = 1; i <= size; i++){
        int b = posGamma[i];
        int p = negMatch[b];
        blockPosition[b].x = BST_x.FindRBT(p)->GetValue();
        int t = blockPosition[b].x + blocks[b]->size.x;
        DiscardTest(BST_x, BST_x.InsertRBT(p, t));
    }
    outline.x = BST_x.FindRBT(size)->GetValue();

    RBT BST_y;
    BST_y.InsertRBT(0, 0);
    for (int i = 1; i <= size; i++){
        int b = posRGamma[i];
        int p = negMatch[b];
        blockPosition[b].y = BST_y.FindRBT(p)->GetValue();
        int t = blockPosition[b].y + blocks[b]->size.y;
        DiscardTest(BST_y, BST_y.InsertRBT(p, t));
    }
    outline.y = BST_y.FindRBT(size)->GetValue();
}

void SequencePair::DiscardTest(RBT &BST, TreeNode* curr){
    TreeNode *next;
    int t = curr->GetValue();
    curr = BST.Successor_2(curr);
    while(curr!=BST.neel){
        next = BST.Successor_2(curr);
        if(curr->GetValue()<t)
            BST.DeleteRBT(curr->GetKey());
        else
            return;
        curr = next;
    }
}

int SequencePair::Area(vector<Block*> blocks){
    CalPosition(blocks);
    return outline.x * outline.y;
}

void SequencePair::Print(){
    cout << "posGamma  ";
    for (int i = 1; i <= size; i++)
        cout << posGamma[i] << " ";
    cout << endl;
    cout << "negGamma  ";
    for (int i = 1; i <= size; i++)
        cout << negGamma[i] << " ";
    cout << endl;
    cout << "posRGamma ";
    for (int i = 1; i <= size; i++)
        cout << posRGamma[i] << " ";
    cout << endl
         << endl;
}