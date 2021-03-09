#include "placement.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits.h>
#include <math.h>

using namespace std;

Placement::Placement(char* auxFileName){
    fstream file;
    file.open(auxFileName, ios::in);
    string trash;
    string nodeFileName, plFileName, sclFileName;
    file >> trash >> trash >> nodeFileName >> trash >> trash >> plFileName >> sclFileName;
    // cout << "node : " << nodeFileName << "pl : " << plFileName << "scl : " << sclFileName << endl;
    file.close();

    NodeParser(nodeFileName.c_str());
    PlParser(plFileName.c_str());
    SclParser(sclFileName.c_str());
    nextRow = false;
    RowFlagInit();
    SubRowFlagInit();
}

void Placement::NodeParser(const char* nodeFileName){
    fstream file;
    file.open(nodeFileName, ios::in);
    string trash;
    getline(file,trash);
    getline(file,trash);
    getline(file,trash);
    file >> trash >> trash >> numCells >> trash >> trash >> numTerminals;
    // cout << "NumNodes : " << numNodes << " NumTerminals : " << numTerminals << endl;
    for (int i = 0; i < numCells; i++){
        Cell c;
        file >> c.name >> c.size.x >> c.size.y;
        getline(file,trash);
        cells.push_back(c);
        // cout << "x : " << tmp1 << " y : " << tmp2 << endl;
    }
    file.close();
}

void Placement::PlParser(const char* plFileName){
    fstream file;
    file.open(plFileName, ios::in);
    string trash;
    getline(file,trash);
    getline(file,trash);
    getline(file,trash);
    for (int i = 0; i < numCells; i++){
        file >> trash >> cells[i].gPos.x >> cells[i].gPos.y;
        getline(file,trash);
        // cout << "x : " << tmp1 << " y : " << tmp2 << endl;
    }
    file.close();
}

void Placement::SclParser(const char* plFileName){
    fstream file;
    file.open(plFileName, ios::in);
    string trash;
    getline(file,trash);
    getline(file,trash);
    getline(file,trash);
    file >> trash >> trash >> numRows;
    getline(file,trash);
    getline(file,trash);
    // cout << "NumRows : " << numRows << endl;
    height = 12;
    for (int i = 0; i < numRows; i++){
        Row *r = new Row;
        getline(file,trash);
        file >> trash >> trash >> r->y;
        getline(file,trash);
        for (int j = 0; j < 5; j++)
            getline(file,trash);
        file >> trash >> trash >> r->x >> trash >> trash >> r->numSites;
        getline(file,trash);
        getline(file,trash);
        rows.push_back(r);
        // cout << "Coordinate : " << tmp1 << " SubrowOrigin : " << tmp2 << " NumSites: " << tmp3 << endl;
    }
    file.close();
}

int Placement::RowIndex (int y){
    return (y - rows[0]->y) / height;
}

SubRow* Placement::NextRow(){
    if(currRowIndex==-999){
        currRowIndex = RowIndex(currCell->gPos.y);
        if(currRowIndex < 0) currRowIndex = 0;
        if(currRowIndex > numRows-1 ) currRowIndex = numRows-1;
        upRow = currRowIndex;
        downRow = currRowIndex;
        SubRowFlagInit();
    }
    else if(nextRow == true){
        if(upStop == false && (upRow == numRows-1 || pow(rows[upRow]->y-currCell->gPos.y,2) > bestCost)){
            upStop = true;
            up = false;
        }
        if(downStop==false && (downRow == 0 || pow(currCell->gPos.y-rows[downRow]->y,2) > bestCost)){
            downStop = true;
            up = true;
        }
        if(upStop && downStop) return nullptr;
        if(up == true){
            currRowIndex = ++upRow;
            SubRowFlagInit();
            if(downStop == false) up = false;
        }
        else if(up == false){
            currRowIndex = --downRow;
            SubRowFlagInit();
            if (upStop == false) up = true;
        }
        nextRow = false;
    }
    return NextSubRow(rows[currRowIndex]);
}

