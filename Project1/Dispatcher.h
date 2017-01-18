//
// Created by turner on 1/18/17.
//

#ifndef CLI_DISPATCHER_H
#define CLI_DISPATCHER_H


class Dispatcher {
    public:
        //runs the command
        void runCommand(std::string);

    private:
        //returns command string
        std::string parseCommand(std::string);



};


#endif //CLI_DISPATCHER_H
