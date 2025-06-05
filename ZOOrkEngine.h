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
    ZOOrkEngine(std::shared_ptr<Room> start);
    void run();

    // Pass the global map of rooms
    void setRoomMap(const std::map<std::string, std::shared_ptr<Room>>& m);

private:
    bool gameOver = false;
    Player *player;
    std::map<std::string, std::shared_ptr<Room>> roomMap;

    void handleGoCommand(const std::vector<std::string>&);
    void handleLookCommand(const std::vector<std::string>&);
    void handleSearchCommand(const std::vector<std::string>&);
    void handleTakeCommand(const std::vector<std::string>&);
    void handleDropCommand(const std::vector<std::string>&);
    void handleQuitCommand(const std::vector<std::string>&);

    static std::vector<std::string> tokenizeString(const std::string&);
    static std::string makeLowercase(std::string);
};

#endif //ZOORK_ZOORKENGINE_H
