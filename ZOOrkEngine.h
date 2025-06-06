//ZOOrkEngine.h
#ifndef ZOORKENGINE_H
#define ZOORKENGINE_H

#include "Room.h"
#include "Player.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

class ZOOrkEngine {
public:
    explicit ZOOrkEngine(std::shared_ptr<Room> start);
    void setRoomMap(const std::map<std::string, std::shared_ptr<Room>>& m);
    void run();

private:
    // Note: all handler methods now take 'const std::vector<std::string>&' to match the .cpp
    void handleGoCommand(const std::vector<std::string>&);
    void handleLookCommand(const std::vector<std::string>&);
    void handleSearchCommand(const std::vector<std::string>&);
    void handleTakeCommand(const std::vector<std::string>&);
    void handleDropCommand(const std::vector<std::string>&);
    void handleInventoryCommand();
    void handleHelpCommand();
    void handleQuitCommand(const std::vector<std::string>&);

    std::vector<std::string> tokenizeString(const std::string&);
    std::string makeLowercase(std::string);

    Player* player;
    std::map<std::string, std::shared_ptr<Room>> roomMap;
    bool gameOver = false;
    bool firstArrivalToZoo = true;
};

#endif // ZOORKENGINE_H
