// File: Combat.cpp

#include "Combat.h"
#include <sstream>    // for std::istringstream
#include <iostream>   // for std::cout
#include <cmath>      // for std::ceil

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
        bodyParts[BodyPartType::Arm]    = BodyPart(200);
        bodyParts[BodyPartType::Leg]    = BodyPart(200);
    } else {
        bodyParts[BodyPartType::Head]   = BodyPart(50);
        bodyParts[BodyPartType::Thorax] = BodyPart(200);
        bodyParts[BodyPartType::Arm]    = BodyPart(200);
        bodyParts[BodyPartType::Leg]    = BodyPart(200);
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
    // Determine base percentage from distance and target part:
    int basePct = 0;
    switch (distance) {
        case Distance::Far:
            switch (targetPart) {
                case BodyPartType::Head:   basePct = 15; break;
                case BodyPartType::Thorax: basePct = 25; break;
                case BodyPartType::Arm:    basePct = 20; break;
                case BodyPartType::Leg:    basePct = 20; break;
            }
            break;
        case Distance::Medium:
            switch (targetPart) {
                case BodyPartType::Head:   basePct = 40; break;
                case BodyPartType::Thorax: basePct = 60; break;
                case BodyPartType::Arm:    basePct = 50; break;
                case BodyPartType::Leg:    basePct = 50; break;
            }
            break;
        case Distance::Close:
            switch (targetPart) {
                case BodyPartType::Head:   basePct = 70; break;
                case BodyPartType::Thorax: basePct = 90; break;
                case BodyPartType::Arm:    basePct = 80; break;
                case BodyPartType::Leg:    basePct = 80; break;
            }
            break;
    }

    // If shooter is the player and is in cover, reduce by 25% (round up)
    int finalPct = basePct;
    if (isPlayer && inCover) {
        finalPct = static_cast<int>(std::ceil(basePct * 0.75));
    }

    return static_cast<double>(finalPct) / 100.0;
}

