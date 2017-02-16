//
// Created by turner on 1/18/17.
//

#include "Dispatcher.h"


Dispatcher* Dispatcher::dispatcher = nullptr;



Dispatcher* Dispatcher::getReference(){
    if(!dispatcher) {
        dispatcher = new Dispatcher();
    }
    return dispatcher;
}

void Dispatcher::runCommand(std::string input){
    if(input != "") {

        std::vector <std::string> splitCommand = parseCommand(input);
        std::string command = splitCommand[0];
        //to lower
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command.compare("move") == 0) {
            moveModule = new MoveModule();
            moveModule->doCommand(splitCommand);
            delete moveModule;
            moveModule = nullptr;
        } else if (command.compare("draw") == 0) {
            drawModule = new DrawModule();
            drawModule->doCommand(splitCommand);
            delete drawModule;
            drawModule = nullptr;
        } else if (command.compare("color") == 0) {
            colorModule = new ColorModule();
            colorModule->doCommand(splitCommand);
            delete colorModule;
            colorModule = nullptr;
        } else if (command.compare("read") == 0) {
            readModule = new ReadModule();
            readModule->doCommand(splitCommand);
            delete readModule;
            readModule = nullptr;
        }else if (command.compare("tiffstat") == 0) {
            tiffStat = new TiffStat();
            tiffStat->doCommand(splitCommand);
        }else if (command.compare("tiffread") == 0) {
            if(tiffStat == nullptr){
                tiffStat = new TiffStat();
            }
            tiffStat->doCommand(splitCommand);
            tiffRead = new TiffRead();
            tiffRead->doCommand(splitCommand);

        }else if (command.compare("tiffwrite") == 0) {
            if(tiffRead == nullptr){
                cout<<"Please Run Tiffread on the Image First"<<endl;
            }else{
                tiffWrite = new TiffWrite();
                tiffWrite->doCommand(splitCommand);
            }
        } else {
            std::cout << "Invalid Command" << std::endl;
        }
    }
}

std::vector<std::string> Dispatcher::parseCommand(std::string input){
    std::vector<std::string> command;
    //add space to end for split
    std::istringstream f(input + " ");
    std::string s;
    while (getline(f, s, ' ')) {
        command.push_back(s);
    }
    return command;
}
