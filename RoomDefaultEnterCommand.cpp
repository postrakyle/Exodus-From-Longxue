// --- RoomDefaultEnterCommand.cpp ---
#include "RoomDefaultEnterCommand.h"
#include <iostream>

void RoomDefaultEnterCommand::execute() {
    std::cout << gameObject->getDescription() << "\n";
}
