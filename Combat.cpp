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

    // If the shooter is the player and they are in cover, reduce accuracy by 25%
    if (isPlayer && inCover) {
        acc *= 0.75;
    }

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

    // Simplified names for printing
    std::string attackerName = isPlayer ? "You" : name;
    std::string targetName   = target->isPlayer ? "you" : target->getName();

    if (roll <= hitChance) {
        int damage = weapon->getDamage();
        if (target->bodyParts.at(targetPart).isBlackedOut()) {
            damage = 9999; // overkill if already blacked out
        }
        target->applyDamage(targetPart, damage);

        if (target->inCover) {
            // 50% chance to break cover when hit
            std::uniform_real_distribution<double> breaker(0.0, 1.0);
            if (breaker(rng) < 0.5) {
                target->breakCover();
                std::cout << targetName << "'s cover is broken!\n";
            }
        }

        // Print only hit location
        std::string partStr = (targetPart == BodyPartType::Head    ? "head"
                            : targetPart == BodyPartType::Thorax  ? "thorax"
                            : targetPart == BodyPartType::Arm     ? "arm"
                                                                    : "leg");
        std::cout << attackerName << " hit " << targetName << " in the " << partStr << ".\n";
    } else {
        std::cout << attackerName << " fired at " << targetName << " and missed.\n";
        if (target->inCover) {
            // 50% chance to break cover on a miss
            std::uniform_real_distribution<double> breaker(0.0, 1.0);
            if (breaker(rng) < 0.5) {
                target->breakCover();
                std::cout << targetName << "'s cover is broken by stray shots!\n";
            }
        }
    }
    return true;
}

void Combatant::reloadWeapon() {
    if (weapon) {
        std::string actor = isPlayer ? "You" : name;
        std::cout << actor << " reloads the " << weapon->getName() << ".\n";
        weapon->reload();
    }
}

bool Combatant::attemptFlee() {
    if (distance != Distance::Far) {
        std::string actor = isPlayer ? "You" : name;
        std::cout << actor << " can't flee unless you're far away!\n";
        return false;
    }
    if (bodyParts.at(BodyPartType::Leg).isBlackedOut()) {
        std::string actor = isPlayer ? "You" : name;
        std::cout << actor << " tries to flee but legs are useless!\n";
        return false;
    }
    std::uniform_real_distribution<double> uni(0.0, 1.0);
    if (uni(rng) < 0.5) {
        std::string actor = isPlayer ? "You" : name;
        std::cout << actor << " successfully flees the combat!\n";
        return true;
    } else {
        std::string actor = isPlayer ? "You" : name;
        std::cout << actor << " attempts to flee but fails!\n";
        return false;
    }
}

//
//  Enemy implementation
//

Enemy::Enemy(EnemyType t)
    : Combatant(
          (t == EnemyType::Scav
             ? "Scavenger"
             : (t == EnemyType::PMC_Chinese ? "PMC (C)" : "PMC (J)")),
          false
      ),
      enemyType(t)
{
    bool isScav = (t == EnemyType::Scav);
    initBodyParts(isScav);

    std::vector<SpecialStat> pool = {
        SpecialStat::None,
        SpecialStat::Armored,
        SpecialStat::Quick,
        SpecialStat::Sharpshooter,
        SpecialStat::Tank
    };
    std::uniform_int_distribution<size_t> pick(0, pool.size() - 1);
    special = pool[pick(rng)];

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

    inCover = false;
    flanking = false;
    flankCountdown = 0;
}

