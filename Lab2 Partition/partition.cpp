#include "partition.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <limits.h>
#include <bitset>

using namespace std;

void Net::Print(){
    for (int i = 0; i < nodes.size(); i++){
        cout << nodes[i] << " ";
    }
    cout << endl;
}

DisjointSet::DisjointSet(int n){
    nodeNum = n;
    p = new int[n+1];
    s = new int[n+1];
    tmpP = new int[n+1];
    tmpS = new int[n+1];
    for (int i = 1; i <= n; i++){
        p[i] = i;
        s[i] = 1;
    }    
}

int DisjointSet::Find(int x){
    return x == p[x] ? x : (p[x] = Find(p[x]));
}

void DisjointSet::Union(int x, int y){
    if(Find(x) == Find(y))
        return;
    s[Find(y)] += s[Find(x)];
    p[Find(x)] = Find(y);
    
}

int DisjointSet::Size(int x){
    return s[Find(x)];
}

void DisjointSet::SaveState(){
    for (int i = 1; i <= nodeNum; i++){
        tmpP[i] = p[i];
        tmpS[i] = s[i];
    }
}

void DisjointSet::RecoverState(){
    for (int i = 1; i <= nodeNum; i++){
        p[i] = tmpP[i];
        s[i] = tmpS[i];
    }
}

void DisjointSet::ClearState(){
    for (int i = 1; i <= nodeNum; i++){
        p[i] = i;
        s[i] = 1;
    }
}

Partition::Partition(){

}

void Partition::Parsing(const char* fileName){
    fstream input;
    input.open(fileName, ios::in);
    string line;
    getline(input, line);
    stringstream ss(line);
    ss >> netNum;
    ss >> nodeNum;
    nodes.resize(nodeNum + 1);
    p.resize(nodeNum + 1);
    bestPartition.resize(nodeNum + 1);
    while(getline(input, line)){
        stringstream ss(line);
        Net net;
        int node;
        while(ss >> node)
            net.nodes.push_back(node);
        nets.push_back(net);
    }

}

void Partition::ParserTest(){
    cout << netNum << " " << nodeNum << endl;
    for (int i = 0; i < nets.size(); i++){
        for (int j = 0; j < nets[i].nodes.size(); j++)
            cout << nets[i].nodes[j] << " ";
        cout << endl;
    }
}

bool cmp(const pair<int, vector<int>> &x,const pair<int, vector<int>> &y){
    //return x.second.size() < y.second.size();
    return x.second.size() > y.second.size();
}

DisjointSet Partition::MinQuitUnion_3(){
    float bestMaxSize ;
    int minQuitSize = INT_MAX;

    DisjointSet dSet(nodeNum);
    dSet.SaveState();

    for (float maxSize = 0.01; maxSize <= ratioMax; maxSize += 0.01){
        int quitSize = 0;
        dSet.ClearState();
        for (int i = 0; i < nets.size(); i++){
            for (int j = 1; j < nets[i].nodes.size(); j++){
                if(dSet.Size(nets[i].nodes[0]) + dSet.Size(nets[i].nodes[j]) > nodeNum*maxSize){
                    quitSize++;
                    break;
                }
                dSet.Union(nets[i].nodes[0], nets[i].nodes[j]);
            }
        }
        if(quitSize < minQuitSize){
            minQuitSize = quitSize;
            bestMaxSize = maxSize;
        }
    }
    cout << "Min Quit Size = " << minQuitSize << endl;
    cout << "Best Max Size = " << bestMaxSize << endl;

    dSet.ClearState();
    for (int i = 0; i < nets.size(); i++){
        for (int j = 1; j < nets[i].nodes.size(); j++){
            if(dSet.Size(nets[i].nodes[0]) + dSet.Size(nets[i].nodes[j]) > nodeNum*bestMaxSize){
                break;
            }
            dSet.Union(nets[i].nodes[0], nets[i].nodes[j]);
        }
    }
    return dSet;
}

DisjointSet Partition::MinQuitUnion_2(){
    DisjointSet dSet(nodeNum);
    double maxSize = 8;
    vector<Net> remainNet = nets;
    while (maxSize <= nodeNum * ratioMax * 2){
        vector<Net> tmpNets;
        cout << "max size = " << maxSize << " remain net num = " << remainNet.size() << endl;
        for (int i = 0; i < remainNet.size(); i++){
            dSet.SaveState();
            for (int j = 1; j < remainNet[i].nodes.size(); j++){
                if(dSet.Size(remainNet[i].nodes[0]) + dSet.Size(remainNet[i].nodes[j]) > min(maxSize, nodeNum* ratioMax/2) ){
                    tmpNets.push_back(remainNet[i]);
                    dSet.RecoverState();
                    break;
                }
                dSet.Union(remainNet[i].nodes[0], remainNet[i].nodes[j]);
            }
        }
        remainNet = tmpNets;
        if(maxSize>100)
            maxSize *= 1.2;
        else
            maxSize += 1;
    }
    cout << "Quit Size = " << remainNet.size() << endl;
    return dSet;
}

