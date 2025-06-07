// File: ZOOrkEngine.cpp

#include "ZOOrkEngine.h"
#include "EnemyTypes.h"
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
#include <cctype>  // for std::toupper

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
        auto words = tokenizeString(input);
        if (words.empty()) continue;

        std::string command = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (command == "go" || command == "goto" || command == "move") {
            handleGoCommand(arguments);
        }
        else if (command == "look" || command == "inspect") {
            handleLookCommand(arguments);
        }
        else if (command == "search") {
            handleSearchCommand(arguments);
        }
        else if (command == "take" || command == "get") {
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
            // Unrecognized input defaults to look
            handleLookCommand(words);
        }
    }
}

void ZOOrkEngine::handleGoCommand(const std::vector<std::string>& arguments) {
    if (arguments.empty()) {
        std::cout << "Go where?\n";
        return;
    }

    // Reconstruct the target room name
    std::string target;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0) target += " ";
        target += arguments[i];
    }
    std::string targetLower = makeLowercase(target);

    Room* currentRoom = player->getCurrentRoom();
    bool moved = false;

    // Try each exit
    for (const auto& kv : currentRoom->getAllExits()) {
        Room* dest = kv.second->getTo();
        std::string destNameLower = makeLowercase(dest->getName());

        if (destNameLower == targetLower) {
            moved = true;

            if (destNameLower == "the lab") {
                if (!player->hasKeycard("Lab Keycard")) {
                    std::cout << "Access Denied. Lab Keycard required.\n";
                    return;
                }
                player->dropItem("Lab Keycard");
                std::cout << "The door seals behind you with a deafening thud.\n"
                            "A cold, mechanical voice crackles over the speakers:\n\n"
                            "\"Congratulations, soldier. Through skill and sacrifice you have proven yourself worthy of the gift of immortality.\n"
                            "The very government you served has traded you to Kiriko as a pawn in their grand design.\n"
                            "Now you stand at a crossroads:\n\n"
                            "1) Upload your mind into the network live forever as data, a ghost in their machine.\n"
                            "2) Use the Overwrite Card to open the escape hatch return to flesh and breathe free air once more.\n"
                            "3) End your life here refuse this cruel destiny.\n\n"
                            "Enter 1, 2, or 3: \"";

                int choice;
                while (true) {
                    std::cin >> choice;
                    if (choice >= 1 && choice <= 3) {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "\n";
                        break;
                    }
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid choice. Enter 1, 2, or 3: ";
                }

                switch (choice) {
                    case 1:
                        std::cout << "You press the neural uplink button. Pain like a furnace burns your mind as data streams away.\n"
                                     "Your body collapses. Your consciousness remains trapped in code, immortal but imprisoned.\n";
                        break;

                    case 2:
                        if (!player->hasKeycard("Overwrite Card")) {
                            std::cout << "You slam your hand on the console, but without the Overwrite Card nothing happens.\n"
                                         "The chamber hums as life support cuts off. You gasp and choke in the failing air.\n";
                        } else {
                            player->dropItem("Overwrite Card");
                            std::cout << "You slide the Overwrite Card into the slot. The hatch snaps open.\n"
                                         "You crawl through to freedom, lungs burning with cold night air. You're alive for now.\n";
                        }
                        break;

                    case 3:
                        std::cout << "You raise your weapon to your head. No words, no struggle just a single shot. Everything goes black.\n";
                        break;
                }

                std::cout << "\n=== END OF LINE ===\n";
                std::exit(0);
            }
            // Normal move
            player->setCurrentRoom(dest);
            dest->enter();
            std::cout << "\nExits:\n";
            for (const auto& exitPair : dest->getAllExits()) {
                std::cout << "  - " << exitPair.second->getTo()->getName() << "\n";
            }

            // --- Zoo combat on first arrival ---
            if (dest->getName() == "Zoo" && firstArrivalToZoo) {
                firstArrivalToZoo = false;
                std::cout << "\nAs you approach the empty pits of the abandoned zoo, a scavenger emerges from the shadows!\n\n";

                auto playerCombatant = std::make_shared<PlayerCombatant>("You");
                auto rifleItem = player->getInventoryItem("Rifle");
                if (rifleItem && rifleItem->getWeapon()) {
                    playerCombatant->equipWeapon(rifleItem->getWeapon());
                } else {
                    playerCombatant->equipWeapon(WeaponFactory::createWeapon(WeaponType::Pistol));
                }

                std::vector<std::shared_ptr<Enemy>> foes;
                foes.push_back(std::make_shared<Enemy>(EnemyType::Scav));

                CombatManager cm;
                if (!cm.engage(*playerCombatant, foes)) {
                    std::cout << "\nYou have been killed in combat. Game Over.\n";
                    std::exit(0);
                }

                std::cout << "\nThe scavenger lies still.\n";
                dest->addLookable("dropped pistol", "A scavenger's pistol lies on the ground.");
                dest->addSearchable("dropped pistol", "You pick up the dropped Pistol.");
                std::cout << "\nReentering " << dest->getName() << "...\n\n";
                dest->enter();
                std::cout << "\nExits:\n";
                for (auto& kv2 : dest->getAllExits())
                    std::cout << "  - " << kv2.second->getTo()->getName() << "\n";
            }

            // --- Lab North Entrance combat on first arrival ---
            if (dest->getName() == "Lab North Entrance" && firstArrivalToLabNorth) {
                firstArrivalToLabNorth = false;
                std::cout << "\nA Japanese PMC squad blocks the Lab North Entrance!\n\n";

                auto playerCombatant = std::make_shared<PlayerCombatant>("You");
                auto rifleItem = player->getInventoryItem("Rifle");
                if (rifleItem && rifleItem->getWeapon()) {
                    playerCombatant->equipWeapon(rifleItem->getWeapon());
                } else {
                    playerCombatant->equipWeapon(WeaponFactory::createWeapon(WeaponType::Pistol));
                }

                std::vector<std::shared_ptr<Enemy>> foes;
                foes.push_back(std::make_shared<Enemy>(EnemyType::PMC_Japanese));

                CombatManager cm;
                if (!cm.engage(*playerCombatant, foes)) {
                    std::cout << "\nYou have been killed by the Japanese PMC squad. Game Over.\n";
                    std::exit(0);
                }

                std::cout << "\nThe PMC soldier falls.\n";
                dest->addLookable("dropped ammo box", "An ammo box stamped with PMC Japanese lies cracked open.");
                dest->addSearchable("dropped ammo box", "You pick up some usable rounds.");
                std::cout << "\nReentering " << dest->getName() << "...\n\n";
                dest->enter();
                std::cout << "\nExits:\n";
                for (auto& kv2 : dest->getAllExits())
                    std::cout << "  - " << kv2.second->getTo()->getName() << "\n";
            }

            // --- Lab Underground Entrance combat on first arrival ---
            if (dest->getName() == "Lab Underground Entrance" && firstArrivalToLabUnderground) {
                firstArrivalToLabUnderground = false;
                std::cout << "\nAs you pry open the bioluminescent door to the underground labs, alarms echo in the corridors!\n\n";

                auto playerCombatant = std::make_shared<PlayerCombatant>("You");
                auto rifleItem = player->getInventoryItem("Rifle");
                if (rifleItem && rifleItem->getWeapon()) {
                    playerCombatant->equipWeapon(rifleItem->getWeapon());
                } else {
                    playerCombatant->equipWeapon(WeaponFactory::createWeapon(WeaponType::Pistol));
                }

                std::vector<std::shared_ptr<Enemy>> foes;
                foes.push_back(std::make_shared<Enemy>(EnemyType::PMC_Japanese));

                CombatManager cm;
                if (!cm.engage(*playerCombatant, foes)) {
                    std::cout << "\nYou have been killed by the Japanese PMC guard. Game Over.\n";
                    std::exit(0);
                }

                std::cout << "\nThe PMC guard collapses to the floor.\n";
                dest->addLookable(
                    "dropped keycard",
                    "A Japanese PMC keycard lies on the floor, its chip still warm."
                );
                dest->addSearchable(
                    "dropped keycard",
                    "You pick up the dropped Lab Keycard."
                );
                std::cout << "\nReentering " << dest->getName() << "...\n\n";
                dest->enter();
                std::cout << "\nExits:\n";
                for (auto& kv2 : dest->getAllExits())
                    std::cout << "  - " << kv2.second->getTo()->getName() << "\n";
            }

            // --- Lab Courtyard combat on first arrival ---
            if (dest->getName() == "Lab Courtyard" && firstArrivalToLabCourtyard) {
                firstArrivalToLabCourtyard = false;
                std::cout << "\nStepping into the overgrown courtyard, a Japanese PMC soldier emerges from cover!\n\n";

                auto playerCombatant = std::make_shared<PlayerCombatant>("You");
                auto rifleItem = player->getInventoryItem("Rifle");
                if (rifleItem && rifleItem->getWeapon()) {
                    playerCombatant->equipWeapon(rifleItem->getWeapon());
                } else {
                    playerCombatant->equipWeapon(WeaponFactory::createWeapon(WeaponType::Pistol));
                }

                std::vector<std::shared_ptr<Enemy>> foes;
                foes.push_back(std::make_shared<Enemy>(EnemyType::PMC_Japanese));

                CombatManager cm;
                if (!cm.engage(*playerCombatant, foes)) {
                    std::cout << "\nThe PMC soldier overpowers you. Game Over.\n";
                    std::exit(0);
                }

                std::cout << "\nThe PMC soldier collapses.\n";
                dest->addLookable(
                    "dropped rifle",
                    "A Japanese PMC rifle lies abandoned in the mud."
                );
                dest->addSearchable(
                    "dropped rifle",
                    "You pick up the dropped Rifle."
                );
                std::cout << "\nReentering " << dest->getName() << "...\n\n";
                dest->enter();
                std::cout << "\nExits:\n";
                for (auto& kv2 : dest->getAllExits())
                    std::cout << "  - " << kv2.second->getTo()->getName() << "\n";
            }
        }
    }

    if (!moved) {
        std::cout << "You can't go to \"" << target << "\" from here.\n";
    }
}