SubRow* Placement::NextSubRow(Row* currRow){
    if(currSubRowIndex ==-999){
        for (int i = 0; i < currRow->subRows.size(); i++){
            currSubRowIndex = i;
            if(currCell->gPos.x < currRow->subRows[i]->xMax)
                break;
        }
        leftRow = currSubRowIndex;
        rightRow = currSubRowIndex;
    }
    else{
        if(rightStop == false && (rightRow == currRow->subRows.size()-1 || pow(currRow->subRows[currSubRowIndex]->xMin - currCell->gPos.x,2) + pow(currCell->gPos.y - currRow->y,2)>bestCost)){
            rightStop = true;
            right = false;
        }
        if(leftStop == false && (leftRow == 0 || pow(currCell->gPos.x - currRow->subRows[currSubRowIndex]->xMin,2)+ pow(currCell->gPos.y - currRow->y,2)>bestCost)){
            leftStop = true;
            right = true;
        }
        if(rightStop && leftStop){
            nextRow = true;
            return currRow->subRows[currSubRowIndex];
        }
        if(right == true){
            currSubRowIndex = ++rightRow;
            if(leftStop == false) right = false;
        }
        else if(right == false){
            currSubRowIndex = --leftRow;
            if(rightStop == false) right = true;
        }
    }
    return currRow->subRows[currSubRowIndex];
}

void Placement::CutRow(){
    for (int i = numCells - numTerminals; i < numCells; i++){
        int firstRow = RowIndex(cells[i].gPos.y);
        int lastRow = RowIndex(cells[i].gPos.y + cells[i].size.y)-1;
        if (firstRow < 0)
            continue;
        // cout << "FirstRow = " << firstRow << " LastRow = " << lastRow << endl;
        for (int j = firstRow; j <= lastRow && j <= rows.size(); j++){
            //cout << "Row " << j << " ++ = " << cells[i].gPos.x << " -- = " << cells[i].gPos.x + cells[i].size.x << endl;
            if (cells[i].gPos.x < rows[j]->x + rows[j]->numSites && cells[i].gPos.x > rows[j]->x)
                rows[j]->cutPoint[cells[i].gPos.x]++;
            if (cells[i].gPos.x + cells[i].size.x < rows[j]->x + rows[j]->numSites && cells[i].gPos.x + cells[i].size.x > rows[j]->x)
                rows[j]->cutPoint[cells[i].gPos.x + cells[i].size.x]--;
        }
    }
    int numSubRow = 0;
    for (int i = 0; i < numRows; i++){
        int prevX = rows[i]->x;
        int prevSum = 0;
        int currX;
        int currSum;
        for (auto it = rows[i]->cutPoint.begin(); it != rows[i]->cutPoint.end(); it++){
            currX = (*it).first;
            currSum = prevSum + (*it).second;
            if (prevSum == 0 && currSum !=0){
                SubRow *sr = new SubRow(prevX, currX,rows[i]->y);
                rows[i]->subRows.push_back(sr);
                numSubRow++;
            }
            prevX = currX;
            prevSum = currSum;
        }

        currX = rows[i]->x + rows[i]->numSites;
        if (prevSum == 0 && prevX != currX){
            SubRow *sr = new SubRow(prevX, currX,rows[i]->y);
            rows[i]->subRows.push_back(sr);
            numSubRow++;
        }
    }
    cout << "numSubRow = " << numSubRow << endl;
}

void Placement::TestCutRow(){
    fstream file;
    file.open("test.gdt", ios::out);
    file<<"gds2{600\nm=2018-09-14 14:26:15 a=2018-09-14 14:26:15\nlib 'asap7sc7p5t_24_SL' 1 1e-6\ncell{c=2018-09-14 14:26:15 m=2018-09-14 14:26:15 'AND2x2_ASAP7_75t_SL'\n";
    //Terminal
    for (int i = numCells - numTerminals; i < numCells; i++){
        file << "b{" << "0" << " xy(" << cells[i].gPos.x << " " << cells[i].gPos.y << " " << cells[i].gPos.x << " " << cells[i].gPos.y + cells[i].size.y << " " << cells[i].gPos.x + cells[i].size.x << " " << cells[i].gPos.y + cells[i].size.y << " " << cells[i].gPos.x + cells[i].size.x << " " << cells[i].gPos.y << ")}" << endl;
    }
    //Row
    for (int i = 0; i < numRows; i++)
        file << "b{" << "1" << " xy(" << rows[i]->x << " " << rows[i]->y << " " << rows[i]->x << " " << rows[i]->y + height << " " << rows[i]->x + rows[i]->numSites << " " << rows[i]->y + height << " " << rows[i]->x + rows[i]->numSites << " " << rows[i]->y << ")}" << endl;
    //SubRow
    for (int i = 0; i < numRows; i++){
        for (int j = 0; j < rows[i]->subRows.size(); j++)
            file << "b{" << "2" << " xy(" << rows[i]->subRows[j]->xMin << " " << rows[i]->y << " " << rows[i]->subRows[j]->xMin << " " << rows[i]->y + height << " " << rows[i]->subRows[j]->xMax << " " << rows[i]->y + height << " " << rows[i]->subRows[j]->xMax << " " << rows[i]->y << ")}" << endl;
    }
    file << "}\n}\n";
}