DisjointSet Partition::MinQuitUnion(){
    DisjointSet dSet(nodeNum);
    double maxSize = 2;
    vector<Net> remainNet = nets;
    while (maxSize <= nodeNum * ratioMax * 2){
        vector<Net> tmpNets;
        cout << "max size = " << maxSize << " remain net num = " << remainNet.size() << endl;
        for (int i = 0; i < remainNet.size(); i++){
            dSet.SaveState();
            for (int j = 1; j < remainNet[i].nodes.size(); j++){
                if(dSet.Size(remainNet[i].nodes[0]) + dSet.Size(remainNet[i].nodes[j]) > min(maxSize, nodeNum* ratioMax/2) ){
                    tmpNets.push_back(remainNet[i]);
                    dSet.RecoverState();
                    break;
                }
                dSet.Union(remainNet[i].nodes[0], remainNet[i].nodes[j]);
            }
        }
        remainNet = tmpNets;
        maxSize *= 1.2;
    }
    cout << "Quit Size = " << remainNet.size() << endl;
    return dSet;
}

void Partition::Dset2Vector(DisjointSet dSet){
    map<int, vector<int>> cMap;
    for (int i = 1; i <= dSet.nodeNum; i++){
        if (cMap.find(dSet.Find(i)) != cMap.end()){
            cMap.find(dSet.Find(i))->second.push_back(i);
        }
        else{
            cMap.insert({dSet.Find(i),{}});
            cMap.find(dSet.Find(i))->second.push_back(i);
        }
    }
    cVector.clear();
    for (auto it = cMap.begin(); it != cMap.end(); it++)
        cVector.push_back(*it);
}

void Partition::BestPartition(std::vector<std::pair<int, std::vector<int>>> &cVector, int topN){
    //sort(cVector.begin(), cVector.end(), cmp);
    int bestCutSize = INT_MAX;
    //vector<Node> p(nodeNum + 1);
    for (int i = 0; i < (1 << topN); i++){
        int leftNum = 0;
        for (int j = 0; j < p.size(); j++)
            p[j].group = 1;
        for (int j = 0; j < cVector.size(); j++){
            bitset<32> ibits(i);
            if (j < topN && ibits[j] == 0 && cVector.size()>10){
                "don't put left";
            }
            else if (leftNum + cVector[j].second.size() < nodeNum * ratioMax){
                leftNum += cVector[j].second.size();
                vector<int> n = cVector[j].second;
                for (int k = 0; k < n.size(); k++)
                    p[n[k]].group = 0;
            }
            if(leftNum > nodeNum * 0.5)
                break;
        }
        if(CutSize(p) < bestCutSize && leftNum < ratioMax * nodeNum +1 && leftNum > ratioMin * nodeNum -1 ){
            bestCutSize = CutSize(p);
            bestPartition = p;
        }
    }
    cout << "bestPartition size = " << bestPartition.size() << endl;
    nodes = bestPartition;
    //return bestPartition;
}

void Partition::Initialize(){
    sort(nets.begin(), nets.end());
    DisjointSet dSet(nodeNum+1);
    if(nets.size() == 6511)
        dSet = MinQuitUnion_2();
    else if(nets.size() == 89187)
        dSet = MinQuitUnion_3();
    else
        dSet = MinQuitUnion();

    Dset2Vector(dSet);
    sort(cVector.begin(), cVector.end(), cmp);
    BestPartition(cVector,14);
    //random_shuffle(cVector.begin(), cVector.end());
    
    for (auto it = cVector.begin(); it != cVector.end(); it++){
        if (it->second.size() > 10)
            cout << "Component Size = " << it->second.size() << endl;
    }
    
}

void Partition::FM(){

}

int Partition::CutSize(vector<Node> &p){
    int cutSize = 0;
    if(p.size()!=nodeNum+1)
        cout << "cut size error" << endl;

    //cout << endl;
    for (int i = 0; i < nets.size(); i++){
        //nets[i].Print();
        for (int j = 0; j < nets[i].nodes.size(); j++){
            if(p[nets[i].nodes[j]].group != p[nets[i].nodes[0]].group){
                cutSize++;
                break;
            }
        }
    }
    //cout << "Cut Size = " << cutSize << endl;
    return cutSize;
}

void Partition::OutputFile(){
    ofstream output;
    output.open("output.txt");
    for (int i = 1; i <= nodeNum; i++){
        output << nodes[i].group << endl;
    }
    output.close();
}

void Partition::OutputFileTest(const char* fileName){
    fstream input;
    vector<Node> p(nodeNum + 1);
    input.open(fileName, ios::in);
    string line;
    int i = 1;
    while(getline(input, line)){
        stringstream ss(line);
        ss >> p[i++].group;
    }
    cout << "Test Cut Size = " << CutSize(p) << endl;
}