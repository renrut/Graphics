//
// Created by turner on 1/19/17.
//

#ifndef CLI_COLORMODULE_H
#define CLI_COLORMODULE_H

#include "Module.h"
#include <vector>

class ColorModule: public Module {
public:
    bool parseParams(std::vector<std::string> parameters);
    void doCommand(std::vector<std::string>);
private:
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
};


#endif //CLI_COLORMODULE_H
