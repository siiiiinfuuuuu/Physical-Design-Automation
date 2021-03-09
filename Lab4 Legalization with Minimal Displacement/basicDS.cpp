#include "basicDS.h"

int Cell::Cost(){
    Point delta = gPos - lPos;
    return delta.x * delta.x + delta.y * delta.y;
}

void SubRow::PlaceRow(){
    clusters.resize(0);
    placeFail = false;
    for (int i = 0; i < cells.size(); i++){
        
        if(i == 0 || clusters.back().xC+clusters.back().wC <= cells[i]->gPos.x){
            Cluster c;
            c.xC = cells[i]->gPos.x;
            AddCell(&c, cells[i]);
            clusters.push_back(c);
        }
        else{
            AddCell(&clusters.back(), cells[i]);
        }
        Collapse(clusters.size()-1);
    }
    for (int i = 0; i < clusters.size(); i++){
        int x = clusters[i].xC;
        for (int j = 0; j < clusters[i].cells.size(); j++){
            clusters[i].cells[j]->lPos.x = x;
            clusters[i].cells[j]->lPos.y = y;
            x += clusters[i].cells[j]->size.x;
        }
    }
}

void SubRow::AddCell(Cluster* c, Cell* cell){
    c->cells.push_back(cell);
    c->eC++;
    c->qC += cell->gPos.x - c->wC;
    c->wC += cell->size.x;
}

void SubRow::Collapse(int cIndex){
    Cluster* c = &clusters[cIndex];
    c->xC = c->qC / c->eC;
    if(c->wC > xMax - xMin){
        placeFail = true;
        return;
    }
    if(c->xC < xMin) c->xC = xMin;
    if(c->xC > xMax-c->wC) c->xC = xMax-c->wC;
    if (cIndex > 0){
        Cluster* cc = &clusters[cIndex - 1];
        if (cc->xC + cc->wC > c->xC){
            AddCluster(cIndex - 1, cIndex);
            clusters.erase(clusters.begin() + cIndex);
            Collapse(cIndex - 1);
        }
    }
}

void SubRow::AddCluster(int cIndex, int ccIndex){
    for (int i = 0; i < clusters[ccIndex].cells.size(); i++)
        clusters[cIndex].cells.push_back(clusters[ccIndex].cells[i]);
    clusters[cIndex].eC += clusters[ccIndex].eC;
    clusters[cIndex].qC += (clusters[ccIndex].qC - clusters[ccIndex].eC * clusters[cIndex].wC);
    clusters[cIndex].wC += clusters[ccIndex].wC;
}