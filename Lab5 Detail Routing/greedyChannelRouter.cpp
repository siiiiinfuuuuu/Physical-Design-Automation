#include "greedyChannelRouter.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

bool Column::noFreeTrack(){
    for (int i = 0; i <= curr.size(); i++)
        if (curr[i] == 0)
            return false;
    return true;
}

GCR::GCR(int iW, int m, int s){
    netNum = 0;
    currCol = 0;
    initialWidth = iW;
    currWidth = iW;
    MJL = m;
    SNC = s;
}

void GCR::parser(char* fileName){
    fstream file;
    file.open(fileName, ios::in);
    string s;
    int tmp;
    getline(file, s);
    stringstream ss(s);
    while(ss>>tmp){
        top.push_back(tmp);
        if(tmp > netNum)
            netNum = tmp;
    }
    colNum = top.size();

    getline(file, s);
    stringstream ss2(s);
    while(ss2>>tmp)
        bottom.push_back(tmp);
    
    // for (int i = 0; i < top.size(); i++)
    //     cout << top[i] << " ";
    // cout << endl;

    // for (int i = 0; i < bottom.size(); i++)
    //     cout << bottom[i] << " ";
    // cout << endl;
    
    colH.resize(colNum);
    colV.resize(colNum);
    for (int i = 0; i < colNum; i++){
        nets[top[i]].topCol.insert(i);
        nets[bottom[i]].bottomCol.insert(i);
    }
}

void GCR::reverse_input(){
    reverse(top.begin(),top.end());
    reverse(bottom.begin(), bottom.end());
    // vector<int> tmp = top;
    // top = bottom;
    // bottom = tmp;
}

pair<int,int> GCR::greedyChannelRouter(){
    while (currCol < colNum){
        // cout << "current column = " << currCol << endl;
        // cout << "current width = " << currWidth << endl;
        // cout << "Step 1" << endl;
        Step1();
        // PrintCurrentColumn();
        // PrintNextColumn();
        // cout << "Step 2" << endl;
        Step2();
        // PrintCurrentColumn();
        // PrintNextColumn();
        // cout << "Step 3" << endl;
        Step3();
        // PrintCurrentColumn();
        // PrintNextColumn();
        // cout << "Step 4" << endl;
        Step4();
        // PrintCurrentColumn();
        // PrintNextColumn();
        // cout << "Step 5" << endl;
        Step5();
        // PrintCurrentColumn();
        // cout << "Step 6" << endl;
        // PrintNextColumn();
        // if(currCol == 15){
        //     ANS();
        //     OutputGDT();
        //     return{0,0};
        // }
        Step6();
    }
    while(NotComplete(currCol-1)){
        // cout << "current column = " << currCol << endl;
        // cout << "current width = " << currWidth << endl;
        extendCol();
        // PrintCurrentColumn();
        // cout << "Step 2" << endl;
        Step2();
        // PrintCurrentColumn();
        // cout << "Step 3" << endl;
        Step3();
        // PrintCurrentColumn();
        // cout << "Step 4" << endl;
        Step4();
        // PrintCurrentColumn();
        // cout << "Step 5" << endl;
        Step5();
        // PrintCurrentColumn();
        // cout << "Step 6" << endl;
        // PrintNextColumn();
        Step6();
        if(currCol>200){
            currCol--;
            return {currCol, currWidth};
        }
    }
    currCol--;
    return {currCol, currWidth};
}

void GCR::connectTop(int currCol, int topTrack){
    colH[currCol].curr[topTrack] = top[currCol];
    for (int i = currWidth; i > topTrack; i--)
        colV[currCol][i] = top[currCol];
}

void GCR::connectBottom(int currCol, int bottomTrack){
    colH[currCol].curr[bottomTrack] = bottom[currCol];
    for (int i = 0; i <= bottomTrack; i++)
        colV[currCol][i] = bottom[currCol];
}

int GCR::findTopTrack(int top, std::vector<int> currH){
    if(top == 0)
        return -1;
    for (int i = currWidth - 1; i >= 0; i--)
        if(currH[i] == 0 || currH[i] == top)
            return i;
    return -1;
}

