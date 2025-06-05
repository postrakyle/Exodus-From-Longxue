#ifndef ZOORK_PLAYER_H
#define ZOORK_PLAYER_H

#include "Character.h"
#include "Room.h"
#include "Inventory.h"
#include <memory>
#include <string>

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

    bool hasKeycard(const std::string &cardName) const {
        return inventory.hasItem(cardName);
    }
    void useKeycard(const std::string &cardName) {
        inventory.removeItem(cardName);
    }

    int getArmorBonus() const {
        return inventory.getArmorBonus();
    }
    auto getEquippedWeapons() const {
        return inventory.getEquippedWeapons();
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

#endif //ZOORK_PLAYER_H
