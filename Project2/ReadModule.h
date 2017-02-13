//
// Created by turner on 1/22/17.
//

#ifndef CLI_READMODULE_H
#define CLI_READMODULE_H

#include <string>
#include <fstream>
#include "Dispatcher.h"

class ReadModule: Module {
public:
    bool parseParams(std::vector<std::string> parameters);
    void doCommand(std::vector<std::string>);
private:
    std::string fileName;
};


#endif //CLI_READMODULE_H
