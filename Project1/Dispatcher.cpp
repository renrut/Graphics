//
// Created by turner on 1/18/17.
//

#include "Dispatcher.h"


Dispatcher* Dispatcher::dispatcher = nullptr;

Dispatcher::Dispatcher(){
    drawModule = new DrawModule();
    moveModule = new MoveModule();
    colorModule = new ColorModule();
    readModule = new ReadModule();
}

Dispatcher::~Dispatcher(){
    delete drawModule;
    delete moveModule;
    delete colorModule;
    delete readModule;
}

Dispatcher* Dispatcher::getReference(){
    if(!dispatcher) {
        dispatcher = new Dispatcher();
    }
    return dispatcher;
}

void Dispatcher::runCommand(std::string input){
    std::vector<std::string> splitCommand = parseCommand(input);
    std::string command = splitCommand[0];
    //to lower
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    if(command.compare("move") == 0){
        moveModule->doCommand(splitCommand);
    }else if(command.compare("draw") == 0){
        drawModule->doCommand(splitCommand);
    }else if(command.compare("color") == 0){
        colorModule->doCommand(splitCommand);
    }else if(command.compare("read") == 0){
        readModule->doCommand(splitCommand);
    }else{
        std::cout<<"Invalid Command"<<std::endl;
    }

}

std::vector<std::string> Dispatcher::parseCommand(std::string input){
    std::vector<std::string> command;
    std::istringstream f(input);
    std::string s;
    while (getline(f, s, ' ')) {
        command.push_back(s);
    }
    return command;
}
