// --- ZOOrkEngine.cpp ---
#include "ZOOrkEngine.h"
#include "Passage.h"
#include <algorithm>
#include <sstream>
#include <limits>
#include <iostream>

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    player = Player::instance();
    player->setCurrentRoom(start.get());
    // When the game starts, show the description and available locations
    start->enter();
    std::cout << "You can go to:\n";
    for (const auto& kv : start->getAllExits()) {
        std::cout << "  - " << kv.second->getTo()->getName() << "\n";
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
        } else if ((command == "look") || (command == "inspect")) {
            handleLookCommand(arguments);
        } else if ((command == "take") || (command == "get")) {
            handleTakeCommand(arguments);
        } else if (command == "drop") {
            handleDropCommand(arguments);
        } else if (command == "quit") {
            handleQuitCommand(arguments);
        } else {
            // If they just type a location name without "go"
            handleGoCommand(words);
        }
    }
}

void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) return;

    // Join multi-word arguments for room names
    std::string arg;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) arg += " ";
        arg += arguments[i];
    }

    Room* currentRoom = player->getCurrentRoom();

    // Try matching any exit by destination name or by exit label
    for (const auto& kv : currentRoom->getAllExits()) {
        std::string exitLabel = kv.first;                         // e.g. "to zoo"
        Room* dest = kv.second->getTo();                          // pointer to the Room

        // Normalize both to lowercase for comparison
        std::string lowerArg      = makeLowercase(arg);
        std::string lowerDestName = makeLowercase(dest->getName());
        std::string lowerExitLbl  = makeLowercase(exitLabel);

        // Matches if player typed "zoo", "to zoo", or the exact exit label
        if (lowerArg == lowerDestName
            || lowerArg == lowerExitLbl
            || lowerArg == "to " + lowerDestName) 
        {
            player->setCurrentRoom(dest);
            dest->enter();

            // After moving, list the available locations
            std::cout << "You can go to:\n";
            for (const auto& nextKv : dest->getAllExits()) {
                std::cout << "  - " << nextKv.second->getTo()->getName() << "\n";
            }
            return;
        }
    }

    std::cout << "You can't go there from here.\n";
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string>) {
    Room* currentRoom = player->getCurrentRoom();
    std::cout << currentRoom->getDescription() << "\n";
    std::cout << "You can go to:\n";
    for (const auto& kv : currentRoom->getAllExits()) {
        std::cout << "  - " << kv.second->getTo()->getName() << "\n";
    }
}

void ZOOrkEngine::handleTakeCommand(std::vector<std::string>) {
    std::cout << "This functionality is not yet enabled.\n";
}

void ZOOrkEngine::handleDropCommand(std::vector<std::string>) {
    std::cout << "This functionality is not yet enabled.\n";
}

void ZOOrkEngine::handleQuitCommand(std::vector<std::string>) {
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
