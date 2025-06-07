// File: ZOOrkEngine.h

#ifndef ZOORKENGINE_H
#define ZOORKENGINE_H

#include "Room.h"
#include "Player.h"
#include <map>
#include <string>
#include <vector>

class ZOOrkEngine {
public:
    explicit ZOOrkEngine(std::shared_ptr<Room> start);
    void setRoomMap(const std::map<std::string, std::shared_ptr<Room>>& m);
    void run();

private:
    void handleGoCommand(const std::vector<std::string>& arguments);
    void handleLookCommand(const std::vector<std::string>& arguments);
    void handleSearchCommand(const std::vector<std::string>& arguments);
    void handleTakeCommand(const std::vector<std::string>& arguments);
    void handleDropCommand(const std::vector<std::string>& arguments);
    void handleInventoryCommand();
    void handleHelpCommand();
    void handleQuitCommand(const std::vector<std::string>& arguments);

    std::vector<std::string> tokenizeString(const std::string& input);
    std::string makeLowercase(std::string input);

    std::map<std::string, std::shared_ptr<Room>> roomMap;
    Player* player = nullptr;
    bool gameOver = false;
    bool firstArrivalToZoo = true;
};

#endif // ZOORKENGINE_H
