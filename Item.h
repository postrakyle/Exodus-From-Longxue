#ifndef ZOORK_ITEM_H
#define ZOORK_ITEM_H

#include "GameObject.h"
#include <memory>
#include <string>

// Enum to categorize items
enum class ItemType {
    Weapon,
    Armor,
    Medkit,
    Keycard,
    OverwriteCard
};

class Player;

class Item : public GameObject {
public:
    // Constructor: name, description, and type
    Item(const std::string &name, const std::string &desc, ItemType type);

    // Accessors
    ItemType getItemType() const;

    int getDamage() const;
    void setDamage(int dmg);

    int getArmorBonus() const;
    void setArmorBonus(int bonus);

    int getHealAmount() const;
    void setHealAmount(int heal);

    // “Use” the item (e.g. armor, medkit, keycard)
    void use(Player *player);

private:
    ItemType itemType;
    int damage = 0;      // for weapons
    int armorBonus = 0;  // for armor
    int healAmount = 0;  // for medkits
};

#endif //ZOORK_ITEM_H
