#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include "Room.h"
#include <map>
#include <memory>
#include <string>

class WorldManager {
public:
    WorldManager();
    std::shared_ptr<Room> getStartingRoom() const;
    const std::map<std::string, std::shared_ptr<Room>>& getAllRooms() const;
private:
    std::map<std::string, std::shared_ptr<Room>> rooms;
    void createRooms();
    void connectRooms();
};

#endif
