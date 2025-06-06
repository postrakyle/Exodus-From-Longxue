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
      special(SpecialStat::None),
      inCover(false),
      flanking(false),
      flankCountdown(0),
      distance(Distance::Medium) // default start at Medium
{
    // By default, assume a “PMC”/player‐class (not Scav) unless overridden
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
        // Already “blacked out”: if head or thorax is targeted again, ensure full kill
        bp.hp = 0;
        if (part == BodyPartType::Head || part == BodyPartType::Thorax) {
            bodyParts[BodyPartType::Thorax].hp = 0;
            bodyParts[BodyPartType::Head].hp   = 0;
        }
        return;
    }
    bp.hp -= dmg;
    if (bp.hp < 0) bp.hp = 0;
}

double Combatant::calculateHitChance(BodyPartType targetPart) const {
    if (!weapon) return 0.0;

    // Base weapon accuracy
    double acc = weapon->getAccuracy();

    // If it’s a scoped Rifle, grant +40%
    if (weapon->getType() == WeaponType::Rifle && weapon->isScoped()) {
        acc += 0.40;
    }

    // Distance modifier: Close=×1.0, Medium=×0.7, Far=×0.4
    static constexpr double distMods[3] = {1.0, 0.7, 0.4};
    acc *= distMods[static_cast<int>(distance)];

    // Body part multiplier
    double partMod = 1.0;
    switch (targetPart) {
        case BodyPartType::Head:   partMod = 0.30; break;
        case BodyPartType::Thorax: partMod = 0.80; break;
        case BodyPartType::Arm:    partMod = 0.60; break;
        case BodyPartType::Leg:    partMod = 0.60; break;
    }
    acc *= partMod;

    // If target is in cover, cut chance in half
    if (inCover) {
        acc *= 0.5;
    }

    // Clamp to [0,1]
    if (acc < 0.0) acc = 0.0;
    if (acc > 1.0) acc = 1.0;
    return acc;
}

