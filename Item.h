//Item.h
#ifndef ZOORK_ITEM_H
#define ZOORK_ITEM_H

#include "Weapons.h"
#include <memory>
#include <string>

//
// All possible item categories:
//
enum class ItemType { Weapon, Armor, Medkit, Keycard, Generic };

class Item {
public:
    // 1) Weapon‐backed constructor:
    Item(const std::string &n, const std::string &desc, std::shared_ptr<Weapon> w);

    // 2) Armor constructor (bonus = extra HP granted when equipped):
    Item(const std::string &n, const std::string &desc, int armorBonus);

    // 3) Medkit constructor (healAmount is how much it heals):
    Item(const std::string &n, const std::string &desc, int healAmount, bool isMedkit);

    // 4) Keycard or Generic constructor:
    Item(const std::string &n, const std::string &desc, ItemType type);

    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    ItemType getItemType() const { return itemType; }

    // If this is weapon‐backed, returns its Weapon ptr; otherwise nullptr
    std::shared_ptr<Weapon> getWeapon() const { return weaponPtr; }

    // If Armor, return its bonus; otherwise 0
    int getArmorBonus() const { return armorBonus; }

    // If Medkit, return healing amount; otherwise 0
    int getHealAmount() const { return healAmount; }

private:
    std::string name;
    std::string description;
    ItemType itemType = ItemType::Generic;

    // Only one of these fields is non‐null / nonzero depending on type:
    std::shared_ptr<Weapon> weaponPtr; // for ItemType::Weapon
    int armorBonus = 0;                // for ItemType::Armor
    int healAmount = 0;                // for ItemType::Medkit
};

#endif // ZOORK_ITEM_H
