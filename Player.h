//Player.h
#ifndef ZOORK_PLAYER_H
#define ZOORK_PLAYER_H

#include "Character.h"
#include "Room.h"
#include "Inventory.h"
#include <memory>
#include <string>
#include <vector>

// Forward‐declare Item so we can return shared_ptr<Item>
class Item;

class Player : public Character {
public:
    static Player *instance();

    void setCurrentRoom(Room *room);
    Room* getCurrentRoom() const;

    // Inventory operations delegate to Inventory
    bool pickUpItem(std::shared_ptr<Item> item) {
        return inventory.addItem(std::move(item));
    }
    bool dropItem(const std::string &itemName) {
        auto removed = inventory.removeItem(itemName);
        return static_cast<bool>(removed);
    }

    // Check if we have a named keycard
    bool hasKeycard(const std::string &cardName) const {
        return inventory.hasItem(cardName);
    }
    void useKeycard(const std::string &cardName) {
        inventory.removeItem(cardName);
    }

    // Return a pointer to an Item in inventory (nullptr if missing)
    std::shared_ptr<Item> getInventoryItem(const std::string &itemName) const {
        return inventory.getItem(itemName);
    }

    // Return armor bonus from equipped armor
    int getArmorBonus() const {
        return inventory.getArmorBonus();
    }

    // Return currently equipped weapons
    auto getEquippedWeapons() const {
        return inventory.getEquippedWeapons();
    }

    // List all item names in inventory
    std::vector<std::string> listInventory() const {
        return inventory.listItemNames();
    }

    // Health fields
    int headHealth = 50;
    int thoraxHealth = 200;
    int armsHealth = 150;
    int legsHealth = 150;

    // Called by Item::use(...) for armor
    void equipArmorBonus(int bonus);
    // Called by Item::use(...) for medkit
    void useMedkitHeal(int amount);

private:
    static Player *playerInstance;
    Room *currentRoom;
    Inventory inventory;

    Player();
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
};

#endif // ZOORK_PLAYER_H
