//
// Created by turner on 1/22/17.
//

#include "ReadModule.h"
//parses any parameters. Decides if can run.
bool ReadModule::parseParams(std::vector<std::string> params){
    if(params.size() > 1){
        fileName = params[1];
    }
    if(params.size() > 2){
        return false;
    }
    return  true;
}

void ReadModule::doCommand(std::vector<std::string> command){
    if(parseParams(command)){
        Dispatcher* d = Dispatcher::getReference();
        std::ifstream file(fileName);
        std::string command;

        std::cout<<"Reading from " << fileName << std::endl;

        while (std::getline(file, command))
        {
            d->runCommand(command);
        }

    }else{
        std::cout<<"Reading from " << fileName << std::endl;
        std::cout<<"Too many Parameters. Cannot run."<<std::endl;
    }
}
