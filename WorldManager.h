//WorldManager.cpp
#ifndef ZOORK_WORLDMANAGER_H
#define ZOORK_WORLDMANAGER_H

#include "Room.h"
#include <map>
#include <memory>
#include <string>

class WorldManager {
public:
    // Constructor: builds rooms and links them
    WorldManager();

    // Return the entry point (starting room)
    std::shared_ptr<Room> getStartingRoom() const;

    // Return map of all rooms by name
    const std::map<std::string, std::shared_ptr<Room>>& getAllRooms() const;

private:
    // Create each room and add lore/look/search entries
    void createRooms();

    // Connect rooms by passages
    void connectRooms();

    // All rooms, keyed by their name string
    std::map<std::string, std::shared_ptr<Room>> rooms;
};

#endif // ZOORK_WORLDMANAGER_H
