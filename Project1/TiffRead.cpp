//
// Created by turner on 2/15/17.
//

#include "TiffRead.h"
#include <GL/glut.h>
#define	checkImageWidth 1024
#define	checkImageHeight 1024
extern GLubyte checkImage[checkImageHeight][checkImageWidth][3];
extern void display();
extern void makeCheckImage();

using namespace std;





//pulls filename from the command
bool TiffRead::parseParams(vector<string> params){
    if(params.size() > 1){
        try {
            filename = params[1];
        }catch (std::invalid_argument){
            cout<< "Invalid File."<<endl;
            return false;
        }
    }
    return true;
}

//Makes sure tiffstat has been run on the file.
bool TiffRead::checkTiffStat(){
    return (tiffStat->filename == filename);
}

//from module
void TiffRead::doCommand(vector<string> command){
    cout<<"\n";
    if(parseParams(command)){
        tiffStat = Dispatcher::getReference()->tiffStat;
        if(checkTiffStat()) {
            makeCheckImage();
            readFile();
        }else{
            cout<<"Please run 'tiffstat "<< filename << "' before running tiffread"<<endl;
            return;
        }
    }
}

//returns false if image is not 8-bit grayscale or 24bit rgb
bool TiffRead::checkSupport(){
    try {
        bps = tiffStat->tags[258]->values;
        pi = tiffStat->tags[262]->values.at(0);
        compression = tiffStat->tags[259]->values.at(0);
    }catch (exception e){
        cout<<"Bits per strip, Photometric, or Compression not found."<<endl;
        return false;
    }
    if(compression != 1 ){
        cout<<"Compression not supported."<<endl;
        return false;
    }if(tiffStat->tags[284]->values.at(0) != 1){
        cout<<"Planar Configuration not supported."<<endl;
        return false;
    }

    if (! (((pi == 0 || pi == 1) && bps.size()==1 && bps.at(0) == 8) || (pi==2 && bps.size() == 3 && bps.at(0) == 8 && bps.at(1) == 8 && bps.at(2) == 8))){
        cout << "Image is not 8-bit Grayscale or 24-bit RGB" <<endl;
        return false;
    }
    return true;
}

//get rows and columns
bool TiffRead::getDimensions(){
    try {
        columns = tiffStat->tags[256]->values.at(0);
        rows = tiffStat->tags[257]->values.at(0);
        rps = tiffStat->tags[278]->values.at(0);
        stripOffsets = tiffStat->tags[273]->values;
    }catch (exception e){
        cout<<"ImageTag doesn't Contain Values for Dimensions or StripOffsets"<<endl;
        return false;
    }
    if(columns > checkImageWidth || rows > checkImageHeight){
        cout<<"Image is too large to display. Must be "<<checkImageHeight<<"x"<<checkImageWidth<<endl;
    }
    return (columns < checkImageWidth && rows < checkImageHeight);
}

void TiffRead::loadArray(){
    ifstream imfile;
    imfile.open (filename, ios::binary );
    char R;
    char G;
    char B;

    for(size_t i = 0; i < stripOffsets.size(); ++i){
        imfile.seekg(stripOffsets.at(i));
        //for each row
        for(size_t r = 0; r < rps && rps*i + r < rows; ++r){
            for(size_t c=0; c < columns; ++c){

                imfile.read(&R, 1);
                if(pi > 1) {
                    //RGB
                    imfile.read(&G, 1);
                    imfile.read(&B, 1);
                } else {
                    //it's greyscale. Set G & B equal to R
                    G = B = R;
                }
                //height to place pixel
                size_t hp = rows - ((i*rps)+r + 1);

                checkImage[hp][c][0] = R;
                checkImage[hp][c][1] = G;
                checkImage[hp][c][2] = B;

            }
        }
    }
}

//reads
void TiffRead::readFile(){
    if(!checkSupport()){
        cout<<"Cannot use Tiffread on this image."<<endl;
        return;
    }
    if(!getDimensions()){
        cout<<"Image not Supported"<<endl;
        return;
    }

    loadArray();
    display();
    return;
}