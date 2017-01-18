//
// Created by Turner Strayhorn on 1/18/17.
//

#ifndef CLI_MODULE_H
#define CLI_MODULE_H


class Module {

public:
    virtual void parseParams(std::string parameters);
    virtual void doCommand();


};


#endif //CLI_MODULE_H
