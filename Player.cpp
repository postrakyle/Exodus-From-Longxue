#include "Player.h"
#include "Item.h"
#include <iostream>

Player* Player::playerInstance = nullptr;

Player::Player()
    : Character("You", "A rugged survivor in the war‐torn streets."),
      currentRoom(nullptr) {}

Player* Player::instance() {
    if (!playerInstance) {
        playerInstance = new Player();
    }
    return playerInstance;
}

void Player::setCurrentRoom(Room *room) {
    currentRoom = room;
}

Room* Player::getCurrentRoom() const {
    return currentRoom;
}

void Player::equipArmorBonus(int bonus) {
    headHealth += bonus;
    thoraxHealth += bonus;
    armsHealth += bonus;
    legsHealth += bonus;
    std::cout << "Your armor increases each hit location by +" << bonus << ".\n";
}

void Player::useMedkitHeal(int amount) {
    thoraxHealth += amount;
    headHealth += amount / 2;
    armsHealth += amount / 2;
    legsHealth += amount / 2;
    std::cout << "You use the medkit and restore health across your body.\n";
}