bool Combatant::shootAt(std::shared_ptr<Combatant> target, BodyPartType targetPart) {
    if (!weapon || weapon->needsReload()) {
        return false; // can’t shoot
    }
    if (!weapon->fireOne()) {
        return false; // out of ammo
    }

    bool attackerIsPlayer = isPlayer;
    // Calculate raw hit chance
    double hitChance = calculateHitChance(targetPart);
    std::uniform_real_distribution<double> uni(0.0, 1.0);
    double roll = uni(rng);

    std::string attackerName = attackerIsPlayer ? "You" : name;
    std::string targetName   = target->isPlayer ? "you" : target->getName();
    bool targetWasInCover    = target->inCover;

    // If attacker is enemy and you are in cover:
    if (!attackerIsPlayer && target->inCover) {
        // Enemy still rolls to see if shot would have hit
        if (roll <= hitChance) {
            // 30% chance that this hitting shot breaks cover and hits you
            std::uniform_real_distribution<double> breaker(0.0, 1.0);
            if (breaker(rng) < 0.3) {
                // Break cover and apply damage
                target->breakCover();
                int damage = weapon->getDamage();
                target->applyDamage(targetPart, damage);
                std::cout << name << " shoots a bullet hitting your "
                          << (targetPart == BodyPartType::Head    ? "head"
                              : targetPart == BodyPartType::Thorax ? "thorax"
                              : targetPart == BodyPartType::Arm    ? "arm"
                                                                    : "leg")
                          << " and breaking your cover!\n";

                // Check brutal death on any zeroed part
                if (target->bodyParts.at(targetPart).hp == 0) {
                    // Force full death
                    target->bodyParts[BodyPartType::Head].hp = 0;
                    target->bodyParts[BodyPartType::Thorax].hp = 0;

                    if (target->isPlayer) {
                        // Player death description (not used for enemies)
                    }
                }
            } else {
                std::cout << name << " fires at you but you remain safely behind cover.\n";
            }
        } else {
            std::cout << name << " fires at you and misses completely.\n";
        }
        return true;
    }

    // Normal hit/miss flow (or player shooting)
    bool playerJustCoveredHit = false;
    if (roll <= hitChance && target->isPlayer) {
        PlayerCombatant* pc = static_cast<PlayerCombatant*>(target.get());
        if (pc->justTookCover) {
            playerJustCoveredHit = true;
            pc->justTookCover = false;
        }
    }

    if (roll <= hitChance) {
        int damage = weapon->getDamage();
        if (target->bodyParts.at(targetPart).isBlackedOut()) {
            damage = 9999; // overkill if already blacked out
        }
        target->applyDamage(targetPart, damage);

        if (playerJustCoveredHit) {
            std::cout << "You run to cover but get hit in the "
                      << (targetPart == BodyPartType::Head    ? "head"
                          : targetPart == BodyPartType::Thorax ? "thorax"
                          : targetPart == BodyPartType::Arm    ? "arm"
                                                                : "leg")
                      << " as you get behind cover.\n";
        } else {
            if (target->isPlayer) {
                std::cout << attackerName << " hits you in the "
                          << (targetPart == BodyPartType::Head    ? "head"
                              : targetPart == BodyPartType::Thorax ? "thorax"
                              : targetPart == BodyPartType::Arm    ? "arm"
                                                                    : "leg")
                          << ".\n";
            } else {
                std::cout << attackerName << " hits " << targetName << " in the "
                          << (targetPart == BodyPartType::Head    ? "head"
                              : targetPart == BodyPartType::Thorax ? "thorax"
                              : targetPart == BodyPartType::Arm    ? "arm"
                                                                    : "leg")
                          << ".\n";

                // If you hit an enemy while behind cover, force a guaranteed 1‐turn flank
                if (attackerIsPlayer && inCover) {
                    Enemy* ePtr = static_cast<Enemy*>(target.get());
                    // Only flank if enemy's leg is still functional
                    if (!ePtr->bodyParts.at(BodyPartType::Leg).isBlackedOut()) {
                        ePtr->flanking = true;
                        ePtr->flankCountdown = 1;  // one move to break cover
                    }
                }

                // Brutal death if that part hit zero
                if (target->bodyParts.at(targetPart).hp == 0) {
                    // Force full death
                    target->bodyParts[BodyPartType::Head].hp = 0;
                    target->bodyParts[BodyPartType::Thorax].hp = 0;

                    // Print a long, descriptive death message:
                    if (targetPart == BodyPartType::Head) {
                        std::cout << targetName << " reels back as the bullet explodes through their skull, "
                                     "blood spurting in a crimson arc. Their body goes limp, "
                                     "eyes staring blankly as they collapse, spine folding unnaturally. "
                                     "The crack of bone echoes, and a faint gurgle of blood spills from "
                                     "their parted lips before silence descends.\n";
                    } else if (targetPart == BodyPartType::Thorax) {
                        std::cout << targetName << " clutches at their chest as the round tears through lungs. "
                                     "They gasp desperately, froth bubbling at their mouth, crimson spray "
                                     "misting in the air. Each breath becomes a ragged gasp; ribs fracture "
                                     "with sickening cracks. They slump to a kneel, one hand pressed against "
                                     "the smoking wound, eyes rolling back as they cough up dark blood, "
                                     "choking in their final moments.\n";
                    } else if (targetPart == BodyPartType::Arm) {
                        std::cout << targetName << " howls as the bullet shreds their arm, bone "
                                     "splintering, muscle and sinew rent. They clutch the mangled limb, "
                                     "blood pouring in rivers down their side. Their knees buckle, body "
                                     "seizing in shock; they scream, clutching the stump, white with pain, "
                                     "tears mixing with sweat as they fall to the ground, arm twitching spasmodically.\n";
                    } else if (targetPart == BodyPartType::Leg) {
                        std::cout << targetName << " collapses instantly, leg severed by the round. "
                                     "They roar in agony, clawing at the stump as hot blood soaks the dirt. "
                                     "Their other foot scrabbles in a futile attempt to stand; they rock "
                                     "back and forth, screaming, bile rising as they choke on each breath. "
                                     "Their torso trembles violently, eyes widening as they slip into unconsciousness.\n";
                    }
                }
            }
        }
    } else {
        std::cout << attackerName << " fired at " << targetName << " and missed.\n";
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
        flankCountdown--;
        if (flankCountdown > 0) {
            std::cout << name << " is flanking...\n";
            return;
        } else {
            flanking = false;
            std::cout << name << " completes the flank maneuver and breaks your cover!\n";
            player->breakCover();
            return;
        }
    }

    // If you are in cover, 30% chance to start flanking this turn
    if (player->inCover) {
        std::uniform_real_distribution<double> uni(0.0, 1.0);
        if (uni(rng) < 0.3) {
            flanking = true;
            flankCountdown = 1;  // one move to break cover
            std::cout << name << " is attempting to flank you!\n";
            return;
        }
    }

    // Otherwise, proceed to shoot
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
    : Combatant(n, true), justTookCover(false)
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
    player.displayStatus();

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

            // Compute player’s actual hit-chances (cover considered)
            double dHead = player.calculateHitChance(BodyPartType::Head);
            double dThor = player.calculateHitChance(BodyPartType::Thorax);
            double dArm  = player.calculateHitChance(BodyPartType::Arm);
            double dLeg  = player.calculateHitChance(BodyPartType::Leg);

            int pctHead = static_cast<int>(std::ceil(dHead * 100));
            int pctThor = static_cast<int>(std::ceil(dThor * 100));
            int pctArm  = static_cast<int>(std::ceil(dArm  * 100));
            int pctLeg  = static_cast<int>(std::ceil(dLeg  * 100));

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
    std::shared_ptr<Combatant> pPtr(&player, [](Combatant*) {});
    this->playerPtr = pPtr;

    currentDistance = Distance::Far;
    player.inCover = false;
    player.distance = currentDistance;

    displayCombatants(player, enemies);

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
    // Clear the "justTookCover" flag unless we explicitly go into Take Cover
    player.justTookCover = false;

    while (true) {
        std::cout << "\nChoose an action:\n"
                  << " 1) Move Closer   2) Move Further   3) Take Cover\n"
                  << " 4) Shoot         5) Reload         6) Flee\n"
                  << "Command> ";

        std::string cmd;
        std::getline(std::cin, cmd);

        // 1) Move Closer
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
        // 2) Move Further
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
        // 3) Take Cover
        if (cmd == "3" || cmd == "take cover") {
            if (!player.isInCover()) {
                player.takeCover();
                player.justTookCover = true;
                std::cout << "You run to cover. You are now Behind Cover.\n";
            } else {
                std::cout << "You are already behind cover.\n";
            }
            return true;
        }
        // 4) Shoot
        if (cmd.rfind("shoot", 0) == 0) {
            std::istringstream iss(cmd);
            std::vector<std::string> tokens;
            std::string tok;
            while (iss >> tok) {
                tokens.push_back(tok);
            }

            int idx = 0;
            std::string partStr;

            // shoot <part>
            if (tokens.size() == 2) {
                partStr = tokens[1];
                if (enemies.size() > 1) {
                    std::cout << "Multiple enemies present—use: shoot <enemyIndex> <bodyPart>\n";
                    continue;  // reprompt
                }
            }
            // shoot <index> <part>
            else if (tokens.size() == 3) {
                try {
                    idx = std::stoi(tokens[1]);
                } catch (...) {
                    std::cout << "Invalid enemy index.\n";
                    continue;
                }
                partStr = tokens[2];
                if (idx < 0 || idx >= static_cast<int>(enemies.size()) || enemies[idx]->isDead()) {
                    std::cout << "Invalid enemy index.\n";
                    continue;
                }
            }
            else {
                std::cout << "Usage: shoot <part>    OR    shoot <enemyIndex> <part>\n";
                continue;
            }

            BodyPartType targetPart = parseBodyPart(partStr);
            auto enemyPtr = enemies[idx];
            if (!player.shootAt(enemyPtr, targetPart)) {
                std::cout << "Unable to shoot (no ammo or reloading).\n";
            }
            return true;
        }
        // 5) Reload
        if (cmd == "5" || cmd == "reload") {
            player.reloadWeapon();
            return true;
        }
        // 6) Flee
        if (cmd == "6" || cmd == "flee") {
            if (player.attemptFlee()) {
                return false;  // you fled: exit combat loop
            }
            return true;      // failed to flee, still your enemies' turn
        }

        // Invalid input — reprompt without enemy acting
        std::cout << "Unknown command. Try again.\n";
    }
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