#ifndef NULLPASSAGE_H
#define NULLPASSAGE_H

#include "Passage.h"

// Forward‐declare Room so we can use Room* without a circular include.
class Room;

// A “null” passage that does nothing when entered.
// Its constructor must take exactly one Room* (the owner).
class NullPassage : public Passage {
public:
    explicit NullPassage(Room* owner);
    // No explicit destructor needed—compiler will generate it.
};

#endif // NULLPASSAGE_H