void Placement::Abacus(){
    sort(cells.begin(), cells.begin()+numCells-numTerminals);
    for (int i = 0; i < numCells-numTerminals; i++){
        // cout << i << endl;
        // if(i%1000==0)
        //     cout << i << endl;
        bestCost = INT_MAX;
        SubRow* bestRow = nullptr;
        // if(i==TESTNUM)
        //     break;
        currCell = &cells[i];
        RowFlagInit();
        while(1){
            SubRow *sr = NextRow();
            if(sr == nullptr) break;
            sr->cells.push_back(&cells[i]);
            sr->PlaceRow();
            int cost = currCell->Cost();
            if(cost < bestCost && sr->placeFail == false){
                bestCost = cost;
                bestRow = sr;
            }
            sr->cells.pop_back();
        }
        bestRow->cells.push_back(currCell);
        bestRow->PlaceRow();
    }
    for (int i = 0; i < rows.size(); i++){
        for (int j = 0; j < rows[i]->subRows.size(); j++){
            if(rows[i]->subRows[j]->y == 8403 && rows[i]->subRows[j]->xMin == 9837)
                cout << "hi";
            rows[i]->subRows[j]->PlaceRow();
        }
    }
}

void Placement::TestAbacus(){
    fstream file;
    file.open("testAbacus.gdt", ios::out);
    file<<"gds2{600\nm=2018-09-14 14:26:15 a=2018-09-14 14:26:15\nlib 'asap7sc7p5t_24_SL' 1 1e-6\ncell{c=2018-09-14 14:26:15 m=2018-09-14 14:26:15 'AND2x2_ASAP7_75t_SL'\n";
    //Terminal
    for (int i = numCells - numTerminals; i < numCells; i++){
        file << "b{" << "0" << " xy(" << cells[i].gPos.x << " " << cells[i].gPos.y << " " << cells[i].gPos.x << " " << cells[i].gPos.y + cells[i].size.y << " " << cells[i].gPos.x + cells[i].size.x << " " << cells[i].gPos.y + cells[i].size.y << " " << cells[i].gPos.x + cells[i].size.x << " " << cells[i].gPos.y << ")}" << endl;
    }
    //localPlacement
    for (int i = 0; i < numCells - numTerminals; i++)
        file << "b{" << "1" << " xy(" << cells[i].lPos.x << " " << cells[i].lPos.y << " " << cells[i].lPos.x << " " << cells[i].lPos.y + cells[i].size.y << " " << cells[i].lPos.x + cells[i].size.x << " " << cells[i].lPos.y + cells[i].size.y << " " << cells[i].lPos.x + cells[i].size.x << " " << cells[i].lPos.y << ")}" << endl;
    //localPlacement
    for (int i = 0; i < numCells - numTerminals; i++)
        file << "b{" << "2" << " xy(" << cells[i].gPos.x << " " << cells[i].gPos.y << " " << cells[i].gPos.x << " " << cells[i].gPos.y + cells[i].size.y << " " << cells[i].gPos.x + cells[i].size.x << " " << cells[i].gPos.y + cells[i].size.y << " " << cells[i].gPos.x + cells[i].size.x << " " << cells[i].gPos.y << ")}" << endl;
    //SubRow
    for (int i = 0; i < numRows; i++){
        for (int j = 0; j < rows[i]->subRows.size(); j++)
            file << "b{" << "3" << " xy(" << rows[i]->subRows[j]->xMin << " " << rows[i]->y << " " << rows[i]->subRows[j]->xMin << " " << rows[i]->y + height << " " << rows[i]->subRows[j]->xMax << " " << rows[i]->y + height << " " << rows[i]->subRows[j]->xMax << " " << rows[i]->y << ")}" << endl;
    }
    file << "}\n}\n";
}

void Placement::RowFlagInit (){
    currRowIndex = -999;
    nextRow = false;
    up = true;
    upStop = false;
    downStop = false;
}

void Placement::SubRowFlagInit (){
    currSubRowIndex = -999;
    right = true;
    rightStop = false;
    leftStop = false;
}

void Placement::OutputFile(){
    fstream file;
    file.open("output.pl", ios::out);
    for (int i = 0; i < numCells; i++){
        if (i < numCells - numTerminals)
            file << cells[i].name << " " << cells[i].lPos.x << " " << cells[i].lPos.y << " : N\n";
        else
            file << cells[i].name << " " << cells[i].gPos.x << " " << cells[i].gPos.y << " : N / FIXED\n";
    }
    file.close();
}