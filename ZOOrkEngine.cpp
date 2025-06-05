#include "ZOOrkEngine.h"
#include "Passage.h"
#include "Room.h"
#include <algorithm>
#include <sstream>
#include <limits>
#include <iostream>

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    player = Player::instance();
    player->setCurrentRoom(start.get());

    // Show the room’s description and list what you can either move to or inspect
    start->enter();

    std::cout << "You notice:\n";
    Room* cur = player->getCurrentRoom();
    for (const auto& kv : cur->getAllExits()) {
        std::cout << "  - " << kv.second->getTo()->getName() << "\n";
    }

    std::cout << "You can inspect:\n";
    for (const auto& objName : cur->getLookableNames()) {
        std::cout << "  - " << objName << "\n";
    }
}

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

        if (command == "go" || command == "goto" || command == "move") {
            handleGoCommand(arguments);
        }
        else if ((command == "look") || (command == "inspect")) {
            handleLookCommand(arguments);
        }
        else if (command == "search") {
            handleSearchCommand(arguments);
        }
        else if ((command == "take") || (command == "get")) {
            handleTakeCommand(arguments);
        }
        else if (command == "drop") {
            handleDropCommand(arguments);
        }
        else if (command == "quit") {
            handleQuitCommand(arguments);
        }
        else {
            // If they just type an object or room name, try look as a fallback
            handleLookCommand(words);
        }
    }
}

void ZOOrkEngine::handleGoCommand(const std::vector<std::string>& arguments) {
    if (arguments.empty()) return;

    // Combine multi-word target (e.g., "red car", "Subway Station")
    std::string target;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) target += " ";
        target += arguments[i];
    }

    Room* currentRoom = player->getCurrentRoom();
    std::string targetLower = makeLowercase(target);

    // 1) Check exits by destination name
    for (const auto& kv : currentRoom->getAllExits()) {
        Room* dest = kv.second->getTo();
        std::string destNameLower = makeLowercase(dest->getName());

        if (destNameLower == targetLower) {
            // Move player
            player->setCurrentRoom(dest);
            dest->enter();

            // Show next possible moves
            std::cout << "You notice:\n";
            for (const auto& exitPair : dest->getAllExits()) {
                std::cout << "  - " << exitPair.second->getTo()->getName() << "\n";
            }

            std::cout << "You can inspect:\n";
            for (const auto& objName : dest->getLookableNames()) {
                std::cout << "  - " << objName << "\n";
            }
            return;
        }
    }

    std::cout << "You can't go to \"" << target << "\" from here.\n";
}

void ZOOrkEngine::handleLookCommand(const std::vector<std::string>& arguments) {
    Room* currentRoom = player->getCurrentRoom();

    if (arguments.empty()) {
        // Just "look" → show room description + what you can inspect
        std::cout << currentRoom->getDescription() << "\n";

        std::cout << "You notice:\n";
        for (const auto& kv : currentRoom->getAllExits()) {
            std::cout << "  - " << kv.second->getTo()->getName() << "\n";
        }

        std::cout << "You can inspect:\n";
        for (const auto& objName : currentRoom->getLookableNames()) {
            std::cout << "  - " << objName << "\n";
        }
    }
    else {
        // "look <object>"
        std::string target;
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (i > 0) target += " ";
            target += arguments[i];
        }

        if (currentRoom->isLookable(target)) {
            std::cout << currentRoom->getLookDescription(target) << "\n";
        } else {
            std::cout << "There's no \"" << target << "\" to look at here.\n";
        }
    }
}

void ZOOrkEngine::handleSearchCommand(const std::vector<std::string>& arguments) {
    if (arguments.empty()) {
        std::cout << "Search what?\n";
        return;
    }

    Room* currentRoom = player->getCurrentRoom();
    std::string target;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) target += " ";
        target += arguments[i];
    }

    if (currentRoom->isSearchable(target)) {
        std::cout << currentRoom->getSearchDescription(target) << "\n";
    } else {
        std::cout << "You find nothing interesting when searching \"" << target << "\".\n";
    }
}

void ZOOrkEngine::handleTakeCommand(const std::vector<std::string>&) {
    std::cout << "This functionality is not yet enabled.\n";
}

void ZOOrkEngine::handleDropCommand(const std::vector<std::string>&) {
    std::cout << "This functionality is not yet enabled.\n";
}

void ZOOrkEngine::handleQuitCommand(const std::vector<std::string>&) {
    std::string input;
    std::cout << "Are you sure you want to QUIT? (y/n)\n> ";
    std::cin >> input;
    std::string quitStr = makeLowercase(input);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
    }
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
