//NullRoom.cpp

#include "NullCommand.h"
#include "NullRoom.h"

NullRoom::NullRoom() : Room("Nowhere", "This is a nonplace.", std::make_shared<NullCommand>()) {}