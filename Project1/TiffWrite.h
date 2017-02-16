//
// Created by turner on 2/15/17.
//

#ifndef CLI_TIFFWRITE_H
#define CLI_TIFFWRITE_H
using namespace std;

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <cstring>
#include <stdlib.h>

#include "Module.h"
#include "Dispatcher.h"


class TiffRead;

class TiffWrite: public Module {

public:
    //vars
    string filename;

    //methods
    void doCommand(vector<string>);
    bool parseParams(vector<string> parameters);



private:
    //vars
    TiffRead* tiffRead;
    TiffStat* tiffStat;

    uint x0;
    uint x1;
    uint y0;
    uint y1;

    uint ifdOff;
    uint valueOffset;
    uint curr;

    //methods
    void writeHeader(ofstream* imfile);
    void writeImage(ofstream* imfile);
    void writeTags(ofstream* imfile);
    void writeTag(ofstream *imfile, short tagNum, short type, uint numVals, uint Val);
    void writeTag(ofstream *imfile, short tagNum, short type, uint numVals, short Val) ;
    void writeTag(ofstream *imfile, short tagNum, short type, uint numVals, vector<short> Val) ;
    void writeTag(ofstream *imfile, short tagNum, short type, uint numVals, vector<uint> Val) ;

};


#endif //CLI_TIFFWRITE_H
