//
// Created by turner on 2/15/17.
//

#include "TiffWrite.h"
#include <GL/glut.h>
#define	checkImageWidth 1024
#define	checkImageHeight 1024
extern GLubyte checkImage[checkImageHeight][checkImageWidth][3];

using namespace std;



//pulls filename from the command
bool TiffWrite::parseParams(vector<string> params){
    if(params.size() == 6){
        try {
            filename = params[1];
            x0 = stoul(params[2]);
            y0 = stoul(params[3]);
            x1 = stoul(params[4]);
            y1 = stoul(params[5]);
        }catch (std::invalid_argument){
            cout<< "Invalid File."<<endl;
            return false;
        }
    } else if(params.size() > 6) {
        cout<<"Too many arguments"<<endl;
        return  false;
    } else {
        cout<<"Not enough arguments"<<endl;
        return false;
    }

    if(x0 > x1 || y0 > y1){
        cout<<"Please pass parameters lower left x, y upper left x,y"<<endl;
        return false;
    }

    return true;
}


//from module
void TiffWrite::doCommand(vector<string> command){
    if(parseParams(command)){
        tiffRead = Dispatcher::getReference()->tiffRead;
        tiffStat = tiffRead->tiffStat;
        ofstream imfile;
        imfile.open(filename, ios::binary);
        cout<<"Header'"<<endl;
        writeHeader(&imfile);
        cout<<"Image"<<endl;
        writeImage(&imfile);
        cout<<"Tags"<<endl;
        writeTags(&imfile);
        imfile.write((char *)&"0000", 4);
        imfile.close();
    } else{
        cout<<"Example: 'tiffwrite mytif.tif 0 0 10 10'"<<endl;

    }
    return;
}

//write header of the file
void TiffWrite::writeHeader(ofstream* imfile){
    short magicno = 42;
    uint imgHeight = y1-y0;
    uint imgWidth = x1-x0;
    short samplesper = 0;
    if(tiffRead->pi < 2){
        samplesper = 1;
    }else{
        samplesper = 3;
    }
    uint imgSize = samplesper * imgHeight * imgWidth;
    ifdOff = 8 + imgSize;
    imfile->write ((char *)&"II", 2);
    imfile->write((char *)&magicno, 2);
    imfile->write((char *)&ifdOff, 4);
}

//write the actual img
void TiffWrite::writeImage(ofstream* imfile){
    for(size_t y=y0; y<y1; ++y){
        for(size_t x=x0; x<x1; ++x){
            if(tiffRead->pi == 2) {
                imfile->write((char *)checkImage[y1 - y - 1][x], 3);
            }else{
                imfile->write((char *)checkImage[y1 - y - 1][x], 1);
            }
        }
    }
}

void TiffWrite::writeTags(ofstream* imfile){
    imfile->seekp(ifdOff);
    //write
    short ifdcount = 12;
    //write ifdcount
    imfile->write ((char *)&ifdcount, 2);
    //Start tags
    //calculate where the values go
    valueOffset = 12 * ifdcount + imfile->tellp() + 4;
    curr = imfile->tellp();

    //256 im width
    uint imgWidth = x1-x0;
    writeTag(imfile, 256, 4, 1, imgWidth);
    //257 img height
    uint imgHeight = y1-y0;
    writeTag(imfile, 257, 4, 1, imgHeight);
    //258 BPS
    if(tiffRead->pi < 2){
        writeTag(imfile, 258, 3, 1, (short)8);
    }else{
        short a[] = {8,8,8};
        vector<short> bps(a, a + sizeof(a)/sizeof(*a));
        writeTag(imfile, 258, 3, 3, bps);
    }
    //259 Compression 1 for us
    writeTag(imfile, 259, 3, 1, (short)1);

    //262 0/1gs 2 rgb
    writeTag(imfile, 262, 3, 1, (short)tiffRead->pi);

    //273 strip offset need to figure out
    short offset = 8;
    writeTag(imfile, 273, 3, 1, offset);

    //277 spp 3
    short samplesper = 0;
    if(tiffRead->pi < 2){
        samplesper = 1;
    }else{
        samplesper = 3;
    }
    writeTag(imfile, 277, 3, 1, samplesper);

    //278 RPS
    writeTag(imfile, 278, 4, 1, imgHeight);

    //279 strip bytecounts
    uint bytes = samplesper * imgHeight * imgWidth;
    writeTag(imfile, 279, 4, 1, bytes);

    //282 XRes
    uint xres[] = {(uint)tiffStat->tags[282]->values.at(0),1};
    vector<uint> xResolution(xres, xres + sizeof(xres)/sizeof(*xres));
    writeTag(imfile, 282, 5, 1, xResolution);

    //283 YRes
    uint yres[] = {(uint)tiffStat->tags[283]->values.at(0),1};
    vector<uint> yResolution(yres, yres + sizeof(yres)/sizeof(*yres));
    writeTag(imfile, 282, 5, 1, yResolution);

    //296 resolution
    writeTag(imfile, 296, 3, 1, (short)tiffStat->tags[296]->values.at(0));

}

