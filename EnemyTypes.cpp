// ---------------------------------
// File: EnemyFactory.h
// ---------------------------------
#ifndef ZOORK_ENEMY_FACTORY_H
#define ZOORK_ENEMY_FACTORY_H

#include "Combat.h"       // Defines Enemy, EnemyType
#include <memory>

/// Simple helper: spawn a new Enemy of the given type.
inline std::shared_ptr<Enemy> createEnemy(EnemyType type) {
    return std::make_shared<Enemy>(type);
}

#endif // ZOORK_ENEMY_FACTORY_H
