#include "UrosEnemy.h"

#include "Config.h"
#include "Random.h"

void UrosEnemy::Spawn()
{
    ResetCommon(20, 100);
    m_x           = kFieldWidth - 4;
    m_y           = RandomRange(3, kFieldHeight - 3);
    m_patternTime = RandomRange(0, 10);
    m_enemyTime   = RandomRange(0, 20);
}

void UrosEnemy::Update(
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
    m_shape = "◐";
    m_speed = 1;

    if (m_patternTime % 4 == 0)
    {
        m_x -= m_speed;
        if (m_patternTime < 20)
        {
            m_y -= m_speed;
        }
        else if (m_patternTime < 40)
        {
            m_y += m_speed;
        }
        else
        {
            m_patternTime = 0;
        }
    }

    while (m_y < 3)
    {
        m_patternTime = 20;
        ++m_y;
    }
    while (m_y > kFieldHeight - 4)
    {
        m_patternTime = 0;
        --m_y;
    }
    if (m_x <= 2)
    {
        m_y = RandomRange(3, kFieldHeight - 3);
        m_x = kFieldWidth - 4;
    }

    ApplyBulletHits(_playerBullets, _player.GetDamage());
}

void UrosEnemy::Fire(
    Vector<EnemyBullet>& _bulletPool,
    size_t&              _cursor)
{
    if (m_life <= 0 || m_enemyTime % 20 != 0)
    {
        return;
    }
    SpawnPooledEnemyBullet(_bulletPool, _cursor, static_cast<float>(m_x - 2), static_cast<float>(m_y), -2.f, 0.f, "◈");
}
