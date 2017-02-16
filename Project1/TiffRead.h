//
// Created by turner on 2/15/17.
//

#ifndef CLI_TIFFREAD_H
#define CLI_TIFFREAD_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <cstring>

#include "Module.h"
#include "Dispatcher.h"


using namespace std;

class TiffRead : public Module {

public:
    //methods
    void doCommand(vector<string>);
    bool parseParams(vector<string> parameters);

    //vars
    string filename;
    TiffStat* tiffStat;
    unsigned long pi;
    bool supported;

private:
    //methods
    bool checkTiffStat();
    void readFile();
    bool checkSupport();
    bool getDimensions();
    void loadArray();

    //vars
    unsigned long columns;
    unsigned long rows;
    unsigned long rps;
    unsigned long compression;
    vector<long> bps;
    vector<long> stripOffsets;
};


#endif //CLI_TIFFREAD_H
