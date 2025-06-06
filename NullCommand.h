//NullCommand.h
#ifndef NULLCOMMAND_H
#define NULLCOMMAND_H

#include "Command.h"
#include <iostream>

class NullCommand : public Command {
public:
    // Must forward a GameObject* up to the base class:
    explicit NullCommand(GameObject* g) : Command(g) {}
    ~NullCommand() override = default;

    // Only declare execute() here (no inline body)
    void execute() override;
};

#endif // NULLCOMMAND_H
