//NullRoom.cpp
#include "NullRoom.h"
#include "NullCommand.h"

NullRoom::NullRoom()
  : Room("Nowhere", "This is a nonplace.", std::make_shared<NullCommand>(this))
{}
