#include "greedyChannelRouter.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    Result bestP;
    pair<int, int> bestResult = {999,999};
    for (int inital = 1; inital <= 23; inital++){
        for (int SNC = 20; SNC >= 7; SNC--){
            for (int MJL = 1; MJL < 10; MJL++){
                // cout << "inital = " << inital << " MJL = " << MJL << " snc = " << SNC << endl;
                if (/*SNC == 20 &&*/ MJL == 1 && inital == 4)
                    continue;
                GCR gcr(inital, MJL, SNC);
                gcr.parser(argv[1]);
                // gcr.reverse_input();
                pair<int,int> result = gcr.greedyChannelRouter();
                if(result.second < bestResult.second){
                    bestResult = result;
                    bestP = {inital, MJL, SNC};
                }
            }

        }
    }
    cout << "best width = " << bestResult.second << " column = " << bestResult.first + 1<<endl;
    cout << "best inital = " << bestP.inital << " MJL = " << bestP.MJL << " SNC = " << bestP.SNC << endl;

    GCR gcr(bestP.inital, bestP.MJL, bestP.SNC);
   // GCR gcr(5, 3, 18);
    gcr.parser(argv[1]);
    // gcr.reverse_input();
    // cout << "netNum = " << gcr.netNum << endl;
    gcr.greedyChannelRouter();
    // gcr.PrintCurrentColumn();
    gcr.ANS();
    gcr.Output();
    gcr.OutputGDT();
    cout << "end" << endl;

    return 0;
}

