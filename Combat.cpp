// File: Combat.cpp

#include "Combat.h"
#include <sstream>    // for std::istringstream
#include <iostream>   // for std::cout

// Initialize the RNG once
std::mt19937 Combatant::rng(std::random_device{}());

//
//  Combatant implementation
//

Combatant::Combatant(const std::string& n, bool isPlayerCtrl)
    : name(n),
      isPlayer(isPlayerCtrl),
      flanking(false),
      flankCountdown(0),
      distance(Distance::Medium)
{
    initBodyParts(false);
}

void Combatant::initBodyParts(bool isScav) {
    if (isScav) {
        bodyParts[BodyPartType::Head]   = BodyPart(50);
        bodyParts[BodyPartType::Thorax] = BodyPart(150);
        bodyParts[BodyPartType::Arm]    = BodyPart(100);
        bodyParts[BodyPartType::Leg]    = BodyPart(100);
    } else {
        bodyParts[BodyPartType::Head]   = BodyPart(50);
        bodyParts[BodyPartType::Thorax] = BodyPart(200);
        bodyParts[BodyPartType::Arm]    = BodyPart(150);
        bodyParts[BodyPartType::Leg]    = BodyPart(150);
    }
}

bool Combatant::isDead() const {
    bool headDown   = bodyParts.at(BodyPartType::Head).isBlackedOut();
    bool thoraxDown = bodyParts.at(BodyPartType::Thorax).isBlackedOut();
    return headDown || thoraxDown;
}

void Combatant::applyDamage(BodyPartType part, int dmg) {
    auto& bp = bodyParts[part];
    if (bp.hp <= 0) {
        // Already “blacked out.” Any second hit on any limb kills instantly.
        bodyParts[BodyPartType::Head].hp   = 0;
        bodyParts[BodyPartType::Thorax].hp = 0;
        return;
    }
    bp.hp -= dmg;
    if (bp.hp < 0) bp.hp = 0;
}

double Combatant::calculateHitChance(BodyPartType targetPart) const {
    if (!weapon) return 0.0;

    // If using a pistol, use fixed percentages based on distance & target
    if (weapon->getType() == WeaponType::Pistol) {
        switch (distance) {
            case Distance::Far:
                switch (targetPart) {
                    case BodyPartType::Head:   return 0.20;
                    case BodyPartType::Thorax: return 0.30;
                    case BodyPartType::Arm:    return 0.25;
                    case BodyPartType::Leg:    return 0.25;
                }
                break;
            case Distance::Medium:
                switch (targetPart) {
                    case BodyPartType::Head:   return 0.35;
                    case BodyPartType::Thorax: return 0.50;
                    case BodyPartType::Arm:    return 0.40;
                    case BodyPartType::Leg:    return 0.40;
                }
                break;
            case Distance::Close:
                switch (targetPart) {
                    case BodyPartType::Head:   return 0.70;
                    case BodyPartType::Thorax: return 0.90;
                    case BodyPartType::Arm:    return 0.80;
                    case BodyPartType::Leg:    return 0.80;
                }
                break;
        }
    }

    // Fallback for other weapons
    double acc = weapon->getAccuracy();
    if (weapon->getType() == WeaponType::Rifle && weapon->isScoped()) {
        acc += 0.40;
    }
    static constexpr double distMods[3] = {1.0, 0.7, 0.4};
    acc *= distMods[static_cast<int>(distance)];

    double partMod = 1.0;
    switch (targetPart) {
        case BodyPartType::Head:   partMod = 0.30; break;
        case BodyPartType::Thorax: partMod = 0.80; break;
        case BodyPartType::Arm:    partMod = 0.60; break;
        case BodyPartType::Leg:    partMod = 0.60; break;
    }
    acc *= partMod;

    if (acc < 0.0) acc = 0.0;
    if (acc > 1.0) acc = 1.0;
    return acc;
}

