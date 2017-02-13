//
// Created by Turner Strayhorn on 1/18/17.
//

#ifndef CLI_MODULE_H
#define CLI_MODULE_H

#include <vector>
#include <string>
#include <iostream>

class Module {

public:
    virtual ~Module(){};
    //parses parameters into private vars. Decides if can run.
    virtual bool parseParams(std::vector<std::string> parameters) = 0;
    virtual void doCommand(std::vector<std::string> command) = 0;


};


#endif //CLI_MODULE_H
