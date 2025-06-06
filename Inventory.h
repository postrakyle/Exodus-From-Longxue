//Inventory.h
#ifndef ZOORK_INVENTORY_H
#define ZOORK_INVENTORY_H

#include <memory>
#include <optional>
#include <string>
#include <vector>

class Item;

static constexpr int MAX_SLOTS = 20;
static constexpr int MAX_WEAPONS = 2;

class Inventory {
public:
    Inventory();

    // Add an item to inventory. Returns false if no space or too many weapons.
    bool addItem(std::shared_ptr<Item> item);

    // Remove an item by name; returns the removed shared_ptr or nullptr if not found.
    std::shared_ptr<Item> removeItem(const std::string &itemName);

    // Check if inventory has an item with that name (by exact string match)
    bool hasItem(const std::string &itemName) const;

    // Get an Item pointer by name (nullptr if missing)
    std::shared_ptr<Item> getItem(const std::string &itemName) const;

    // List all item names currently in inventory:
    std::vector<std::string> listItemNames() const;

    // Equip/unequip (not strictly needed here, but kept for completeness)
    bool equipArmor(const std::string &armorName);
    bool equipWeapon(const std::string &weaponName);
    bool unequipArmor();
    bool unequipWeapon(const std::string &weaponName);

    // If armor is equipped, return its bonus. Otherwise 0.
    int getArmorBonus() const;

    // Return all currently equipped weapons
    std::vector<std::shared_ptr<Item>> getEquippedWeapons() const;

    // Clear everything
    void clearAll();

private:
    std::optional<size_t> findIndexByName(const std::string &name) const;

    std::vector<std::shared_ptr<Item>> items;
    std::vector<std::shared_ptr<Item>> equippedWeapons; // up to MAX_WEAPONS
    std::shared_ptr<Item> equippedArmor;                // single armor slot
};

#endif // ZOORK_INVENTORY_H
