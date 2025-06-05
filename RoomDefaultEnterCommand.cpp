//RoomDefualtEnterCommand.cpp


#include "RoomDefaultEnterCommand.h"

void RoomDefaultEnterCommand::execute() {
    std::cout << gameObject->getDescription() << "\n";
}