bool Combatant::shootAt(std::shared_ptr<Combatant> target, BodyPartType targetPart) {
    if (!weapon || weapon->needsReload()) {
        return false;
    }
    if (!weapon->fireOne()) {
        return false;
    }

    // If target is behind cover, shot is blocked
    if (target->inCover) {
        std::cout << name << "'s shot is blocked by " << target->getName() << "'s cover!\n";
        return true;
    }

    double hitChance = calculateHitChance(targetPart);
    std::uniform_real_distribution<double> uni(0.0, 1.0);
    double roll = uni(rng);

    if (roll <= hitChance) {
        int damage = weapon->getDamage();
        bool alreadyBlacked = target->bodyParts.at(targetPart).isBlackedOut();
        target->applyDamage(targetPart, damage);

        if (target->isDead()) {
            // Brutal death description based on where they were shot
            switch (targetPart) {
                case BodyPartType::Head:
                    std::cout << name << " unloads a round into " << target->getName()
                              << "'s skull. The bullet fractures bone, splintering skull matter "
                              << "as blood and brain tissue erupt outward. Lying limp, the body "
                              << "twists as life bleeds from the shattered cranium.\n";
                    break;
                case BodyPartType::Thorax:
                    std::cout << name << " fires at " << target->getName()
                              << "'s chest, the bullet tearing through ribs. A sickening crack "
                              << "echoes as lungs collapse, lungs filling with frothy crimson. "
                              << "The corpse slumps forward, ribs jutting outward, each breath "
                              << "a gurgling gasp of blood.\n";
                    break;
                case BodyPartType::Arm:
                    std::cout << name << " shoots into " << target->getName()
                              << "'s arm, severing bone and tendons. The limb hangs by a shred, "
                              << "flesh splattering the ground. " << target->getName()
                              << " staggers, agony etched on their face before collapsing, "
                              << "blood pulsing from the torn stump.\n";
                    break;
                case BodyPartType::Leg:
                    std::cout << name << " blasts " << target->getName()
                              << "'s leg off, the femur erupting like a fountain of gore. "
                              << "Chunks of muscle and bone rain down as the corpse crumples, "
                              << "unable to stand on one remaining filthy stump.\n";
                    break;
            }
        } else {
            // Non‐lethal or limb‐only hit
            std::cout << name << " hits " << target->getName()
                      << " in the "
                      << (targetPart == BodyPartType::Head   ? "Head"
                          : targetPart == BodyPartType::Thorax ? "Thorax"
                          : targetPart == BodyPartType::Arm     ? "Arm"
                                                               : "Leg")
                      << " for " << damage << " damage!\n";
        }
    } else {
        std::cout << name << " fires at " << target->getName() << " and misses.\n";
    }
    return true;
}

void Combatant::reloadWeapon() {
    if (weapon) {
        std::cout << name << " reloads the " << weapon->getName() << ".\n";
        weapon->reload();
    }
}

bool Combatant::attemptFlee() {
    // No longer used directly; fleeing logic is in promptPlayerAction
    return false;
}

//
//  Enemy implementation
//

Enemy::Enemy(EnemyType t)
    : Combatant(
          t == EnemyType::Scav
            ? "Scavenger"
            : (t == EnemyType::PMC_Chinese ? "PMC (C)" : "PMC (J)"),
          false)
{
    enemyType = t;
    bool isScav = (t == EnemyType::Scav);
    initBodyParts(isScav);

    if (isScav) {
        equipWeapon(WeaponFactory::createWeapon(WeaponType::Pistol));
    } else {
        std::uniform_real_distribution<double> uni(0.0, 1.0);
        if (uni(rng) < 0.7) {
            equipWeapon(WeaponFactory::createWeapon(WeaponType::AssaultRifle));
        } else {
            equipWeapon(WeaponFactory::createWeapon(WeaponType::Pistol));
        }
    }
    inCover = false;  // no cover behavior
}

void Enemy::decideAction(std::shared_ptr<Combatant> player) {
    if (isDead()) return;

    if (weapon->needsReload() || weapon->getAmmo() == 0) {
        reloadWeapon();
        return;
    }

    // If player is behind cover, enemy cannot hit
    if (player->inCover) {
        std::cout << name << "'s shot is blocked by " << player->getName() << "'s cover!\n";
        return;
    }

    std::uniform_real_distribution<double> uni(0.0, 1.0);
    BodyPartType target = BodyPartType::Thorax;
    if (!player->bodyParts.at(BodyPartType::Head).isBlackedOut() && uni(rng) < 0.2) {
        target = BodyPartType::Head;
    } else if (!player->bodyParts.at(BodyPartType::Leg).isBlackedOut() && uni(rng) < 0.3) {
        target = BodyPartType::Leg;
    } else if (!player->bodyParts.at(BodyPartType::Arm).isBlackedOut() && uni(rng) < 0.3) {
        target = BodyPartType::Arm;
    }

    shootAt(player, target);
}

