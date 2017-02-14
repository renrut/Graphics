//
// Created by turner on 2/13/17.
//

#include "TiffStat.h"
using namespace std;




//pulls filename from the command
bool TiffStat::parseParams(vector<string> params){
        if(params.size() > 1){
            cout<<params[1]<<endl;
            try {
                filename = params[1];
            }catch (std::invalid_argument){
                std::cout<< "Invalid Parameter."<<std::endl;
                return false;
            }
        }
        return true;
}

//from module
void TiffStat::doCommand(vector<string> command){
    if(parseParams(command)){
        handleFile();
    }
}

//reads in the file
void TiffStat::handleFile() {

    char buffer[3];
    short magicno; // 2 byte quantity
    int ifdaddress; // 4 byte quantity
    short ifdcount;
    // 2 byte quantity
    ifstream imfile;
    imfile.open (filename, ios::binary );
    imfile.seekg (0, ios::beg); //not strictly necessary, but a good idea
    imfile.read (buffer,2);
    imfile.read ((char *)&magicno,2); //MAKE SURE YOU UNDERSTAND WHY THIS WORKS!
    imfile.read ((char *)&ifdaddress,4);
    imfile.seekg(ifdaddress,ios::beg);
    imfile.read((char *)&ifdcount,2);

    buffer[2]='\0'; //Necessary because buffer is a C-style string


    cout << "Endian: " << buffer << "\n";
    cout << "Magic: " << magicno << "\n";
    cout << "IFD Address: " << ifdaddress << "\n";
    cout<< "IFD Count: " << ifdcount << "\n";


    cout<<"\nTAGS"<<endl;
    for(size_t i = 0; i < ifdcount; ++i){
        readTag(&imfile);
    }

    imfile.close();


}

//struct TiffTag {
//    //read ifd tags
//    short tag; //2 byte
//    short fieldType;
//    unsigned long typeCount;
//    unsigned long valueOffset;
//    vector<long> values;
//};

TiffTag* TiffStat::readTag(ifstream* imfile){
    TiffTag * t = new TiffTag();
    imfile->read((char *)&(t->tag), 2);
    imfile->read((char *)&(t->fieldType), 2);
    imfile->read((char *)&(t->typeCount), 4);
    //here, we need to check if we're getting the value or getting the offset.
    if(isOffset(t->fieldType, t->typeCount)){
        imfile->read((char *)& (t->valueOffset), 4);
    }else{
        t->valueOffset = 0;
    }
    getTagValues(t, imfile);
    printTag(t);
    return t;
}

//returns whether or not the value is offset
bool TiffStat::isOffset(short type, unsigned long count){
    return !((count == 1 && type <= 3) || (count == 2 && type<=2));
}

//pushes values of tag into an array of longs in the tifftag struct
void TiffStat::getTagValues(TiffTag* t, ifstream* imfile){
    //save current location
    ios::seekdir cur = ios::cur;
    //skip to offset. if none, offset is 0
    imfile->seekg(t->valueOffset, ios::cur);
    //for each value
    for(int i = 0; i < t->typeCount; ++i){
        long val;
        imfile->read((char*)&val, typesize[t->fieldType]);
        t->values.push_back(val);
    }

    //return to after the value
    imfile->seekg(t->fieldType*t->typeCount, cur);
}

//prints the tag
void TiffStat::printTag(TiffTag *tag){
    cout<<tag->tag;
    for(int i = 0; i < tag->values.size(); ++i){
        cout<<" val: " << tag->values.at(i);
    }
    if(tag->values.size() == 0){
        cout<<" Offset";
    }
    cout<<endl;
}

//swaps the bits of a short
void TiffStat::SwapShortBytes(unsigned short int * twobytes)
{
    unsigned char * tmp = (unsigned char *)twobytes;
    unsigned int i;
    i = tmp[1];
    tmp[1] = tmp[0];
    tmp[0] = i;
    return;
}