#pragma once
#include "Bullet.h"
#include "EnemyBullet.h"
#include "GameObject.h"
#include "Player.h"

class Enemy : public GameObject
{
public:
    void Render(Console& _console) const override;

    virtual void Update(Player& _player, Vector<Bullet>& _playerBullets) = 0;
    virtual void Fire(Vector<EnemyBullet>& _bulletPool, size_t& _cursor);

    [[nodiscard]] bool CollidesWithPlayer(const Player& _player) const;
    [[nodiscard]] bool ConsumePendingScore(int& _outScore);

protected:
    [[nodiscard]] virtual eColor GetLiveColor() const;

    void ResetCommon(int _life, int _score);
    void ApplyBulletHits(Vector<Bullet>& _playerBullets, int _damage);
    void RunDeathSequence();

    int  m_life        = 0;
    int  m_speed       = 0;
    int  m_score       = 0;
    int  m_dieEvent    = 0;
    int  m_patternTime = 0;
    int  m_enemyTime   = 0;
    bool m_bJustDied   = false;
};
