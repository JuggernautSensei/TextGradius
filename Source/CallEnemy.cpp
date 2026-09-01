#include "CallEnemy.h"

#include "Config.h"
#include "Random.h"

void CallEnemy::Spawn()
{
    ResetCommon(20, 150);
    m_x           = kFieldWidth - 4;
    m_y           = RandomRange(3, kFieldHeight - 3);
    m_patternTime = 0;
    m_enemyTime   = RandomRange(10, 30);
}

void CallEnemy::Update(
    Player&         _player,
    Vector<Bullet>& _playerBullets)
{
    if (m_life <= 0)
    {
        RunDeathSequence();
        return;
    }

    ++m_enemyTime;
    ++m_patternTime;
    m_shape = "☎";
    m_speed = 1;

    if (m_patternTime % 2 == 0)
    {
        m_x -= m_speed;
    }
    if (m_x < 2)
    {
        m_y = RandomRange(3, kFieldHeight - 3);
        m_x = kFieldWidth - 4;
    }

    ApplyBulletHits(_playerBullets, _player.GetDamage());
}

void CallEnemy::Fire(
    Vector<EnemyBullet>& _bulletPool,
    size_t&              _cursor)
{
    if (m_life <= 0 || m_enemyTime % 30 != 0)
    {
        return;
    }

    const auto fx = static_cast<float>(m_x - 2);
    const auto fy = static_cast<float>(m_y);
    SpawnPooledEnemyBullet(_bulletPool, _cursor, fx, fy, -2.f, 0.f, "♬");
    SpawnPooledEnemyBullet(_bulletPool, _cursor, fx, fy, -1.72f, -0.5f, "♬");
    SpawnPooledEnemyBullet(_bulletPool, _cursor, fx, fy, -1.72f, 0.5f, "♬");
}
