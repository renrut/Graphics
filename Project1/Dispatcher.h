//
// Created by turner on 1/18/17.
//

#ifndef CLI_DISPATCHER_H
#define CLI_DISPATCHER_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "MoveModule.h"
#include "DrawModule.h"
#include "ColorModule.h"
#include "ReadModule.h"

class ReadModule;

class Dispatcher {
    public:
        //runs the command
        static Dispatcher* getReference();
        void runCommand(std::string input);

    private:
        //returns command string
        static Dispatcher* dispatcher;
        std::vector<std::string> parseCommand(std::string input);
        DrawModule* drawModule;
        MoveModule* moveModule;
        ColorModule* colorModule;
        ReadModule* readModule;

};


#endif //CLI_DISPATCHER_H