int GCR::findBottomTrack(int bottom, std::vector<int> currH){
    if(bottom == 0)
        return -1;
    for (int i = 0; i < currWidth; i++)
        if(currH[i] == 0 || currH[i] == bottom)
            return i;
    return -1;
}

bool GCR::isEmptyV(Job j){
    for (int i = j.start + 1; i <= j.finish; i++)
        if(colV[currCol][i] != 0)
            return false;
    return true;
}

bool GCR::movable(int ta, int tb){
    //Is it legal to move track A to track B
    if (colH[currCol].curr[tb] != 0)
        return false;
    //falling
    if(ta>tb){
        for (int track = ta; track > tb; track--)
            if(colV[currCol][track]!=0)
                return false;
        return true;
    }
    //rising
    else if(ta < tb){
        for (int track = ta + 1; track <= tb; track++)
            if(colV[currCol][track]!=0)
                return false;
        return true;
    }
    return false;
}

void GCR::move(int ta, int tb){
    if(ta>tb){
        for (int track = ta; track > tb; track--)
            colV[currCol][track] = colH[currCol].curr[ta];
    }
    //rising
    else if(ta < tb){
        for (int track = ta + 1; track <= tb; track++)
            colV[currCol][track] = colH[currCol].curr[ta];
    }
    colH[currCol].curr[tb] = colH[currCol].curr[ta];
    colH[currCol].next[tb] = colH[currCol].curr[ta];
}

void GCR::Step1(){
    if (currCol == 0){
        colH[currCol].curr.resize(initialWidth, 0);
        colH[currCol].next.resize(initialWidth, 0);
        colV[currCol].resize(initialWidth + 1, 0);
    }
    else{
        colH[currCol].curr = colH[currCol - 1].next;
        colH[currCol].next.resize(currWidth, 0);
        colV[currCol].resize(currWidth + 1, 0);
    }

    int topTrack = findTopTrack(top[currCol], colH[currCol].curr);
    int bottomTrack = findBottomTrack(bottom[currCol], colH[currCol].curr);
    if(topTrack == -1 && bottomTrack == -1){
        //both top bottom fail
        if(top[currCol] == bottom[currCol]){
            //Figure 6
            for (int i = 0; i <= currWidth; i++)
                colV[currCol][i] = top[currCol];
        }
        else{
            if (top[currCol] != 0)
                colH[currCol].topFail = true;
            if (bottom[currCol] != 0)
                colH[currCol].bottomFail = true;
        }
    }
    else if(topTrack == -1){
        //top fail
        connectBottom(currCol, bottomTrack);
        if (top[currCol] != 0)
            colH[currCol].topFail = true;
    }
    else if(bottomTrack == -1){
        //bottom fail
        connectTop(currCol, topTrack);
        if (bottom[currCol] != 0)
            colH[currCol].bottomFail = true;
    }
    else{
        //both top bottom success
        if(topTrack > bottomTrack || (topTrack == bottomTrack && top[currCol] == bottom[currCol])){
            //no overlap
            connectTop(currCol, topTrack);
            connectBottom(currCol, bottomTrack);
        }
        else if(currWidth - topTrack < bottomTrack +1){
            //overlap -> pick top
            connectTop(currCol, topTrack);
            colH[currCol].bottomFail = true;
        }
        else{
            //overlap -> pick bottom
            connectBottom(currCol, bottomTrack);
            colH[currCol].topFail = true;
        }
    }
}

