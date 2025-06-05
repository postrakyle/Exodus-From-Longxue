//NullCommand.h

#ifndef ZOORK_NULLCOMMAND_H
#define ZOORK_NULLCOMMAND_H

#include "Command.h"
#include <iostream>

class NullCommand : public Command {
public:
    NullCommand() : Command(nullptr) {}

    void execute() override;
};


#endif //ZOORK_NULLCOMMAND_H
