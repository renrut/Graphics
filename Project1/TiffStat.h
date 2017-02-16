//
// Created by turner on 2/13/17.
//

#ifndef CLI_TIFFSTAT_H
#define CLI_TIFFSTAT_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <cstring>

#include "Module.h"

using namespace std;

//template<typename T>
struct TiffTag {
    //read ifd tags
    short tag; //2 byte
    short fieldType;
    uint typeCount;
    uint valueOffset;
    vector<long> values;
};

class TiffStat: public Module {
public:
    //methods
    void doCommand(vector<string>);
    bool parseParams(vector<string> parameters);
    //vars
    map<short, TiffTag*> tags;
    string filename;

private:
    //methods
    void handleFile();
    void SwapShortBytes(unsigned short int * twobytes);
    void printTag(TiffTag *tag);
    TiffTag* readTag(ifstream *imfile);
    bool isOffset(short type, unsigned long count);
    void getTagValues(TiffTag* t, ifstream* imfile);
    void SwapFourBytes(unsigned int * fourbytes);

    //vars
    bool bigendian;
    char* buffer;
    int typesize[6] = {0,1,1,2,4,8};
    map<short, string> fieldTypeMap = {
            {1, "BYTE"}, {2, "ASCII"}, {3,"SHORT"}, {4, "LONG"}, {5, "RATIONAL"}
    };
    map<short, string> tagMap = {
            {254,"SubfileType"},
            {255,"SubfileType"},
            {256,"ImageWidth"},
            {257,"ImageLength"},
            {258,"BitsPerSample"},
            {259,"Compression"},
            {262,"PhotometricInterpretation"},
            {263,"Threshholding"},
            {264,"CellWidth"},
            {265,"CellLength"},
            {266,"FillOrder"},
            {269,"DocumentName"},
            {270,"ImageDescription"},
            {271,"Make"},
            {272,"Model"},
            {273,"StripOffsets"},
            {274,"Orientation"},
            {277,"SamplesPerPixel"},
            {278,"RowsPerStrip"},
            {279,"StripByteCounts"},
            {280,"MinSampleValue"},
            {281,"MaxSampleValue"},
            {282,"XResolution"},
            {283,"YResolution"},
            {284,"PlanarConfiguration"},
            {285,"PageName"},
            {286,"XPosition"},
            {287,"YPosition"},
            {288,"FreeOffsets"},
            {289,"FreeByteCounts"},
            {290,"GrayResponseUnit"},
            {291,"GrayResponseCurve"},
            {292,"T4Options"},
            {293,"T6Options"},
            {296,"ResolutionUnit"},
            {297,"PageNumber"},
            {301,"TransferFunction"},
            {305,"Software"},
            {306,"DateTime"},
            {315,"Artist"},
            {316,"HostComputer"},
            {317,"Predictor"},
            {318,"WhitePoint"},
            {319,"PrimaryChromaticities"},
            {320,"ColorMap"},
            {321,"HalftoneHints"},
            {322,"TileWidth"},
            {323,"TileLength"},
            {324,"TileOffsets"},
            {325,"TileByteCounts"},
            {332,"InkSet"},
            {333,"InkNames"},
            {334,"NumberOfInks"},
            {336,"DotRange"},
            {337,"TargetPrinter"},
            {338,"ExtraSamples"},
            {339,"SampleFormat"},
            {340,"SMinSampleValue"},
            {341,"SMaxSampleValue"},
            {342,"TransferRange"},
            {512,"JPEGProc"},
            {513,"JPEGInterchangeFormat"},
            {514,"JPEGInterchangeFormatLngth"},
            {515,"JPEGRestartInterval"},
            {517,"PEGLosslessPredictors"},
            {518,"JPEGPointTransforms"},
            {519,"JPEGQTables"},
            {520,"JPEGDCTables"},
            {521,"JPEGACTables"},
            {529,"YCbCrCoefficients"},
            {530,"YCbCrSubSampling"},
            {531,"YCbCrPositioning"},
            {532,"ReferenceBlackWhite"},
            {33432,"Copyright"}
    };

};



#endif //CLI_TIFFSTAT_H