void ZOOrkEngine::handleLookCommand(const std::vector<std::string>& arguments) {
    Room* currentRoom = player->getCurrentRoom();
    if (arguments.empty()) {
        std::cout << "\n" << currentRoom->getDescription() << "\n";
        std::cout << "Exits:\n";
        for (const auto& kv : currentRoom->getAllExits()) {
            std::cout << "  - " << kv.second->getTo()->getName() << "\n";
        }
    } else {
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
            currentRoom->addLookable("lab keycard",   "A Lab Keycard glints on the counter.");
            currentRoom->addSearchable("lab keycard","You pick up the Lab Keycard; you can now access all Lab entrances.");
        }
        else if (target == "dead body") {
            currentRoom->addLookable(
                "overwrite card",
                "A sleek Overwrite Card stamped with the Longxue BioTech seal gleams here."
            );
            currentRoom->addSearchable(
                "overwrite card",
                "You pick up the Overwrite Card."
            );
        }
    } else {
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

        if (target == "rifle" || target == "shotgun" || target == "pistol") {
            properName = std::string(1, static_cast<char>(std::toupper(target[0]))) + target.substr(1);
            type = ItemType::Weapon;
        }
        else if (target == "lab keycard") {
            properName = "Lab Keycard";
            type = ItemType::Keycard;
        }
        else if (target == "overwrite card") {
            properName = "Overwrite Card";
            type = ItemType::Keycard;
        }
        else {
            std::cout << "You can't pick that up.\n";
            return;
        }

        std::shared_ptr<Item> newItem;
        if (type == ItemType::Weapon) {
            WeaponType wtype = WeaponType::Pistol;
            if (properName == "Rifle")        wtype = WeaponType::Rifle;
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
                "A Keycard stamped with the Longxue BioTech seal.",
                ItemType::Keycard
            );
        }

        if (player->pickUpItem(newItem)) {
            std::cout << "Picked up: " << properName << "\n";
        }
    } else {
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