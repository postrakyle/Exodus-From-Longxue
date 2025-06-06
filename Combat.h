// File: Combat.h

#ifndef ZOORK_COMBAT_H
#define ZOORK_COMBAT_H

#include "Weapons.h"     // must define Weapon, WeaponType, WeaponFactory
#include "EnemyTypes.h"  // must define EnemyType
#include <map>
#include <memory>
#include <random>
#include <string>
#include <vector>

//
//  Pick a body part to aim at:
//
enum class BodyPartType { Head, Thorax, Arm, Leg };

//
//  Special “trait” an enemy (or player) might have:
//
enum class SpecialStat { None, Armored, Quick, Sharpshooter, Tank };

//
//  Distance from target, for hit‐chance modifiers:
//
enum class Distance { Close = 0, Medium = 1, Far = 2 };

//
//  BodyPart holds current & max HP for that limb/area:
//
struct BodyPart {
    int hp;
    int maxHp;
    BodyPart(int m = 0) : hp(m), maxHp(m) {}
    bool isBlackedOut() const { return hp <= 0; }
};

//
//  Base class for anything that can fight (player or enemy):
//
class Combatant {
public:
    // Constructor: name + whether “player‐controlled” or not
    Combatant(const std::string& n, bool isPlayerCtrl);
    virtual ~Combatant() = default;

    // Returns true if “dead” (Head or Thorax ≤ 0)
    bool isDead() const;

    // Inflict damage to a specific body part
    void applyDamage(BodyPartType part, int dmg);

    // Compute base chance to hit that part (0.0 .. 1.0)
    double calculateHitChance(BodyPartType targetPart) const;

    // Fire one shot at `target`. Returns false if no ammo or reloading.
    // Otherwise returns true (and prints “hit” or “miss” text).
    bool shootAt(std::shared_ptr<Combatant> target, BodyPartType targetPart);

    // Reload your weapon (if any)
    void reloadWeapon();

    // Attempt to flee (only if at Far distance, and if legs are not blacked‐out)
    virtual bool attemptFlee();

    // Equip a Weapon instance onto this Combatant
    void equipWeapon(std::shared_ptr<Weapon> w) { weapon = w; }

    // Accessors:
    std::shared_ptr<Weapon> getWeapon() const { return weapon; }
    std::string getName() const { return name; }
    bool isInCover() const { return inCover; }
    int getHp(BodyPartType part) const { return bodyParts.at(part).hp; }

    // In‐combat actions (made public so external code can invoke directly):
    void takeCover() { inCover = true; }
    void breakCover() { inCover = false; }
    void tick() {
        if (flanking && --flankCountdown <= 0) {
            inCover = false;
            flanking = false;
        }
    }

    // Expose bodyParts so UI/AI can check hp per limb:
    std::map<BodyPartType, BodyPart> bodyParts;

    // Public so other code (ZOOrkEngine, CombatManager) can directly read/write:
    bool inCover;
    bool flanking;
    int flankCountdown;
    Distance distance;

protected:
    // Initialize each body part’s HP
    void initBodyParts(bool isScav);

    std::string name;
    bool isPlayer;
    SpecialStat special;
    std::shared_ptr<Weapon> weapon;

    static std::mt19937 rng;
};

//
//  Enemy class: derives from Combatant, picks a random special stat and weapon.
//  Enemy::decideAction(...) is called each enemy turn during combat.
//
class Enemy : public Combatant {
public:
    explicit Enemy(EnemyType t);
    void decideAction(std::shared_ptr<Combatant> player);

private:
    EnemyType enemyType;
};

//
//  PlayerCombatant: wraps the real Player into a Combatant so we can run a fight.
//  Overwrites attemptFlee (can’t flee unless at Far distance, or if legs blacked‐out).
//  Also has a displayStatus().
//
class PlayerCombatant : public Combatant {
public:
    explicit PlayerCombatant(const std::string& n);

    bool attemptFlee() override;
    void displayStatus() const;
};

//
//  CombatManager: orchestrates a turn‐based loop between one PlayerCombatant
//  and a vector of Enemy instances. Returns true if player survives, false if dead.
//
class CombatManager {
public:
    bool engage(PlayerCombatant& player, std::vector<std::shared_ptr<Enemy>>& enemies);

private:
    bool allEnemiesDead(const std::vector<std::shared_ptr<Enemy>>& enemies) const;
    void displayCombatants(PlayerCombatant& player, const std::vector<std::shared_ptr<Enemy>>& enemies) const;
    bool playerTurn(PlayerCombatant& player, std::vector<std::shared_ptr<Enemy>>& enemies);
    bool enemiesTurn(PlayerCombatant& player, const std::vector<std::shared_ptr<Enemy>>& enemies);
    bool promptPlayerAction(PlayerCombatant& player, std::vector<std::shared_ptr<Enemy>>& enemies);
    BodyPartType parseBodyPart(const std::string& s) const;

    // Distance is universal between player and all enemies:
    Distance currentDistance;
};

#endif // ZOORK_COMBAT_H
