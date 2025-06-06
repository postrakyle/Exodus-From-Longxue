// ————————————————————————————————
// File: Weapons.h
// ————————————————————————————————
#ifndef WEAPONS_H
#define WEAPONS_H

#include <string>
#include <memory>
#include <random>

// ——————————————
// Four possible weapon types
// ——————————————
enum class WeaponType {
    Rifle,
    AssaultRifle,
    Shotgun,
    Pistol
};

// ——————————
// Base Weapon class
// ——————————
class Weapon {
public:
    explicit Weapon(WeaponType t);

    WeaponType getType()  const { return type; }
    const std::string& getName()     const { return name; }
    int                getDamage()   const { return baseDamage; }
    double             getAccuracy() const { return baseAccuracy; }
    int                getAmmo()     const { return ammo; }
    int                getMaxAmmo()  const { return maxAmmo; }
    bool               needsReload() const { return (ammo == 0) || reloading; }
    bool               isScoped()    const { return scoped; }

    // Fire a single shot. Returns true if a shot was consumed.
    bool fireOne();
    // Initiate reload sequence (refill ammo).
    void reload();
    // Toggle “scoped” state (for the bolt‐action rifle).
    void toggleScope();

protected:
    // Concrete subclasses will fill in 'name', 'baseDamage', 'baseAccuracy', 'maxAmmo', etc.
    WeaponType   type;
    std::string  name;
    int          baseDamage;
    double       baseAccuracy;
    int          ammo;
    int          maxAmmo;
    bool         reloading;
    bool         scoped;

    // After finishing reload, this helper refills the weapon’s ammo.
    void doReload();
};

// —————————————————
// Weapon factory stub
// —————————————————
struct WeaponFactory {
    static std::shared_ptr<Weapon> createWeapon(WeaponType type);
};

#endif // WEAPONS_H
