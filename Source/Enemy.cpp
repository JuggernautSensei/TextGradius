#include "Enemy.h"

void Enemy::ResetCommon(
    const int _life,
    const int _score)
{
    m_life        = _life;
    m_score       = _score;
    m_dieEvent    = 6;
    m_speed       = 0;
    m_patternTime = 0;
    m_enemyTime   = 0;
    m_bJustDied   = false;
    m_color       = eColor::LightRed;
    m_shape.clear();
    m_bAlive = true;
}

void Enemy::ApplyBulletHits(
    Vector<Bullet>& _playerBullets,
    const int       _damage)
{
    for (Bullet& bullet: _playerBullets)
    {
        if (!bullet.IsAlive() || bullet.IsHit())
        {
            continue;
        }
        if (bullet.GetX() <= m_x + 3 && bullet.GetX() >= m_x - 3 && bullet.GetY() <= m_y + 1 && bullet.GetY() >= m_y - 1)
        {
            bullet.MarkHit();
            m_life -= _damage;
        }
    }
}

void Enemy::RunDeathSequence()
{
    m_speed = 0;
    m_shape.clear();
    --m_dieEvent;
    if (m_dieEvent <= 0)
    {
        m_bJustDied = true;
        m_bAlive    = false;
    }
}

bool Enemy::CollidesWithPlayer(const Player& _player) const
{
    if (!m_bAlive || m_life <= 0)
    {
        return false;
    }
    return _player.GetX() >= m_x - 3 && _player.GetX() <= m_x + 3 && _player.GetY() == m_y;
}

bool Enemy::ConsumePendingScore(int& _outScore)
{
    if (!m_bJustDied)
    {
        return false;
    }
    m_bJustDied = false;
    _outScore   = m_score;
    return true;
}

void Enemy::Render(Console& _console) const
{
    if (!m_bAlive)
    {
        return;
    }

    if (m_life <= 0 && m_dieEvent > 0)
    {
        _console.PrintAt(m_x - 1, m_y - 1, std::to_string(m_score), eColor::LightRed);
        _console.PrintAt(m_x, m_y, "○", eColor::LightRed);
        return;
    }

    if (!m_shape.empty())
    {
        _console.PrintAt(m_x, m_y, m_shape, GetLiveColor());
    }
}

void Enemy::Fire(
    Vector<EnemyBullet>&,
    size_t&)
{
}

eColor Enemy::GetLiveColor() const
{
    return eColor::LightRed;
}
