//
// Created by turner on 1/19/17.
//

#include "ColorModule.h"

//parses any parameters. Decides if can run.
bool ColorModule::parseParams(std::vector<std::string> params){
    if(params.size() > 1){
        x = std::stof(params[1]);
    }
    if(params.size() > 2){
        y = std::stof(params[2]);

    }
    if(params.size() > 3){
        z = std::stof(params[3]);
    }
    if(params.size() > 4){
        return false;
    }
    return  true;
}

void ColorModule::doCommand(std::vector<std::string> command){
    if(parseParams(command)){
        std::cout<<"Color "<<x<<","<<y<<","<<z<<std::endl;
    }else{
        std::cout<<"Too many Parameters. Cannot run."<<std::endl;
    }
}

