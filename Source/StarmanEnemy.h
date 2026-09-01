#pragma once
#include "Enemy.h"

class StarmanEnemy : public Enemy
{
public:
    void Spawn();
    void Update(Player& _player, Vector<Bullet>& _playerBullets) override;

protected:
    [[nodiscard]] eColor GetLiveColor() const override;

private:
    float m_fx       = 0.f;
    float m_fy       = 0.f;
    float m_vx       = 0.f;
    float m_vy       = 0.f;
    int   m_lifeTime = 0;
};
