#ifndef ZOORK_INVENTORY_H
#define ZOORK_INVENTORY_H

#include <memory>
#include <string>
#include <vector>
#include <optional>   // for std::optional

class Item;

// Manages a player’s carried items, equipped armor, and weapons.
class Inventory {
public:
    Inventory();

    // Add an item to inventory. Returns true if added successfully.
    bool addItem(std::shared_ptr<Item> item);

    // Remove and return an item by name from inventory. nullptr if not found.
    std::shared_ptr<Item> removeItem(const std::string &itemName);

    // Check if an item with this name is in inventory.
    bool hasItem(const std::string &itemName) const;

    // List all item names currently carried.
    std::vector<std::string> listItemNames() const;

    // Equip a piece of armor by name (only one armor slot).
    bool equipArmor(const std::string &armorName);

    // Equip a weapon by name (up to MAX_WEAPONS).
    bool equipWeapon(const std::string &weaponName);

    // Unequip the currently equipped armor (if any).
    bool unequipArmor();

    // Unequip a specific weapon by name.
    bool unequipWeapon(const std::string &weaponName);

    // Return total armor bonus from equipped armor (0 if none).
    int getArmorBonus() const;

    // Return vector of all equipped weapons.
    std::vector<std::shared_ptr<Item>> getEquippedWeapons() const;

    // Clear all items and unequip everything.
    void clearAll();

private:
    static constexpr int MAX_SLOTS = 6;      // total inventory capacity
    static constexpr int MAX_WEAPONS = 2;    // max weapons equipped

    // All items carried (including equipped ones).
    std::vector<std::shared_ptr<Item>> items;

    // Currently equipped armor (nullptr if none).
    std::shared_ptr<Item> equippedArmor;

    // Currently equipped weapons (size ≤ MAX_WEAPONS).
    std::vector<std::shared_ptr<Item>> equippedWeapons;

    // Helper: find index in `items` by item name; returns std::nullopt if not present.
    std::optional<size_t> findIndexByName(const std::string &name) const;
};

#endif // ZOORK_INVENTORY_H
