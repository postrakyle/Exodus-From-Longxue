#include "Item.h"
#include "Player.h"
#include <iostream>

Item::Item(const std::string &name, const std::string &desc, ItemType type)
    : GameObject(name, desc), itemType(type) {}

ItemType Item::getItemType() const {
    return itemType;
}

int Item::getDamage() const {
    return damage;
}

void Item::setDamage(int dmg) {
    damage = dmg;
}

int Item::getArmorBonus() const {
    return armorBonus;
}

void Item::setArmorBonus(int bonus) {
    armorBonus = bonus;
}

int Item::getHealAmount() const {
    return healAmount;
}

void Item::setHealAmount(int heal) {
    healAmount = heal;
}

void Item::use(Player *player) {
    if (!player) return;

    switch (itemType) {
        case ItemType::Armor:
            std::cout << "You equip " << getName()
                      << ". It grants +" << armorBonus << " armor bonus.\n";
            player->equipArmorBonus(armorBonus);
            break;
        case ItemType::Medkit:
            std::cout << "You use " << getName()
                      << " and restore " << healAmount << " health.\n";
            player->useMedkitHeal(healAmount);
            break;
        case ItemType::Keycard:
            std::cout << "You swipe the " << getName()
                      << ". It unlocks the lab door.\n";
            player->useKeycard(getName());
            break;
        case ItemType::OverwriteCard:
            std::cout << "You hold up the overwrite card. It may override security later.\n";
            break;
        default:
            std::cout << "You can't use that right now.\n";
            break;
    }
}
