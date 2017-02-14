//
// Created by turner on 1/19/17.
//

#include "MoveModule.h"
#include <iterator>

//parses any parameters. Decides if can run.
bool MoveModule::parseParams(std::vector<std::string> params){
    if(params.size() > 1){
        try {
            x = std::stof(params[1]);
        }catch (std::invalid_argument){
            std::cout<< "Invalid Parameter."<<std::endl;
            return false;
        }

    }
    if(params.size() > 2){
        try {
            y = std::stof(params[2]);
        }catch (std::invalid_argument){
            std::cout<< "Invalid Parameter."<<std::endl;
            return false;
        }
    }
    if(params.size() > 3){
        try {
            z = std::stof(params[3]);
        }catch (std::invalid_argument){
            std::cout<< "Invalid Parameter."<<std::endl;
            return false;
        }    }
    if(params.size() > 4){
        return false;
    }
    return  true;
}

void MoveModule::doCommand(std::vector<std::string> command){
    if(parseParams(command)){
        std::cout<<"Move "<<x<<","<<y<<","<<z<<std::endl;
    }else{
        std::cout<< "Please enter up to 3 numeral coordinates."<<std::endl;
    }
}