//
//  PlayerCombatant implementation
//

PlayerCombatant::PlayerCombatant(const std::string& n)
    : Combatant(n, true)
{
    initBodyParts(false);
    equipWeapon(WeaponFactory::createWeapon(WeaponType::Pistol));
}

bool PlayerCombatant::attemptFlee() {
    // No longer used directly; fleeing logic is in promptPlayerAction
    return false;
}

void PlayerCombatant::displayStatus() const {
    // Print all limb HP horizontally
    std::cout << "\n=== " << name << " Status ===\n";
    std::cout << "Head: "   << bodyParts.at(BodyPartType::Head).hp   << "/" << bodyParts.at(BodyPartType::Head).maxHp   << "  |  "
              << "Thorax: " << bodyParts.at(BodyPartType::Thorax).hp << "/" << bodyParts.at(BodyPartType::Thorax).maxHp << "  |  "
              << "Arm: "    << bodyParts.at(BodyPartType::Arm).hp    << "/" << bodyParts.at(BodyPartType::Arm).maxHp    << "  |  "
              << "Leg: "    << bodyParts.at(BodyPartType::Leg).hp    << "/" << bodyParts.at(BodyPartType::Leg).maxHp    << "\n";
    // Then print weapon & universal distance
    std::cout << "Weapon: "
              << (weapon ? weapon->getName() : "None")
              << " [" << (weapon ? std::to_string(weapon->getAmmo()) + "/" + std::to_string(weapon->getMaxAmmo()) : "N/A") << "]  |  "
              << "Distance: "
              << (distance == Distance::Close  ? "Close"
                 : distance == Distance::Medium ? "Medium"
                                                : "Far")
              << "\n========================\n";
}

//
//  CombatManager implementation
//

bool CombatManager::allEnemiesDead(const std::vector<std::shared_ptr<Enemy>>& enemies) const {
    for (auto& e : enemies) {
        if (!e->isDead()) return false;
    }
    return true;
}

void CombatManager::displayCombatants(PlayerCombatant& player,
                                      const std::vector<std::shared_ptr<Enemy>>& enemies) const
{
    player.displayStatus();

    std::cout << "\n=== Enemies ===\n";
    for (size_t i = 0; i < enemies.size(); ++i) {
        auto& e = enemies[i];
        if (e->isDead()) {
            std::cout << i << ": " << e->getName() << " [DEAD]\n";
        } else {
            std::cout << i << ": " << e->getName()
                      << "  |  Head: "   << e->getHp(BodyPartType::Head)   << "/" << e->bodyParts.at(BodyPartType::Head).maxHp
                      << "  |  Th: "     << e->getHp(BodyPartType::Thorax) << "/" << e->bodyParts.at(BodyPartType::Thorax).maxHp
                      << "  |  A: "      << e->getHp(BodyPartType::Arm)    << "/" << e->bodyParts.at(BodyPartType::Arm).maxHp
                      << "  |  L: "      << e->getHp(BodyPartType::Leg)    << "/" << e->bodyParts.at(BodyPartType::Leg).maxHp
                      << "\n";
        }
    }
    std::cout << "===============\n";
}

bool CombatManager::engage(PlayerCombatant& player,
                           std::vector<std::shared_ptr<Enemy>>& enemies)
{
    std::cout << "\n--- Combat Start! ---\n";
    currentDistance = Distance::Far;
    player.inCover = false;
    player.distance = currentDistance;
    for (auto& e : enemies) {
        e->distance = currentDistance;
    }

    while (true) {
        if (!playerTurn(player, enemies)) {
            return !player.isDead();
        }
        if (allEnemiesDead(enemies)) {
            std::cout << "The enemy lies still.\n";
            return true;
        }

        if (!enemiesTurn(player, enemies)) {
            return false;
        }
        if (allEnemiesDead(enemies)) {
            std::cout << "The enemy lies still.\n";
            return true;
        }
        if (player.isDead()) {
            std::cout << "\nYou have been killed in combat.\n";
            return false;
        }
    }
}

