//Weapons.cpp
#include "Weapons.h"
#include <iostream>

Weapon::Weapon(WeaponType t)
    : type(t), reloading(false), scoped(false)
{
    switch (type) {
        case WeaponType::Rifle:
            name         = "Rifle";
            baseDamage   = 80;        // single-shot bolt-action
            baseAccuracy = 0.60;      // 60% at “close”
            ammo         = maxAmmo = 5;
            break;

        case WeaponType::AssaultRifle:
            name         = "Assault Rifle";
            baseDamage   = 25;        // per bullet
            baseAccuracy = 0.75;
            ammo         = maxAmmo = 30;
            break;

        case WeaponType::Shotgun:
            name         = "Shotgun";
            baseDamage   = 60;      // damage to one targeted body part
            baseAccuracy = 0.50;
            ammo         = maxAmmo = 8;
            break;

        case WeaponType::Pistol:
            name         = "Pistol";
            baseDamage   = 50;
            baseAccuracy = 0.65;
            ammo         = maxAmmo = 15;
            break;
    }
}

bool Weapon::fireOne() {
    if (ammo <= 0) {
        std::cout << name << " is out of ammo and must reload!\n";
        return false;
    }
    ammo--;
    reloading = (ammo == 0);
    // After firing a scoped‐in rifle shot, toggle scope off
    if (type == WeaponType::Rifle && scoped) {
        scoped = false;
    }
    return true;
}

void Weapon::reload() {
    if (ammo == maxAmmo) {
        std::cout << name << " is already fully loaded.\n";
        return;
    }
    doReload();
    std::cout << "Reloading " << name << "... (" << maxAmmo << " rounds)\n";
    ammo = maxAmmo;
    reloading = false;
}

void Weapon::toggleScope() {
    if (type != WeaponType::Rifle) {
        std::cout << "Cannot scope with " << name << ".\n";
        return;
    }
    scoped = !scoped;
    std::cout << (scoped ? "Scoped in on Rifle.\n" : "Scoped out.\n");
}

void Weapon::doReload() {
    // Placeholder for reload animations or future AP‐cost logic
}

// ——————————————————————————————————————————————————————————
// WeaponFactory
// ——————————————————————————————————————————————————————————
std::shared_ptr<Weapon> WeaponFactory::createWeapon(WeaponType type) {
    return std::make_shared<Weapon>(type);
}
