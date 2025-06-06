//Inventory.cpp
#include "Inventory.h"
#include "Item.h"
#include <algorithm>

Inventory::Inventory() {}

bool Inventory::addItem(std::shared_ptr<Item> item) {
    if (!item) return false;

    ItemType type = item->getItemType();
    if (type == ItemType::Weapon) {
        if ((int)equippedWeapons.size() >= MAX_WEAPONS) {
            return false;
        }
        equippedWeapons.push_back(item);
    } else if (type == ItemType::Armor) {
        equippedArmor = item;
    }

    if ((int)items.size() < MAX_SLOTS) {
        items.push_back(item);
        return true;
    }
    return false;
}

std::shared_ptr<Item> Inventory::removeItem(const std::string &itemName) {
    auto idxOpt = findIndexByName(itemName);
    if (!idxOpt) return nullptr;

    size_t idx = *idxOpt;
    auto removed = items[idx];
    items.erase(items.begin() + idx);

    // If it was equipped armor, unequip
    if (equippedArmor && equippedArmor->getName() == itemName) {
        equippedArmor.reset();
    }

    // If it was an equipped weapon, remove from equippedWeapons
    equippedWeapons.erase(
        std::remove_if(
            equippedWeapons.begin(),
            equippedWeapons.end(),
            [&](auto &w) { return w->getName() == itemName; }
        ),
        equippedWeapons.end()
    );
    return removed;
}

bool Inventory::hasItem(const std::string &itemName) const {
    return static_cast<bool>(findIndexByName(itemName));
}

std::shared_ptr<Item> Inventory::getItem(const std::string &itemName) const {
    auto idxOpt = findIndexByName(itemName);
    if (!idxOpt) return nullptr;
    return items[*idxOpt];
}

std::vector<std::string> Inventory::listItemNames() const {
    std::vector<std::string> names;
    names.reserve(items.size());
    for (const auto &it : items) {
        names.push_back(it->getName());
    }
    return names;
}

bool Inventory::equipArmor(const std::string &armorName) {
    auto idxOpt = findIndexByName(armorName);
    if (!idxOpt) return false;
    size_t idx = *idxOpt;
    if (items[idx]->getItemType() != ItemType::Armor) return false;
    equippedArmor = items[idx];
    return true;
}

bool Inventory::equipWeapon(const std::string &weaponName) {
    if ((int)equippedWeapons.size() >= MAX_WEAPONS) return false;
    auto idxOpt = findIndexByName(weaponName);
    if (!idxOpt) return false;
    size_t idx = *idxOpt;
    if (items[idx]->getItemType() != ItemType::Weapon) return false;
    equippedWeapons.push_back(items[idx]);
    return true;
}

bool Inventory::unequipArmor() {
    if (!equippedArmor) return false;
    equippedArmor.reset();
    return true;
}

bool Inventory::unequipWeapon(const std::string &weaponName) {
    auto it = std::find_if(
        equippedWeapons.begin(),
        equippedWeapons.end(),
        [&](auto &w) { return w->getName() == weaponName; }
    );
    if (it == equippedWeapons.end()) return false;
    equippedWeapons.erase(it);
    return true;
}

int Inventory::getArmorBonus() const {
    return equippedArmor ? equippedArmor->getArmorBonus() : 0;
}

std::vector<std::shared_ptr<Item>> Inventory::getEquippedWeapons() const {
    return equippedWeapons;
}

void Inventory::clearAll() {
    items.clear();
    equippedWeapons.clear();
    equippedArmor.reset();
}

std::optional<size_t> Inventory::findIndexByName(const std::string &name) const {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->getName() == name) {
            return i;
        }
    }
    return std::nullopt;
}