bool Combatant::shootAt(std::shared_ptr<Combatant> target, BodyPartType targetPart) {
    if (!weapon || weapon->needsReload()) {
        return false; // can’t shoot
    }
    if (!weapon->fireOne()) {
        return false; // out of ammo
    }

    double hitChance = calculateHitChance(targetPart);
    std::uniform_real_distribution<double> uni(0.0, 1.0);
    double roll = uni(rng);

    if (roll <= hitChance) {
        // Hit!
        int damage = weapon->getDamage();
        // If that part is already blacked out, force an instant kill
        if (target->bodyParts.at(targetPart).isBlackedOut()) {
            damage = 9999;
        }
        target->applyDamage(targetPart, damage);

        // If target was in cover, 50% chance we knock them out of cover
        if (target->inCover) {
            std::uniform_real_distribution<double> breaker(0.0, 1.0);
            if (breaker(rng) < 0.5) {
                target->breakCover();
            }
        }

        std::cout << name << " hits " << target->getName()
                  << " in the "
                  << (targetPart == BodyPartType::Head   ? "Head"
                      : targetPart == BodyPartType::Thorax ? "Thorax"
                      : targetPart == BodyPartType::Arm     ? "Arm"
                                                           : "Leg")
                  << " for " << damage << " damage!\n";
    } else {
        // Miss
        std::cout << name << " fires at " << target->getName() << " and misses.\n";
        // If target was in cover, 30% chance stray break cover
        if (target->inCover) {
            std::uniform_real_distribution<double> breaker(0.0, 1.0);
            if (breaker(rng) < 0.3) {
                target->breakCover();
                std::cout << target->getName() << "'s cover is destroyed by stray shots!\n";
            }
        }
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
    // Only allow flee if at Far distance and legs are not blacked out
    if (distance != Distance::Far) {
        std::cout << name << " can't flee unless you're far away!\n";
        return false;
    }
    if (bodyParts.at(BodyPartType::Leg).isBlackedOut()) {
        std::cout << name << " tries to flee but legs are useless!\n";
        return false;
    }
    // Otherwise 50% chance
    std::uniform_real_distribution<double> uni(0.0, 1.0);
    if (uni(rng) < 0.5) {
        std::cout << name << " successfully flees the combat!\n";
        return true;
    } else {
        std::cout << name << " attempts to flee but fails!\n";
        return false;
    }
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

    // Randomly assign a special stat:
    std::vector<SpecialStat> pool = {
        SpecialStat::None,
        SpecialStat::Armored,
        SpecialStat::Quick,
        SpecialStat::Sharpshooter,
        SpecialStat::Tank
    };
    std::uniform_int_distribution<size_t> pick(0, pool.size() - 1);
    special = pool[pick(rng)];

    // Equip a weapon based on type:
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

    // Enemies never use cover:
    inCover = false;
}

void Enemy::decideAction(std::shared_ptr<Combatant> player) {
    if (isDead()) return;

    // If out of ammo or needs reload, reload:
    if (weapon->needsReload() || weapon->getAmmo() == 0) {
        reloadWeapon();
        return;
    }

    // Always shoot; choose target body part based on what's not blacked out
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
    // Only allow flee if at Far distance and legs are not blacked out
    if (distance != Distance::Far) {
        std::cout << name << " can't flee unless you're far away!\n";
        return false;
    }
    if (bodyParts.at(BodyPartType::Leg).isBlackedOut()) {
        std::cout << name << " tries to flee but legs are gone!\n";
        return false;
    }
    return Combatant::attemptFlee();
}

void PlayerCombatant::displayStatus() const {
    // Display all limb HP on one line, separated by " | "
    std::cout << "\n=== " << name << " Status ===\n";
    std::cout << "Head: "   << bodyParts.at(BodyPartType::Head).hp   << "/" << bodyParts.at(BodyPartType::Head).maxHp   << " | "
              << "Thorax: " << bodyParts.at(BodyPartType::Thorax).hp << "/" << bodyParts.at(BodyPartType::Thorax).maxHp << " | "
              << "Arm: "    << bodyParts.at(BodyPartType::Arm).hp    << "/" << bodyParts.at(BodyPartType::Arm).maxHp    << " | "
              << "Leg: "    << bodyParts.at(BodyPartType::Leg).hp    << "/" << bodyParts.at(BodyPartType::Leg).maxHp    << "\n";
    std::cout << "Weapon: "
              << (weapon ? weapon->getName() : "None")
              << " [" << (weapon ? std::to_string(weapon->getAmmo()) + "/"
                                + std::to_string(weapon->getMaxAmmo())
                                : "N/A") << "]\n";
    std::cout << (inCover ? "Behind Cover" : "Exposed")
              << " | Distance: "
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
    // Display player status first (horizontal HP, cover state, distance)
    player.displayStatus();

    // Display each enemy on one line, with horizontal HP
    std::cout << "\n=== Enemies ===\n";
    for (size_t i = 0; i < enemies.size(); ++i) {
        auto& e = enemies[i];
        if (e->isDead()) {
            std::cout << i << ": " << e->getName() << " [DEAD]\n";
        } else {
            std::cout << i << ": " << e->getName()
                      << " | Head: "   << e->getHp(BodyPartType::Head)   << "/" << e->bodyParts.at(BodyPartType::Head).maxHp
                      << " | Th: "     << e->getHp(BodyPartType::Thorax) << "/" << e->bodyParts.at(BodyPartType::Thorax).maxHp
                      << " | A: "      << e->getHp(BodyPartType::Arm)    << "/" << e->bodyParts.at(BodyPartType::Arm).maxHp
                      << " | L: "      << e->getHp(BodyPartType::Leg)    << "/" << e->bodyParts.at(BodyPartType::Leg).maxHp
                      << (e->isInCover() ? " | Behind Cover" : " | Exposed")
                      << " | Dist: "   << (e->distance == Distance::Close  ? "C"
                                     : e->distance == Distance::Medium ? "M" : "F")
                      << "\n";
        }
    }
    std::cout << "===============\n";
}

bool CombatManager::engage(PlayerCombatant& player,
                           std::vector<std::shared_ptr<Enemy>>& enemies)
{
    std::cout << "\n--- Combat Start! ---\n";
    // Start at Far distance
    currentDistance = Distance::Far;
    player.inCover = false;
    player.distance = currentDistance;
    for (auto& e : enemies) {
        e->inCover = false;
        e->distance = currentDistance;
    }

    while (true) {
        if (!playerTurn(player, enemies)) {
            return !player.isDead();
        }
        if (allEnemiesDead(enemies)) {
            std::cout << "\nAll enemies are down. You survived!\n";
            return true;
        }

        if (!enemiesTurn(player, enemies)) {
            return false;
        }
        if (allEnemiesDead(enemies)) {
            std::cout << "\nAll enemies are down. You survived!\n";
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
    for (auto& e : enemies) {
        if (e->isDead()) continue;
        e->tick();
        // Use a shared_ptr to wrap the existing PlayerCombatant for targeting
        e->decideAction(std::make_shared<PlayerCombatant>(player));
        if (player.isDead()) return false;
    }
    return true;
}

bool CombatManager::promptPlayerAction(PlayerCombatant& player,
                                       std::vector<std::shared_ptr<Enemy>>& enemies)
{
    std::cout << "\nChoose an action:\n";
    std::cout << " 1) Move Closer   2) Move Further   3) Take Cover\n";
    std::cout << " 4) Shoot         5) Reload         6) Flee\n";
    std::cout << "Command> ";

    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "1" || cmd == "move closer") {
        bool wasInCover = player.isInCover();
        if (currentDistance != Distance::Close) {
            // Decrease universal distance
            currentDistance = static_cast<Distance>(static_cast<int>(currentDistance) - 1);
            // Moving breaks cover if you were behind cover
            if (wasInCover) {
                player.breakCover();
                std::cout << "You move closer and drop out of cover. You are now Exposed.\n";
            } else {
                std::cout << "You move closer.\n";
            }
            for (auto& e : enemies) {
                e->inCover = false;
                e->distance = currentDistance;
            }
            player.distance = currentDistance;
        } else {
            std::cout << "You are already at the closest range.\n";
        }
        return true;
    }
    if (cmd == "2" || cmd == "move further") {
        bool wasInCover = player.isInCover();
        if (currentDistance != Distance::Far) {
            // Increase universal distance
            currentDistance = static_cast<Distance>(static_cast<int>(currentDistance) + 1);
            // Moving breaks cover if you were behind cover
            if (wasInCover) {
                player.breakCover();
                std::cout << "You move farther and drop out of cover. You are now Exposed.\n";
            } else {
                std::cout << "You move farther.\n";
            }
            for (auto& e : enemies) {
                e->inCover = false;
                e->distance = currentDistance;
            }
            player.distance = currentDistance;
        } else {
            std::cout << "You are already at the farthest range.\n";
        }
        return true;
    }
    if (cmd == "3" || cmd == "take cover") {
        if (!player.isInCover()) {
            player.takeCover();
            std::cout << "You drop behind cover. You are now Behind Cover.\n";
        } else {
            std::cout << "You are already behind cover.\n";
        }
        return true;
    }
    if (cmd == "4" || cmd.rfind("shoot", 0) == 0) {
        std::istringstream iss(cmd);
        std::string word;
        iss >> word; // "shoot"
        int idx;
        std::string partStr;
        if (!(iss >> idx >> partStr)) {
            std::cout << "Usage: shoot <enemyIndex> <head|thorax|arm|leg>\n";
            return true;
        }
        if (idx < 0 || idx >= static_cast<int>(enemies.size()) || enemies[idx]->isDead()) {
            std::cout << "Invalid enemy index.\n";
            return true;
        }
        BodyPartType targetPart = parseBodyPart(partStr);
        auto enemyPtr = enemies[idx];
        if (!player.shootAt(enemyPtr, targetPart)) {
            std::cout << "Unable to shoot (no ammo or reloading).\n";
        }
        return true;
    }
    if (cmd == "5" || cmd == "reload") {
        player.reloadWeapon();
        return true;
    }
    if (cmd == "6" || cmd == "flee") {
        if (player.attemptFlee()) {
            return false; // player successfully fled → exit combat
        }
        return true;  // still fighting
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