void Enemy::decideAction(std::shared_ptr<Combatant> player) {
    if (isDead()) return;

    // If currently flanking, decrement countdown
    if (flanking) {
        if (--flankCountdown > 0) {
            std::cout << name << " is flanking...\n";
            return;
        } else {
            flanking = false;
            std::cout << name << " completes the flank maneuver and breaks your cover!\n";
            player->breakCover();
            // After breaking cover, enemy’s turn ends here
            return;
        }
    }

    // Only attempt to flank if you (the player) are in cover
    if (player->inCover) {
        std::uniform_real_distribution<double> uni(0.0, 1.0);
        if (uni(rng) < 0.4) {
            flanking = true;
            flankCountdown = 2;
            std::cout << name << " is attempting to flank you!\n";
            return;
        }
    }

    // Otherwise, proceed to shoot normally
    if (weapon->needsReload() || weapon->getAmmo() == 0) {
        reloadWeapon();
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
    if (distance != Distance::Far) {
        std::cout << "You can't flee unless you're far away!\n";
        return false;
    }
    if (bodyParts.at(BodyPartType::Leg).isBlackedOut()) {
        std::cout << "You try to flee but legs are gone!\n";
        return false;
    }
    return Combatant::attemptFlee();
}

void PlayerCombatant::displayStatus() const {
    std::cout << "\n=== You Status ===\n";
    std::cout << "Head: "   << bodyParts.at(BodyPartType::Head).hp   << "/" << bodyParts.at(BodyPartType::Head).maxHp   << "  |  "
              << "Thorax: " << bodyParts.at(BodyPartType::Thorax).hp << "/" << bodyParts.at(BodyPartType::Thorax).maxHp << "  |  "
              << "Arm: "    << bodyParts.at(BodyPartType::Arm).hp    << "/" << bodyParts.at(BodyPartType::Arm).maxHp    << "  |  "
              << "Leg: "    << bodyParts.at(BodyPartType::Leg).hp    << "/" << bodyParts.at(BodyPartType::Leg).maxHp    << "\n";

    std::cout << "Weapon: "
              << (weapon ? weapon->getName() : "None")
              << " [" << (weapon ? std::to_string(weapon->getAmmo()) + "/"
                                + std::to_string(weapon->getMaxAmmo())
                                : "N/A") << "]  |  "
              << "Distance: "
              << (distance == Distance::Close  ? "Close"
                 : distance == Distance::Medium ? "Medium"
                                                : "Far")
              << "  |  "
              << (inCover ? "Cover: Behind Cover" : "Cover: Exposed")
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

void CombatManager::displayCombatants(
    PlayerCombatant& player,
    const std::vector<std::shared_ptr<Enemy>>& enemies
) const {
    // Show player status
    player.displayStatus();

    // Show all enemies with their Head/Thorax/Arm/Leg HP
    std::cout << "\n=== Enemies ===\n";
    for (size_t i = 0; i < enemies.size(); ++i) {
        auto& e = enemies[i];
        if (e->isDead()) {
            std::cout << i << ": " << e->getName() << "  |  Dead\n";
        } else {
            int headHp   = e->bodyParts.at(BodyPartType::Head).hp;
            int headMax  = e->bodyParts.at(BodyPartType::Head).maxHp;
            int thorHp   = e->bodyParts.at(BodyPartType::Thorax).hp;
            int thorMax  = e->bodyParts.at(BodyPartType::Thorax).maxHp;
            int armHp    = e->bodyParts.at(BodyPartType::Arm).hp;
            int armMax   = e->bodyParts.at(BodyPartType::Arm).maxHp;
            int legHp    = e->bodyParts.at(BodyPartType::Leg).hp;
            int legMax   = e->bodyParts.at(BodyPartType::Leg).maxHp;

            std::cout << i << ": " << e->getName() << "  |  "
                      << "Head: " << headHp   << "/" << headMax   << "  |  "
                      << "Th: "   << thorHp   << "/" << thorMax   << "  |  "
                      << "A: "    << armHp    << "/" << armMax    << "  |  "
                      << "L: "    << legHp    << "/" << legMax    << "\n";

            // VATS-style fixed probabilities based on distance
            int pctHead, pctThor, pctArm, pctLeg;
            switch (player.distance) {
                case Distance::Far:
                    pctHead = 15;
                    pctThor = 25;
                    pctArm  = 20;
                    pctLeg  = 20;
                    break;
                case Distance::Medium:
                    pctHead = 40;
                    pctThor = 60;
                    pctArm  = 50;
                    pctLeg  = 50;
                    break;
                case Distance::Close:
                    pctHead = 70;
                    pctThor = 90;
                    pctArm  = 80;
                    pctLeg  = 80;
                    break;
                default:
                    pctHead = pctThor = pctArm = pctLeg = 0;
            }

            std::cout << "    Probabilities -> "
                      << "H: "  << pctHead << "%  |  "
                      << "T: "  << pctThor << "%  |  "
                      << "A: "  << pctArm  << "%  |  "
                      << "L: "  << pctLeg  << "%\n";
        }
    }
    std::cout << "===============\n";
}

bool CombatManager::engage(
    PlayerCombatant& player,
    std::vector<std::shared_ptr<Enemy>>& enemies
) {
    // Create a non‐owning shared_ptr to the real player so enemies can damage it directly
    std::shared_ptr<Combatant> pPtr(&player, [](Combatant*) {});
    this->playerPtr = pPtr;

    currentDistance = Distance::Far;
    player.inCover = false;
    player.distance = currentDistance;

    // At the very start, show the initial UI:
    displayCombatants(player, enemies);

    while (true) {
        // 1) Player’s turn
        if (!playerTurn(player, enemies)) {
            // If player fled or died, just return false/true and let caller print once
            return !player.isDead();
        }
        if (allEnemiesDead(enemies)) {
            std::cout << "\nAll enemies are down. You survived!\n";
            return true;
        }

        // 2) Enemy’s turn
        if (!enemiesTurn(player, enemies)) {
            // Player died; do NOT print “You have been killed…” here.
            return false;
        }
        if (allEnemiesDead(enemies)) {
            std::cout << "\nAll enemies are down. You survived!\n";
            return true;
        }

        // 3) After both sides acted, show updated UI
        displayCombatants(player, enemies);
    }
}

bool CombatManager::playerTurn(
    PlayerCombatant& player,
    std::vector<std::shared_ptr<Enemy>>& enemies
) {
    player.tick();
    return promptPlayerAction(player, enemies);
}

bool CombatManager::enemiesTurn(
    PlayerCombatant& player,
    const std::vector<std::shared_ptr<Enemy>>& enemies
) {
    for (auto& e : enemies) {
        if (e->isDead()) continue;
        e->tick();
        e->decideAction(this->playerPtr);
        if (player.isDead()) {
            return false;
        }
    }
    return true;
}

bool CombatManager::promptPlayerAction(
    PlayerCombatant& player,
    std::vector<std::shared_ptr<Enemy>>& enemies
) {
    std::cout << "\nChoose an action:\n";
    std::cout << " 1) Move Closer   2) Move Further   3) Take Cover\n";
    std::cout << " 4) Shoot         5) Reload         6) Flee\n";
    std::cout << "Command> ";

    std::string cmd;
    std::getline(std::cin, cmd);

    if (cmd == "1" || cmd == "move closer") {
        bool wasInCover = player.isInCover();
        if (currentDistance != Distance::Close) {
            currentDistance = static_cast<Distance>(static_cast<int>(currentDistance) - 1);
            player.distance = currentDistance;
            if (wasInCover) {
                player.breakCover();
                std::cout << "You move closer and drop out of cover. You are now Exposed.\n";
            } else {
                std::cout << "You move closer.\n";
            }
        } else {
            std::cout << "You are already at the closest range.\n";
        }
        return true;
    }
    if (cmd == "2" || cmd == "move further") {
        bool wasInCover = player.isInCover();
        if (currentDistance != Distance::Far) {
            currentDistance = static_cast<Distance>(static_cast<int>(currentDistance) + 1);
            player.distance = currentDistance;
            if (wasInCover) {
                player.breakCover();
                std::cout << "You move farther and drop out of cover. You are now Exposed.\n";
            } else {
                std::cout << "You move farther.\n";
            }
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
    if (cmd.rfind("shoot", 0) == 0) {
        std::istringstream iss(cmd);
        std::vector<std::string> tokens;
        std::string tok;
        while (iss >> tok) {
            tokens.push_back(tok);
        }

        int idx = 0;
        std::string partStr;

        if (tokens.size() == 2) {
            partStr = tokens[1];
            if (enemies.size() > 1) {
                std::cout << "Multiple enemies present—use: shoot <enemyIndex> <bodyPart>\n";
                return true;
            }
        } else if (tokens.size() == 3) {
            try {
                idx = std::stoi(tokens[1]);
            } catch (...) {
                std::cout << "Invalid index.\n";
                return true;
            }
            partStr = tokens[2];
            if (idx < 0 || idx >= static_cast<int>(enemies.size()) || enemies[idx]->isDead()) {
                std::cout << "Invalid enemy index.\n";
                return true;
            }
        } else {
            std::cout << "Usage: shoot <part>    OR    shoot <enemyIndex> <part>\n";
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
            return false; // exit combat loop (you fled)
        }
        return true;
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
