#pragma once
#include "GameObject.h"
#include "Player.h"

class EnemyBullet : public GameObject
{
public:
    void Spawn(float _x, float _y, float _vx, float _vy, StringView _shape);
    void Update() override;

    void               Kill();
    [[nodiscard]] bool CollidesWithPlayer(const Player& _player) const;

private:
    float m_fx     = 0.f;
    float m_fy     = 0.f;
    float m_vx     = 0.f;
    float m_vy     = 0.f;
    bool  m_bDying = false;
};

void SpawnPooledEnemyBullet(Vector<EnemyBullet>& _pool, size_t& _cursor, float _x, float _y, float _vx, float _vy, StringView _shape);