void GCR::Step2(){
    for (int i = 0; i < colH[currCol].curr.size(); i++)
        if(colH[currCol].curr[i]!=0)
            colH[currCol].PendingNet[colH[currCol].curr[i]].insert(i);
    vector<Job> jobs;
    for (auto it = colH[currCol].PendingNet.begin(); it != colH[currCol].PendingNet.end(); it++){
        set<int> s = it->second;
        auto sit = s.begin();
        for (int i = 0; i < s.size() - 1; i++){
            jobs.push_back({it->first, *sit++, *sit});
            //sit++;
        }
    }
    sort(jobs.begin(), jobs.end());
    // random_shuffle(jobs.begin(), jobs.end());
    vector<Job> selectedJobs;
    for(int i = 0; i < jobs.size(); i++)
        if(isEmptyV(jobs[i])){
            selectedJobs.push_back(jobs[i]);
            mergeNet.insert(jobs[i].netNum);
            colH[currCol].PendingNet[jobs[i].netNum].erase(jobs[i].start);
            colH[currCol].PendingNet[jobs[i].netNum].erase(jobs[i].finish);
            for (int j = jobs[i].start + 1; j <= jobs[i].finish; j++)
                colV[currCol][j] = jobs[i].netNum;
        }
    //decide next track of split net from selectedJob
    vector<vector<Job>> mergedJobs;
    

    for (int sJ = 0, mJ = 0; sJ < selectedJobs.size(); sJ++){
        if (sJ == 0){
            mergedJobs.resize(1);
            mergedJobs[mJ].push_back(selectedJobs[sJ]);
        }
        else if (selectedJobs[sJ].start == selectedJobs[sJ - 1].finish)
            mergedJobs[mJ].push_back(selectedJobs[sJ]);
        else{
            mergedJobs.resize(mergedJobs.size() + 1);
            mergedJobs[++mJ].push_back(selectedJobs[sJ]);
        }
    }
    for (int mJ = 0; mJ < mergedJobs.size(); mJ++){
        set<int> track;
        int net = mergedJobs[mJ][0].netNum;
        for (int j = 0; j < mergedJobs[mJ].size();j++){
            track.insert(mergedJobs[mJ][j].start);
            track.insert(mergedJobs[mJ][j].finish);
        }
        int nextTrack = -1;
        NET_STATE netState = NetState(net);
        if(netState == rising) nextTrack = *track.rbegin();
        else if(netState == falling ) nextTrack = *track.begin();
        else if(netState == steady){
            if(track.lower_bound(currWidth/2)!=track.end())
                nextTrack = *track.lower_bound(currWidth / 2);
            else
                nextTrack = *track.rbegin();
                // nextTrack = *track.rbegin(); *********************************
        }
        else if(netState == complete /*&& colH[currCol].PendingNet[net].size()==0*/) continue;
        //else if(netState == complete) nextTrack = *track.rbegin();

        colH[currCol].next[nextTrack] = net;
    }
}

void GCR::Step3(){
    for (auto it = colH[currCol].PendingNet.begin(); it != colH[currCol].PendingNet.end(); it++){
        if (it->second.size() > 1){
            int n = it->first;

            int bottomTrack = *it->second.begin();
            int bottom2Track = *(++it->second.begin());
            colH[currCol].PendingNet[n].erase(bottomTrack);
            int avaiableTrack = -1;
            for (int t = bottomTrack + 1; t < bottom2Track; t++)
                if(movable(bottomTrack, t))
                    avaiableTrack = t;
            if (avaiableTrack != -1 && avaiableTrack - bottomTrack >= MJL)
                move(bottomTrack, avaiableTrack);
            else
                colH[currCol].next[bottomTrack] = n;

            int topTrack = *it->second.rbegin();
            int top2Track = *(++it->second.rbegin());
            colH[currCol].PendingNet[n].erase(topTrack);
            avaiableTrack = -1;
            for (int t = topTrack - 1; t > top2Track; t--)
                if(movable(topTrack, t))
                    avaiableTrack = t;
            if (avaiableTrack != -1 && topTrack - avaiableTrack >= MJL)
                move(topTrack, avaiableTrack);
            else
                colH[currCol].next[topTrack] = n;

        }
    }
}

void GCR::Step4(){
    for (auto it = colH[currCol].PendingNet.begin(); it != colH[currCol].PendingNet.end(); it++){
        if (it->second.size() == 1 /*&& mergeNet.find(it->first)!=mergeNet.end()*/){
            int n = it->first;
            NET_STATE netState = NetState(n);
            int track = *it->second.begin();
            colH[currCol].PendingNet[n].erase(track);
            if(netState == rising){
                int avaiableTrack = -1;
                for (int t = track + 1; t < currWidth - 1; t++){
                    if(movable(track, t))
                        avaiableTrack = t;
                }
                if (avaiableTrack != -1 && avaiableTrack - track >= MJL)
                    move(track, avaiableTrack);
                else   
                    colH[currCol].next[track] = n;
            }
            else if(netState == falling){
                int avaiableTrack = -1;
                for (int t = track - 1; t > 0; t--)
                    if(movable(track, t))
                        avaiableTrack = t;
                    if (avaiableTrack != -1 && track - avaiableTrack >= MJL)
                        move(track, avaiableTrack);
                    else
                        colH[currCol].next[track] = n;
            }
            else if(netState == steady){
                colH[currCol].next[track] = n;
            }
        }
    }
    //Extend net still in PendingNet
    for (auto it = colH[currCol].PendingNet.begin(); it != colH[currCol].PendingNet.end(); it++)
        for (auto itt = it->second.begin(); itt != it->second.end(); itt++){
                colH[currCol].next[*itt] = it->first;
        }
            
}

