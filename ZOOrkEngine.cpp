// File: ZOOrkEngine.cpp

#include "ZOOrkEngine.h"
#include "Passage.h"
#include "Room.h"
#include "Item.h"
#include "Player.h"
#include "Weapons.h"
#include "Combat.h"
#include <algorithm>
#include <sstream>
#include <limits>
#include <iostream>

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    player = Player::instance();
    player->setCurrentRoom(start.get());

    // Show initial room description and exits
    start->enter();
    std::cout << "\nExits:\n";
    Room* cur = player->getCurrentRoom();
    for (const auto& kv : cur->getAllExits()) {
        std::cout << "  - " << kv.second->getTo()->getName() << "\n";
    }
}

void ZOOrkEngine::setRoomMap(const std::map<std::string, std::shared_ptr<Room>>& m) {
    roomMap = m;
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "\n> ";
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
        else if (command == "inventory" || command == "inv") {
            handleInventoryCommand();
        }
        else if (command == "help") {
            handleHelpCommand();
        }
        else if (command == "quit") {
            handleQuitCommand(arguments);
        }
        else {
            // Unrecognized input defaults to “look”
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

            std::cout << "\nExits:\n";
            for (const auto& exitPair : dest->getAllExits()) {
                std::cout << "  - " << exitPair.second->getTo()->getName() << "\n";
            }

            // ─────────────────────────────────────────────────────────────
            // If we just entered the Zoo for the first time, trigger combat:
            // ─────────────────────────────────────────────────────────────
            if (dest->getName() == "Zoo" && firstArrivalToZoo) {
                firstArrivalToZoo = false;

                std::cout << "\nAs you approach the abandoned zoo, a scavenger emerges from a shattered cage!\n\n";

                // 1) Build a PlayerCombatant from the existing Player
                auto playerCombatant = std::make_shared<PlayerCombatant>("You");

                // If the Player’s inventory contains a “Rifle” item, equip that weapon:
                auto rifleItem = player->getInventoryItem("Rifle");
                if (rifleItem && rifleItem->getWeapon()) {
                    playerCombatant->equipWeapon(rifleItem->getWeapon());
                } else {
                    // Otherwise default to pistol
                    playerCombatant->equipWeapon(WeaponFactory::createWeapon(WeaponType::Pistol));
                }

                // 2) Create a single Scav enemy
                std::vector<std::shared_ptr<Enemy>> foes;
                auto scav = std::make_shared<Enemy>(EnemyType::Scav);
                foes.push_back(scav);

                // 3) Engage
                CombatManager cm;
                bool playerSurvived = cm.engage(*playerCombatant, foes);

                if (!playerSurvived) {
                    std::cout << "\nYou have been killed in combat. Game Over.\n";
                    exit(0);
                } else {
                    std::cout << "\nYou emerge victorious from the abandoned cages. The scavenger lies still.\n";

                    // Drop a “Pistol” item into the Zoo for the player to pick up
                    Room* zooRoom = dest;
                    auto pistolItem = std::make_shared<Item>(
                        "Pistol",
                        "A scavenger’s dropped 9mm pistol lies here.",
                        WeaponFactory::createWeapon(WeaponType::Pistol)
                    );
                    zooRoom->addLookable("dropped pistol", "A scavenger’s pistol lies on the ground.");
                    zooRoom->addSearchable("dropped pistol", "You pick up the dropped Pistol.");

                    // ──────────────────────────────────────────────────────────
                    // NOW: re‐introduce the room’s description & exits
                    std::cout << "\nAs the echoes of gunfire fade, you find yourself back in "
                              << dest->getName() << ".\n\n";
                    dest->enter();
                    std::cout << "\nExits:\n";
                    for (const auto& kv2 : dest->getAllExits()) {
                        std::cout << "  - " << kv2.second->getTo()->getName() << "\n";
                    }
                    // ──────────────────────────────────────────────────────────
                }
            }

            return;
        }
    }
    std::cout << "You can't go to \"" << target << "\" from here.\n";
}

void ZOOrkEngine::handleLookCommand(const std::vector<std::string>& arguments) {
    Room* currentRoom = player->getCurrentRoom();
    if (arguments.empty()) {
        std::cout << "\n" << currentRoom->getDescription() << "\n";
        std::cout << "Exits:\n";
        for (const auto& kv : currentRoom->getAllExits()) {
            std::cout << "  - " << kv.second->getTo()->getName() << "\n";
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

        // Spawn tangible items after searching, if not already created:
        if (target == "rifle case") {
            auto rifle = WeaponFactory::createWeapon(WeaponType::Rifle);
            currentRoom->addLookable("rifle",   "A sturdy assault rifle leans against the seat.");
            currentRoom->addSearchable("rifle", "You pick up the Rifle. Damage: 80.");
        }
        else if (target == "shotgun rack") {
            auto shotgun = WeaponFactory::createWeapon(WeaponType::Shotgun);
            currentRoom->addLookable("shotgun",   "A shotgun rests atop a broken chair.");
            currentRoom->addSearchable("shotgun", "You pick up the Shotgun. Damage: 60.");
        }
        else if (target == "tv rack") {
            auto keycard = std::make_shared<Item>(
                "Lab Keycard",
                "A Level-2 lab keycard.",
                ItemType::Keycard
            );
            currentRoom->addLookable("lab keycard",   "A Level-2 lab keycard glints on the counter.");
            currentRoom->addSearchable("lab keycard","You pick up the Lab Keycard. Can open the lab.");
        }
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
        } else if (target == "dropped pistol") {
            properName = "Pistol";
            type = ItemType::Weapon;
        } else {
            std::cout << "You can't pick that up.\n";
            return;
        }

        std::shared_ptr<Item> newItem;
        if (type == ItemType::Weapon) {
            WeaponType wtype = WeaponType::Pistol;
            if (properName == "Rifle")       wtype = WeaponType::Rifle;
            else if (properName == "Shotgun") wtype = WeaponType::Shotgun;
            else if (properName == "Pistol")  wtype = WeaponType::Pistol;

            newItem = std::make_shared<Item>(
                properName,
                "A " + properName + " dropped on the ground.",
                WeaponFactory::createWeapon(wtype)
            );
        } else {
            newItem = std::make_shared<Item>(
                properName,
                "A Level-2 lab keycard.",
                ItemType::Keycard
            );
        }

        if (player->pickUpItem(newItem)) {
            std::cout << "Picked up: " << properName << "\n";
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

void ZOOrkEngine::handleInventoryCommand() {
    auto contents = player->listInventory();
    if (contents.empty()) {
        std::cout << "Your inventory is empty.\n";
    } else {
        std::cout << "You are carrying:\n";
        for (const auto &itemName : contents) {
            std::cout << "  - " << itemName << "\n";
        }
    }
}

void ZOOrkEngine::handleHelpCommand() {
    std::cout << "Available commands:\n";
    std::cout << "  go <room>            - Move to a connected room (e.g. go Theater)\n";
    std::cout << "  look [<object>]      - Look around (room description) or at a specific object\n";
    std::cout << "  search <object>      - Search an object (may reveal items)\n";
    std::cout << "  take <item>          - Pick up an item after you’ve spawned it\n";
    std::cout << "  drop <item>          - Drop an item from your inventory\n";
    std::cout << "  inventory (inv)      - List items you are carrying\n";
    std::cout << "  help                 - Show this help text\n";
    std::cout << "  quit                 - Exit the game\n";
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
