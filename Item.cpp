//Item.cpp

#include "Item.h"

//
// Weapon‐backed constructor
//
Item::Item(const std::string &n, const std::string &desc, std::shared_ptr<Weapon> w)
    : name(n), description(desc), itemType(ItemType::Weapon), weaponPtr(std::move(w)),
      armorBonus(0), healAmount(0) { }

//
// Armor constructor
//
Item::Item(const std::string &n, const std::string &desc, int armorBonus_)
    : name(n), description(desc), itemType(ItemType::Armor),
      weaponPtr(nullptr), armorBonus(armorBonus_), healAmount(0) { }

//
// Medkit constructor
//
Item::Item(const std::string &n, const std::string &desc, int healAmount_, bool isMedkit)
    : name(n), description(desc), itemType(ItemType::Medkit),
      weaponPtr(nullptr), armorBonus(0), healAmount(healAmount_) { }

//
// Keycard or generic‐type constructor
//
Item::Item(const std::string &n, const std::string &desc, ItemType type_)
    : name(n), description(desc), itemType(type_),
      weaponPtr(nullptr), armorBonus(0), healAmount(0) { }
