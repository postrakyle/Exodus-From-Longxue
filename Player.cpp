// --- Player.cpp ---
#include "Player.h"
Player *Player::playerInstance = nullptr;
void Player::setCurrentRoom(Room* room) { currentRoom = room; }
Room* Player::getCurrentRoom() const { return currentRoom; }
