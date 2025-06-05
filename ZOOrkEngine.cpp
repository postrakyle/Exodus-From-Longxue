#include "ZOOrkEngine.h"
#include <algorithm>
#include <utility>
#include <sstream>
#include <limits>
#include "Passage.h"

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    player = Player::instance();
    player->setCurrentRoom(start.get());
    player->getCurrentRoom()->enter();
}

// This function allows passing the room map into the engine
void ZOOrkEngine::setRoomMap(const std::map<std::string, std::shared_ptr<Room>>& m) {
    roomMap = m;
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);
        std::vector<std::string> words = tokenizeString(input);
        if (words.empty()) continue;
        std::string command = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (command == "go") {
            handleGoCommand(arguments);
        } else if ((command == "look") || (command == "inspect")) {
            handleLookCommand(arguments);
        } else if ((command == "take") || (command == "get")) {
            handleTakeCommand(arguments);
        } else if (command == "drop") {
            handleDropCommand(arguments);
        } else if (command == "quit") {
            handleQuitCommand(arguments);
        } else {
            std::cout << "I don't understand that command.\n";
        }
    }
}

// Supports "go direction" OR "go room name" (even with multi-word names)
void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) return;

    // Join multi-word arguments for room names
    std::string arg;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) arg += " ";
        arg += arguments[i];
    }

    Room* currentRoom = player->getCurrentRoom();

    // 1. Try as a direction
    auto passage = currentRoom->getPassage(arg);
    if (passage && passage->getTo() != currentRoom) {
        player->setCurrentRoom(passage->getTo());
        passage->enter();
        return;
    }

    // 2. Try as a room name (matches any *adjacent* room)
    for (const auto& kv : currentRoom->getAllExits()) {
        Room* dest = kv.second->getTo();
        if (makeLowercase(dest->getName()) == makeLowercase(arg)) {
            player->setCurrentRoom(dest);
            kv.second->enter();
            return;
        }
    }

    std::cout << "You can't go there from here.\n";
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string>) {
    std::cout << "This functionality is not yet enabled.\n";
}

void ZOOrkEngine::handleTakeCommand(std::vector<std::string>) {
    std::cout << "This functionality is not yet enabled.\n";
}

void ZOOrkEngine::handleDropCommand(std::vector<std::string>) {
    std::cout << "This functionality is not yet enabled.\n";
}

void ZOOrkEngine::handleQuitCommand(std::vector<std::string>) {
    std::string input;
    std::cout << "Are you sure you want to QUIT?\n> ";
    std::cin >> input;
    std::string quitStr = makeLowercase(input);

    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::vector<std::string> ZOOrkEngine::tokenizeString(const std::string &input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        tokens.push_back(makeLowercase(token));
    }
    return tokens;
}

std::string ZOOrkEngine::makeLowercase(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}
