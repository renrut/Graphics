//
// Created by turner on 2/13/17.
//

#include "TiffStat.h"
using namespace std;




//pulls filename from the command
bool TiffStat::parseParams(vector<string> params){
        if(params.size() > 1){
            try {
                filename = params[1];
            }catch (std::invalid_argument){
                std::cout<< "Invalid File."<<std::endl;
                return false;
            }
        }
        return true;
}

//from module
void TiffStat::doCommand(vector<string> command){
    cout<<"\n";
    if(parseParams(command)){
        handleFile();
    }
}

//reads in the file
void TiffStat::handleFile() {

    char buffer[3];
    short magicno; // 2 byte quantity
    uint ifdaddress; // 4 byte quantity
    short ifdcount;
    // 2 byte quantity
    ifstream imfile;
    imfile.open (filename, ios::binary );
    imfile.seekg (0, ios::beg); //not strictly necessary, but a good idea
    imfile.read (buffer,2);
    imfile.read ((char *)&magicno,2); //MAKE SURE YOU UNDERSTAND WHY THIS WORKS!
    imfile.read ((char *)&ifdaddress,4);

    buffer[2]='\0'; //Necessary because buffer is a C-style string
    //check magic number is 42
    if(!(magicno == 42 || magicno == 10752)){
        cout<<"Wrong Magic Number ("<<magicno<<") Not a Valid Tiff File"<<endl;
        filename = "";
        return;
    }
    cout<<filename<<endl;
    //check endian
    if(!strcmp(buffer, "II")){
        bigendian = false;
        cout << "Endian: <little-endian> "<<"\n";
    }else if(!strcmp(buffer, "MM")){
        bigendian = true;
        cout << "Endian: <big-endian> "<<"\n";
    }else{
        cout<<"Does not Begin with 'II' or 'MM'. Not a Valid Tiff File"<<endl;
        filename = "";
        return;
    }
    //swap vals if BE
    if(bigendian){
        SwapShortBytes((unsigned short int *)&magicno);
        SwapFourBytes((unsigned int *)&ifdaddress);
    }
    imfile.seekg(ifdaddress,ios::beg);
    imfile.read((char *)&ifdcount,2);

    if(bigendian){
        SwapShortBytes((unsigned short int *)&ifdcount);
    }
    cout << "Magic: " << magicno << "\n";
    cout << "IFD Address: " << ifdaddress << "\n";
    cout<< "IFD Count: " << ifdcount << "\n";

    for(short i = 0; i < ifdcount; ++i){
        TiffTag * t = readTag(&imfile);
        tags[t->tag] = t;
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

//Reads the tag from the IFD. Expects the ifstream* to be at the start of the tag
TiffTag* TiffStat::readTag(ifstream* imfile){
    TiffTag * t = new TiffTag();
    imfile->read((char *)&(t->tag), 2);
    imfile->read((char *)&(t->fieldType), 2);
    imfile->read((char *)&(t->typeCount), 4);


    //here, we need to check if we're getting the value or getting the offset.
    t->valueOffset = 0;
    //check for BE
    if(bigendian){
        SwapShortBytes((unsigned short int *)&(t->tag));
        SwapShortBytes((unsigned short int *)&(t->fieldType));
        SwapFourBytes((unsigned int *)&(t->typeCount));
    }

    if(isOffset(t->fieldType, t->typeCount)){
        imfile->read((char *)& (t->valueOffset), 4);
        if(bigendian){
            SwapFourBytes((unsigned int *)&(t->valueOffset));
        }
    }

    if(t->fieldType <= 5){
        //at this point, the imfile param is either reading before the last 4 bits or after
        getTagValues(t, imfile);
    }
    printTag(t);
    return t;
}

//returns whether or not the value is offset
bool TiffStat::isOffset(short type, unsigned long count){
    return !((count <= 4 && type <=2) || (count <=2 && type==3) || (count==1 && type == 4));
}

//pushes values of tag into an array of longs in the tifftag struct
void TiffStat::getTagValues(TiffTag* t, ifstream* imfile){
    //if offset
    streampos currentPos = imfile->tellg();
    if(t->valueOffset != 0){
        imfile->seekg(t->valueOffset);
    }

    //for each val in value count
    for(size_t i = 0; i < t-> typeCount; ++i){
        if(t->fieldType == 1 || t->fieldType == 2){
            char val;
            imfile->read((char *)&val, 1);
            t->values.emplace_back((long)val);
        }if(t->fieldType == 3){
            short val;
            imfile->read((char *)&val, 2);
            if(bigendian){
                SwapShortBytes((unsigned short int *)&val);
            }
            t->values.emplace_back((long)val);
        }if(t->fieldType == 4){
            uint val;
            imfile->read((char *)&val, 4);
            if(bigendian) {
                SwapFourBytes((unsigned int *) &val);
            }
            t->values.emplace_back((long)val);
        }if(t->fieldType == 5){
            uint val1;
            uint val2;
            imfile->read((char *)&val1, 4);
            imfile->read((char *)&val2, 4);
            if(bigendian) {
                SwapFourBytes((unsigned int *) &val1);
                SwapFourBytes((unsigned int *) &val2);
            }
            t->values.emplace_back(((long)(val1/val2)));
        }
    }
    //return to curr if offset/ return to beginning of next tag otherwise

    if(t->valueOffset != 0){
        imfile->seekg(currentPos);
    }else{
        currentPos += 4;
        imfile->seekg(currentPos);
    }
    return;
}

//prints the tag
//ie SubFileType (254) LONG (4) 1<0>
void TiffStat::printTag(TiffTag *tag){
    //tag
    cout<<tagMap[tag->tag]<<" ("<<tag->tag<<") "<<fieldTypeMap[tag->fieldType]<<" ("<<tag->fieldType<<") "<< tag->typeCount <<"<";
    //values. only prints 10
    for (size_t i = 0; i < min((int)tag->values.size(), 10); ++i){
        if(tag->fieldType == 2){
            cout<<(char)tag->values.at(i);
        }else{
            cout<<tag->values.at(i);
        }
        if(!(tag->fieldType == 2) && i+1 != tag->values.size()){
            cout<<" ";
        }
    }
    if(tag->values.size() > 10){
        cout<<"...";
    }
    cout<<">"<<endl;
}

//swaps the bits of a short
void TiffStat::SwapShortBytes(unsigned short int * twobytes)
{
    unsigned char * tmp = (unsigned char *)twobytes;
    unsigned char i;
    i = tmp[1];
    tmp[1] = tmp[0];
    tmp[0] = i;
    return;
}

void TiffStat::SwapFourBytes(unsigned int * fourbytes){
    unsigned char * tmp = (unsigned char *)fourbytes;
    unsigned char i;
    i = tmp[3];
    tmp[3] = tmp[0];
    tmp[0] = i;
    i = tmp [2];
    tmp[2] = tmp[1];
    tmp[1] = i;
    return;
}