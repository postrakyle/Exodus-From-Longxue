#ifndef ZOORK_ZOORKENGINE_H
#define ZOORK_ZOORKENGINE_H

#include "Player.h"
#include "Room.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>

class ZOOrkEngine {
public:
    // Construct with the starting room
    ZOOrkEngine(std::shared_ptr<Room> start);

    // Optionally give the engine a map of all rooms
    void setRoomMap(const std::map<std::string, std::shared_ptr<Room>>& m);

    // Main game loop
    void run();

private:
    bool gameOver = false;
    Player *player;
    std::map<std::string, std::shared_ptr<Room>> roomMap;

    // Command handlers
    void handleGoCommand(const std::vector<std::string>&);
    void handleLookCommand(const std::vector<std::string>&);
    void handleSearchCommand(const std::vector<std::string>&);
    void handleTakeCommand(const std::vector<std::string>&);
    void handleDropCommand(const std::vector<std::string>&);
    void handleInventoryCommand();
    void handleHelpCommand();
    void handleQuitCommand(const std::vector<std::string>&);

    // Helpers for parsing input
    static std::vector<std::string> tokenizeString(const std::string&);
    static std::string makeLowercase(std::string);
};

#endif // ZOORK_ZOORKENGINE_H
