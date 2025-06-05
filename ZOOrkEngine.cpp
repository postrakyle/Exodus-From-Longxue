#include "ZOOrkEngine.h"
#include "Passage.h"
#include "Room.h"
#include "Item.h"
#include "Player.h"
#include <algorithm>
#include <sstream>
#include <limits>
#include <iostream>

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    player = Player::instance();
    player->setCurrentRoom(start.get());

    // Show room description, exits, and inspectables
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
            // If they type an object or room name, assume look
            handleLookCommand(words);
        }
    }
}

void ZOOrkEngine::handleGoCommand(const std::vector<std::string>& arguments) {
    if (arguments.empty()) return;
    std::string target;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) target += " ";
        target += arguments[i];
    }

    Room* currentRoom = player->getCurrentRoom();
    std::string targetLower = makeLowercase(target);

    for (const auto& kv : currentRoom->getAllExits()) {
        Room* dest = kv.second->getTo();
        std::string destNameLower = makeLowercase(dest->getName());
        if (destNameLower == targetLower) {
            player->setCurrentRoom(dest);
            dest->enter();

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

        // Spawn tangible items after search, if not already present
        if (target == "rifle case") {
            // Create Rifle
            auto rifle = std::make_shared<Item>("Rifle", "A standard assault rifle.", ItemType::Weapon);
            rifle->setDamage(40);
            currentRoom->addLookable("rifle", "A sturdy assault rifle leans against the seat.");
            currentRoom->addSearchable("rifle", "You pick up the Rifle. Damage: 40.");
        }
        else if (target == "shotgun rack") {
            // Create Shotgun
            auto shotgun = std::make_shared<Item>("Shotgun", "A worn but functional shotgun.", ItemType::Weapon);
            shotgun->setDamage(60);
            currentRoom->addLookable("shotgun", "A shotgun rests atop a broken chair.");
            currentRoom->addSearchable("shotgun", "You pick up the Shotgun. Damage: 60.");
        }
        else if (target == "tv rack") {
            // Create Lab Keycard
            auto keycard = std::make_shared<Item>("Lab Keycard", "A Level‐2 lab keycard.", ItemType::Keycard);
            currentRoom->addLookable("lab keycard", "A Level‐2 lab keycard glints on the counter.");
            currentRoom->addSearchable("lab keycard", "You pick up the Lab Keycard. Can open the lab.");
        }
        // (Other lore‐only searches already exist from WorldManager)
    }
    else {
        std::cout << "You find nothing interesting when searching \"" << target << "\".\n";
    }
}

void ZOOrkEngine::handleTakeCommand(const std::vector<std::string>& arguments) {
    if (arguments.empty()) {
        std::cout << "Take what?\n";
        return;
    }
    std::string target;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) target += " ";
        target += arguments[i];
    }

    Room* currentRoom = player->getCurrentRoom();
    if (currentRoom->isLookable(target)) {
        // Map object→proper Item name
        std::string properName;
        ItemType type;

        if (target == "rifle") {
            properName = "Rifle";
            type = ItemType::Weapon;
        } else if (target == "shotgun") {
            properName = "Shotgun";
            type = ItemType::Weapon;
        } else if (target == "lab keycard") {
            properName = "Lab Keycard";
            type = ItemType::Keycard;
        } else {
            std::cout << "You can't pick that up.\n";
            return;
        }

        auto newItem = std::make_shared<Item>(properName, "", type);
        if (type == ItemType::Weapon) {
            if (properName == "Rifle") newItem->setDamage(40);
            else if (properName == "Shotgun") newItem->setDamage(60);
        }

        if (player->pickUpItem(newItem)) {
            std::cout << "Picked up: " << properName << "\n";
            // Remove from room’s lookables/searchables
            // (Optional: implement removeLookable/removeSearchable if desired)
        }
    }
    else {
        std::cout << "There is no \"" << target << "\" here to take.\n";
    }
}

void ZOOrkEngine::handleDropCommand(const std::vector<std::string>& arguments) {
    if (arguments.empty()) {
        std::cout << "Drop what?\n";
        return;
    }
    std::string target;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) target += " ";
        target += arguments[i];
    }

    if (player->dropItem(target)) {
        Room* currentRoom = player->getCurrentRoom();
        currentRoom->addLookable(target, "A " + target + " lies here on the ground.");
        currentRoom->addSearchable(target, "You see the " + target + " sitting on the floor.");
    }
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
