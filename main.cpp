//main.cpp
#include "WorldManager.h"
#include "ZOOrkEngine.h"

int main() {
    WorldManager world;
    std::shared_ptr<Room> start = world.getStartingRoom();
    ZOOrkEngine zoork(start);
    zoork.setRoomMap(world.getAllRooms());
    zoork.run();
    return 0;
}
