// --- RoomDefaultEnterCommand.h ---
#ifndef ZOORK_ROOMDEFAULTENTERCOMMAND_H
#define ZOORK_ROOMDEFAULTENTERCOMMAND_H

#include "Command.h"

class RoomDefaultEnterCommand : public Command {
public:
    explicit RoomDefaultEnterCommand(GameObject* g) : Command(g) {}
    void execute() override;
};

#endif //ZOORK_ROOMDEFAULTENTERCOMMAND_H