void TiffWrite::writeTag(ofstream *imfile, short tagNum, short type, uint numVals, uint val) {
    //look mom im delegating!
    cout<<tagNum<<" " << val<<endl;
    unsigned int a[] = {val};
    vector<unsigned int> b(a, a + sizeof(a)/sizeof(*a));
    writeTag(imfile, tagNum, type, numVals, b);
}
void TiffWrite::writeTag(ofstream *imfile, short tagNum, short type, uint numVals, short val) {
    //look mom im delegating!
    short a[] = {val};
    vector<short> b(a, a + sizeof(a)/sizeof(*a));
    writeTag(imfile, tagNum, type, numVals,b);
}

//may need to offset
void TiffWrite::writeTag(ofstream *imfile, short tagNum, short type, uint numVals, vector<short> val) {
    cout<<"Writing1"<<tagNum<<endl;
    imfile->write((char *) &tagNum, 2);
    imfile->write((char *) &type, 2);
    imfile->write((char *) &numVals, 4);
    if(val.size() == 1){
        imfile->write((char *) &val.at(0), 2);
        curr = imfile->tellp();
        curr+=2;
        imfile->seekp(curr);

    }else if(val.size() == 2){
        imfile->write((char *) &val.at(0), 2);
        imfile->write((char *) &val.at(1), 2);
    }else{
        cout<<"Offsetting: "<<valueOffset<<endl;
        //need to offset

        //write to offset
        imfile->write((char *) &valueOffset, 4);
        //save curr
        curr = imfile->tellp();
        //seek to offset
        imfile->seekp(valueOffset);
        //write vals

        for(size_t v=0; v<val.size(); ++v){
            imfile->write((char *)&val.at(v),2);
        }

        //save new offset
        valueOffset = imfile->tellp();

        //seek back to curr
        imfile->seekp(curr);

    }
}
//need to offset
void TiffWrite::writeTag(ofstream *imfile, short tagNum, short type, uint numVals, vector<uint> val) {
    cout<<"Writing2"<<tagNum<<endl;

    imfile->write((char *) &tagNum, 2);
    imfile->write((char *) &type, 2);
    imfile->write((char *) &numVals, 4);

    if(val.size() == 1){
        imfile->write((char *) &val.at(0), 4);
    }else{
        //need to offset
        //write to offset
        imfile->write((char *) &valueOffset, 4);
        //save curr
        curr = imfile->tellp();
        //seek to offset
        imfile->seekp(valueOffset);
        //write vals
        for(size_t v=0; v<val.size(); ++v){
            imfile->write((char *)&val.at(v),4);
        }
        //save new offset
        valueOffset = imfile->tellp();
        //seek back to curr
        imfile->seekp(curr);
    }
}
