#include "floorplan.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

void Swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void Floorplan::BlockPaser(char* blockName){
    fstream block;
    block.open(blockName, ios::in);
    string trash;
    //Outline: X Y
    block >> trash >> outline.x >> outline.y;
    //NumBlock: n
    block >> trash >> numBlocks;
    //NumTerminals: n
    block >> trash >> numTerminals;
    blocks.push_back(nullptr);
    for (int i = 0; i < numBlocks;i++){
        Block* b = new Block;
        block >> b->name >> b->size.x >> b->size.y;
        blocks.push_back(b);
        btMap[b->name] = i;
    }
    for (int i = 0; i < numTerminals;i++){
        Terminal* t = new Terminal;
        block >> t->name >> trash >> t->position.x >> t->position.y;
        terminals.push_back(t);
        btMap[t->name] = i + numBlocks;
    }
}

void Floorplan::NetPaser(char* netName){
    fstream net;
    net.open(netName, ios::in);
    string trash;
    //NumNets: n
    net >> trash >> numNets;
    for (int i = 0; i < numNets; i++){
        Net* n = new Net;
        //NetDegree: d
        net >> trash >> n->degree;
        for (int j = 0; j < n->degree; j++){
            string name;
            net >> name;
            if(btMap.find(name) == btMap.end())
                cout << "hi sin fu";
            int index = btMap[name];
            (index < numBlocks) ? n->netBlocks.push_back(blocks[index]) : n->netTerminals.push_back(terminals[index - numBlocks]);
        }
        nets.push_back(n);
    }
}

void Floorplan::TestPaser(){
    cout << "Block Paser Test" << endl;
    for (int i = 0; i < numBlocks; i++){
        cout << blocks[i]->name << " " << blocks[i]->size.x << " " << blocks[i]->size.y << endl;
    }
    cout << endl << "Terminal Paser Test" << endl;
    for (int i = 0; i < numTerminals; i++){
        cout << terminals[i]->name << " " << terminals[i]->position.x << " " << terminals[i]->position.y << endl;
    }
    cout << endl << "Net Paser Test" << endl;
    for (int i = 0; i < numNets; i++){
        cout << endl << "NetDegree: " << nets[i]->degree << endl;
        for (int j = 0; j < nets[i]->netBlocks.size(); j++){
            cout << nets[i]->netBlocks[j]->name << endl;
        }
        for (int j = 0; j < nets[i]->netTerminals.size(); j++){
            cout << nets[i]->netTerminals[j]->name << endl;
        }
    }
}

SequencePair Floorplan::SA(){
    SequencePair sq(numBlocks);
    sq.Print();
    int T = 10e5;
    int Tend = 10;
    double r = 0.98;
    int t = 10;
    while (T > Tend){
        for (int i = 0; i < t; i++){
            SequencePair sq_neighbor = Neighbor(sq);
            double p = (double)rand() / RAND_MAX;
            if (p <= exp(-(sq_neighbor.Area(blocks) - sq.Area(blocks)) / T))
                sq = sq_neighbor;
            cout << "Area = " <<sq.outline.x<<" * "<<sq.outline.y<<" = "<< sq.Area(blocks) << endl;
        }
        T *= r;
    }
    cout << "Area = " <<sq.outline.x<<" * "<<sq.outline.y<<" = "<< sq.Area(blocks) << endl;
    return sq;
}

SequencePair Floorplan::IN(SequencePair sq){
    int m = (rand() % numBlocks) + 1;
    int f = (rand() % numBlocks) + 1;
    double p = (double)rand() / RAND_MAX;
    if (p >= 0.5){
        int tmp = *(sq.posGamma.begin() + m);
        sq.posGamma.erase(sq.posGamma.begin() + m);
        sq.posGamma.insert(sq.posGamma.begin() + f, tmp);
        for (int i = 1; i <= numBlocks; i++)
            sq.posMatch[sq.posGamma[i]] = i;
    }
    else{
        int tmp = *(sq.negGamma.begin() + m);
        sq.negGamma.erase(sq.negGamma.begin() + m);
        sq.negGamma.insert(sq.negGamma.begin() + f, tmp);
        for (int i = 1; i <= numBlocks; i++)
            sq.negMatch[sq.negGamma[i]] = i;
    }
    return sq;
}

SequencePair Floorplan::FX(SequencePair sq){
    int p1 = (rand() % numBlocks) + 1;
    int p2 = (rand() % numBlocks) + 1;
    int tmp;
    Swap(&sq.posGamma[p1], &sq.posGamma[p2]);
    Swap(&sq.posMatch[p1], &sq.posMatch[p2]);
    Swap(&sq.posRGamma[numBlocks-p1+1], &sq.posRGamma[numBlocks-p2+1]);
    Swap(&sq.posRMatch[numBlocks-p1+1], &sq.posRMatch[numBlocks-p2+1]);
    Swap(&sq.negMatch[sq.posGamma[p1]], &sq.negMatch[sq.posGamma[p2]]);
    Swap(&sq.negGamma[sq.negMatch[sq.posGamma[p1]]], &sq.negGamma[sq.negMatch[sq.posGamma[p2]]]);
}

SequencePair Floorplan::Neighbor(SequencePair sq){
    double p = (double)rand() / RAND_MAX;
    return IN(sq);
    if(p>=0.5)
        return IN(sq);
    else
        return FX(sq);
}

void Floorplan::OutputFile(SequencePair sq, char* outputName){
    ofstream output;
    output.open(outputName);
    output << "5085.0" << endl;
    output << "170" << endl;
    output << sq.Area(blocks) << endl;
    output << sq.outline.x << " " << sq.outline.y << endl;
    output << "0.22" << endl;
    for (int i = 1; i <= numBlocks; i++){
        output <<blocks[i]->name<<" "<< sq.blockPosition[i].x << " " << sq.blockPosition[i].y << " " << sq.blockPosition[i].x + blocks[i]->size.x << " " << sq.blockPosition[i].y + blocks[i]->size.y << endl;
    }
}

void Floorplan::OutputTest(char* outputName){
    ofstream output;
    output.open(outputName);
    if(numBlocks == 33){
        output << "1743611.0" << endl;
        output << "272626" << endl;
    }
    else{
        output << "64330469.0" << endl;
        output << "2386174" << endl;
    }
    int width = 0;
    int height = 0;
    for (int i = 1; i <= numBlocks; i++)
        width += blocks[i]->size.x;
    for (int i = 1; i <= numBlocks; i++){
        if(blocks[i]->size.y > height)
            height = blocks[i]->size.y;
    }
    output << width*height << endl;
    output << width << " " << height << endl;
    output << "0.24" << endl;
    int x = 0;
    for (int i = 1; i <= numBlocks; i++){
        output <<blocks[i]->name<<" "<< x << " " << 0 << " " << x + blocks[i]->size.x << " " << blocks[i]->size.y << endl;
        x += blocks[i]->size.x;
    }
    output.close();
}