bool CombatManager::playerTurn(PlayerCombatant& player,
                               std::vector<std::shared_ptr<Enemy>>& enemies)
{
    player.tick();
    displayCombatants(player, enemies);
    return promptPlayerAction(player, enemies);
}

bool CombatManager::enemiesTurn(PlayerCombatant& player,
                                const std::vector<std::shared_ptr<Enemy>>& enemies)
{
    // Wrap actual player in shared_ptr (no-op deleter)
    std::shared_ptr<Combatant> playerPtr(&player, [](Combatant*){});
    for (auto& e : enemies) {
        if (e->isDead()) continue;
        e->tick();
        e->decideAction(playerPtr);
        if (player.isDead()) return false;
    }
    return true;
}

bool CombatManager::promptPlayerAction(PlayerCombatant& player,
                                       std::vector<std::shared_ptr<Enemy>>& enemies)
{
    std::cout << "\nChoose an action:\n";
    std::cout << " 1) Move Closer   2) Move Further   3) Take Cover\n";
    std::cout << " 4) Shoot         5) Reload         6) Flee <location>\n";
    std::cout << "Command> ";

    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "1" || cmd == "move closer") {
        if (currentDistance != Distance::Close) {
            currentDistance = static_cast<Distance>(static_cast<int>(currentDistance) - 1);
            player.distance = currentDistance;
            for (auto& e : enemies) {
                e->distance = currentDistance;
            }
        }
        std::cout << "You move closer.\n";
        return true;
    }
    if (cmd == "2" || cmd == "move further") {
        if (currentDistance != Distance::Far) {
            currentDistance = static_cast<Distance>(static_cast<int>(currentDistance) + 1);
            player.distance = currentDistance;
            for (auto& e : enemies) {
                e->distance = currentDistance;
            }
        }
        std::cout << "You move farther.\n";
        return true;
    }
    if (cmd == "3" || cmd == "take cover") {
        if (!player.isInCover()) {
            player.takeCover();
            std::cout << "You drop behind cover.\n";
        } else {
            std::cout << "You are already in cover.\n";
        }
        return true;
    }
    if (cmd.rfind("shoot", 0) == 0) {
        std::istringstream iss(cmd);
        std::string word, partStr;
        iss >> word >> partStr; // e.g. "shoot" and "head"
        if (partStr.empty()) {
            std::cout << "Usage: shoot <head|thorax|arm|leg>\n";
            return true;
        }
        BodyPartType targetPart = parseBodyPart(partStr);
        std::shared_ptr<Enemy> targetEnemy = nullptr;
        for (auto& e : enemies) {
            if (!e->isDead()) {
                targetEnemy = e;
                break;
            }
        }
        if (!targetEnemy) {
            std::cout << "No valid target to shoot.\n";
            return true;
        }
        if (!player.shootAt(targetEnemy, targetPart)) {
            std::cout << "Unable to shoot (no ammo or reloading).\n";
        }
        return true;
    }
    if (cmd == "5" || cmd == "reload") {
        player.reloadWeapon();
        return true;
    }
    if (cmd.rfind("flee", 0) == 0) {
        std::istringstream iss(cmd);
        std::string word, location;
        iss >> word;
        if (!(iss >> location)) {
            std::cout << "Usage: flee <location>\n";
            return true;
        }
        std::cout << "You flee back to " << location << ".\n";
        return false;
    }

    std::cout << "Unknown command. Try again.\n";
    return true;
}

BodyPartType CombatManager::parseBodyPart(const std::string& s) const {
    std::string u = s;
    for (auto& c : u) c = static_cast<char>(std::tolower(c));
    if (u == "head")   return BodyPartType::Head;
    if (u == "thorax") return BodyPartType::Thorax;
    if (u == "arm")    return BodyPartType::Arm;
    if (u == "leg")    return BodyPartType::Leg;
    return BodyPartType::Thorax;
}
