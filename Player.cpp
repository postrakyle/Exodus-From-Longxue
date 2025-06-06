// ---------------------------------
// File: Player.cpp
// ---------------------------------
#include "Player.h"
#include <algorithm>

//
// Initialize static singleton pointer to nullptr
//
Player* Player::playerInstance = nullptr;

//
// private constructor
//
Player::Player()
    : Character("You", "A lone survivor in the ruined city."),
      currentRoom(nullptr),
      headHealth(50),
      thoraxHealth(200),
      armsHealth(150),
      legsHealth(150)
{
}

//
// Singleton accessor
//
Player* Player::instance() {
    if (!playerInstance) {
        playerInstance = new Player();
    }
    return playerInstance;
}

//
// Setter/getter for player's current room
//
void Player::setCurrentRoom(Room *room) {
    currentRoom = room;
}

Room* Player::getCurrentRoom() const {
    return currentRoom;
}

//
// Called by Item::use(...) when an armor item is applied.
// We simply add 'bonus' to thoraxHealth (capped at original max + bonus).
//
void Player::equipArmorBonus(int bonus) {
    // Suppose base thorax max is 200. New cap = 200 + bonus.
    thoraxHealth += bonus;
    int cap = 200 + bonus;
    if (thoraxHealth > cap) {
        thoraxHealth = cap;
    }
}

//
// Called by Item::use(...) when a medkit is applied.
// Here we restore each body part by 'amount', capped at its original maximum.
//
void Player::useMedkitHeal(int amount) {
    headHealth   = std::min(headHealth   + amount, 50);
    thoraxHealth = std::min(thoraxHealth + amount, 200);
    armsHealth   = std::min(armsHealth   + amount, 150);
    legsHealth   = std::min(legsHealth   + amount, 150);
}
