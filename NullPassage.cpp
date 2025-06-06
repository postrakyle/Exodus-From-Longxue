#include "NullPassage.h"
#include "NullCommand.h"
#include "Room.h"    // assuming your Room class is declared here

NullPassage::NullPassage(Room* owner)
    : Passage(
        owner->getName(),
        owner->getDescription(),
        std::make_shared<NullCommand>(owner),
        owner,   // “from” room
        owner    // “to” room (same, since it does nothing)
    )
{
    // Nothing else to do
}