void GCR::Step5(){
    if(colH[currCol].bottomFail){
        int insertTrack = 0;
        int mid = 2;
        NET_STATE ns = NetState(bottom[currCol]);
        if(ns==falling)
            mid = 10;
        else if (ns==rising)
            mid = 1;
        while(insertTrack <= currWidth/mid && colV[currCol][insertTrack] == 0)
            colV[currCol][insertTrack++] = bottom[currCol];
        for (int i = 0; i <= currCol; i++){
            colH[i].curr.insert(colH[i].curr.begin() + insertTrack - 1, 0);
            colV[i].insert(colV[i].begin() + insertTrack , colV[i][insertTrack - 1]);
        }
        colH[currCol].next.insert(colH[currCol].next.begin() + insertTrack-1, bottom[currCol]);
        colH[currCol].curr[insertTrack-1] = bottom[currCol];
        colV[currCol][insertTrack] = 0;
        currWidth++;
    }
    // PrintCurrentColumn();
    // PrintNextColumn();
    if(colH[currCol].topFail){
        int insertTrack = currWidth;
        int mid = 2;
        NET_STATE ns = NetState(top[currCol]);
        if(ns==falling)
            mid = 10;
        else if (ns==rising)
            mid = 1;

        while(insertTrack >=  currWidth/mid && colV[currCol][insertTrack] == 0)
            colV[currCol][insertTrack--] = top[currCol];
        for (int i = 0; i <= currCol; i++){
            colH[i].curr.insert(colH[i].curr.begin() + insertTrack + 1, 0);
            colV[i].insert(colV[i].begin() + insertTrack + 1, colV[i][insertTrack + 1]);
        }
        colH[currCol].next.insert(colH[currCol].next.begin() + insertTrack+1, top[currCol]);
        colH[currCol].curr[insertTrack + 1] = top[currCol];
        colV[currCol][insertTrack + 1] = 0;
        currWidth++;
    }
    // PrintCurrentColumn();
    // PrintNextColumn();
}

void GCR::Step6(){
    currCol++;
}

void GCR::extendCol(){
    colH.resize(colH.size() + 1);
    colV.resize(colV.size() + 1);
    colH[currCol].curr = colH[currCol - 1].next;
    colH[currCol].next.resize(currWidth, 0);
    colV[currCol].resize(currWidth + 1, 0);
}

NET_STATE GCR::NetState(int n){
    int nextTop = -1;
    int nextBottom = -1;
    if(nets[n].topCol.upper_bound(currCol)!=nets[n].topCol.end())
        nextTop = *nets[n].topCol.upper_bound(currCol);
    if(nets[n].bottomCol.upper_bound(currCol)!=nets[n].bottomCol.end())
        nextBottom = *nets[n].bottomCol.upper_bound(currCol);
    if (nextTop == -1 && nextBottom == -1 && colH[currCol].bottomFail && bottom[currCol] == n) return falling;
    if(nextTop == -1 && nextBottom == -1 && colH[currCol].topFail&&top[currCol]==n) return rising;
    else if (nextTop == -1 && nextBottom == -1) return complete;
    else if (nextTop == -1) return falling;
    else if (nextBottom == -1) return rising;
    else if(nextTop == nextBottom) return steady;
    else if(nextTop - nextBottom > SNC) return falling ;
    else if (nextBottom - nextTop > SNC) return rising;
    else return steady;
}

bool GCR::NotComplete(int currCol){
    for (int i = 0; i < currWidth; i++)
        if(colH[currCol].next[i]!=0)
            return true;
    return false;
}

