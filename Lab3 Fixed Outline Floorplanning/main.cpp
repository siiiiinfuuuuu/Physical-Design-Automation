#include <stdio.h>
#include "floorplan.h"

int main(int argc, char** argv){
    Floorplan* fp = new Floorplan;
    fp->BlockPaser(argv[2]);
    fp->NetPaser(argv[3]);
    //SequencePair sq = fp->SA();
    
    //fp->OutputFile(sq, argv[4]);
    fp->OutputTest(argv[4]);
    // RBT tree;
    // tree.InsertRBT(0, 10);
    // tree.InsertRBT(1, 10);

    // tree.InsertRBT(5, 60);
    // tree.DeleteRBT(6);
    // tree.InsertRBT(1, 5);
    // tree.InsertRBT(2, 15);
    // tree.InsertRBT(3, 25);
    // tree.DeleteRBT(4);


    // SequencePair* sq = new SequencePair(fp->numBlocks);
    // sq->CalPosition(fp->blocks);
    // cout << sq->outline.x << " " << sq->outline.y << endl;
    
}