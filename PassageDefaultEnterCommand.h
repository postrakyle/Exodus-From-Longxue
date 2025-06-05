//PassageDefualtEnterCommand.h

#ifndef ZOORK_PASSAGEDEFAULTENTERCOMMAND_H
#define ZOORK_PASSAGEDEFAULTENTERCOMMAND_H


#include "Command.h"

#include <utility>

class PassageDefaultEnterCommand : public Command {
public:
    explicit PassageDefaultEnterCommand(GameObject* g) : Command(g) {}

    void execute() override;
};


#endif //ZOORK_PASSAGEDEFAULTENTERCOMMAND_H
