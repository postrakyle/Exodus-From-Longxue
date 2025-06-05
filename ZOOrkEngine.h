#ifndef ZOORK_ZOORKENGINE_H
#define ZOORK_ZOORKENGINE_H

#include "Player.h"
#include "Room.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>

class ZOOrkEngine {
public:
    ZOOrkEngine(std::shared_ptr<Room>);
    void run();

    // Allows main() or WorldManager to set the room map
    void setRoomMap(const std::map<std::string, std::shared_ptr<Room>>& m);

private:
    bool gameOver = false;
    Player *player;
    std::map<std::string, std::shared_ptr<Room>> roomMap;

    void handleGoCommand(std::vector<std::string>);
    void handleLookCommand(std::vector<std::string>);
    void handleTakeCommand(std::vector<std::string>);
    void handleDropCommand(std::vector<std::string>);
    void handleQuitCommand(std::vector<std::string>);

    static std::vector<std::string> tokenizeString(const std::string&);
    static std::string makeLowercase(std::string);
};

#endif //ZOORK_ZOORKENGINE_H
