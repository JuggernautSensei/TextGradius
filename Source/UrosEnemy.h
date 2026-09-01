#pragma once
#include "Enemy.h"

class UrosEnemy : public Enemy
{
public:
    void Spawn();
    void Update(Player& _player, Vector<Bullet>& _playerBullets) override;
    void Fire(Vector<EnemyBullet>& _bulletPool, size_t& _cursor) override;
};