void GCR::PrintCurrentColumn(){
    cout << "column " << currCol << endl;
    cout << "Current Horizontal" << endl << endl << setw(4) <<"";
    for (int i = 0; i <= currCol;i++)
        cout << setw(4)<< i;
    cout << endl;
    for (int i = currWidth - 1; i >= 0; i--){
        cout << setw(4) << i;
        for (int j = 0; j <= currCol;j++)
            cout << setw(4)<< colH[j].curr[i];
        cout << endl;
    }
        
    cout << endl << "Current Verical" << endl << endl  << setw(4) <<"";
    for (int i = 0; i <= currCol;i++)
        cout << setw(4)<< i;
     cout << endl;
    for (int i = currWidth; i >= 0; i--){
        cout << setw(4) << i;
        for (int j = 0; j <= currCol;j++)
            cout << setw(4) << colV[j][i];
        cout << endl;
    }
        
    cout << endl;
}

void GCR::PrintNextColumn(){
    cout << "column " << currCol << endl;
    cout << "Next Horizontal" << endl;
    for (int i = currWidth - 1; i >= 0; i--)
        cout << colH[currCol].next[i] << endl;
    cout << endl;
}

void GCR::ANS(){
    for (int w = 0; w < currWidth; w++){
        int currNet = colH[0].curr[w];
        int start = 0;
        for (int col = 1; col <= currCol; col++){
            if(colH[col].curr[w]!=currNet){
                if(currNet!=0 && start+1!=col)
                    ans[currNet].push_back({'H',start+1, w+2, col});
                start = col;
                currNet = colH[col].curr[w];
            }
        }
        if(currNet!=0&& start+1!=currCol+1)
            ans[currNet].push_back({'H',start+1, w+2, currCol+1});
    }
    for (int col = 0; col <= currCol; col++){
        int currNet = 0;
        int start = 0;
        for (int w = 0; w <= currWidth; w++){
            if(colV[col][w]!=currNet){
                if(currNet!=0 && start+1!= w+1)
                    ans[currNet].push_back({'V',col+1, start+1, w+1});
                start = w;
                currNet = colV[col][w];
            }
        }
        if(currNet!=0)
            ans[currNet].push_back({'V', col + 1, start + 1, currWidth + 2});
    }
}

void GCR::OutputGDT(){
    fstream file;
    file.open("output.gdt", ios::out);
    file<<"gds2{600\nm=2018-09-14 14:26:15 a=2018-09-14 14:26:15\nlib 'asap7sc7p5t_24_SL' 0.1 1e-5\ncell{c=2018-09-14 14:26:15 m=2018-09-14 14:26:15 'AND2x2_ASAP7_75t_SL'\n";

    for (auto it = ans.begin(); it != ans.end(); it++){
        // cout << it->first << endl;
        for (int i = 0; i < it->second.size(); i++){
            Line l = it->second[i];
            // cout << l.d << " " << l.a << " " << l.b << " " << l.c << endl;
            if(l.d=='H')
                file << "b{" << it->first << " xy(" << l.a - 0.1 << " " << l.b - 0.1 << " " << l.a - 0.1 << " " << l.b + 0.1 << " " << l.c + 0.1 << " " << l.b + 0.1 << " " << l.c + 0.1 << " " << l.b - 0.1 << ")}" << endl;
            else
                file << "b{" << it->first << " xy(" << l.a - 0.1 << " " << l.b - 0.1 << " " << l.a - 0.1 << " " << l.c + 0.1 << " " << l.a + 0.1 << " " << l.c + 0.1 << " " << l.a + 0.1 << " " << l.b - 0.1 << ")}" << endl;
        }
        // cout << endl;
    }
    file << "}\n}\n";
}

void GCR::Output(){
    fstream file;
    file.open("output", ios::out);
    for (auto it = ans.begin(); it != ans.end(); it++){
        file << ".begin " << it->first << endl;
        for (int i = 0; i < it->second.size(); i++){
            Line l = it->second[i];
            file << "." << l.d << " " << l.a << " " << l.b << " " << l.c << endl;
        }
        file << ".end" << endl;
    }
}