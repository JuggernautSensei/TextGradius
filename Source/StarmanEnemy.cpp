#include "StarmanEnemy.h"

#include "Config.h"
#include "Random.h"

void StarmanEnemy::Spawn()
{
    ResetCommon(20, 200);
    m_x        = kFieldWidth - 4;
    m_y        = RandomRange(3, kFieldHeight - 3);
    m_fx       = static_cast<float>(m_x);
    m_fy       = static_cast<float>(m_y);
    m_vx       = 0.f;
    m_vy       = 0.f;
    m_lifeTime = 40;
}

void StarmanEnemy::Update(
    Player&         _player,
    Vector<Bullet>& _playerBullets)
{
    if (m_life <= 0)
    {
        RunDeathSequence();
        return;
    }

    --m_lifeTime;
    ++m_patternTime;
    m_shape = "★";

    if (_player.GetX() < m_x && m_vx > -2.f)
    {
        m_vx -= 0.4f;
    }
    else if (_player.GetX() > m_x && m_vx < 2.f)
    {
        m_vx += 0.4f;
    }
    if (_player.GetY() < m_y && m_vy > -1.f)
    {
        m_vy -= 0.2f;
    }
    else if (_player.GetY() > m_y && m_vy < 1.f)
    {
        m_vy += 0.2f;
    }

    m_fx += m_vx;
    m_fy += m_vy;
    while (m_fy < 3.f)
    {
        m_fy += 1.f;
    }
    while (m_fy > kFieldHeight - 3.f)
    {
        m_fy -= 1.f;
    }
    m_x = static_cast<int>(m_fx);
    m_y = static_cast<int>(m_fy);

    if (m_lifeTime > 0)
    {
        if (m_x <= 2 || m_x > kFieldWidth - 2)
        {
            m_lifeTime = 0;
        }

        const bool bTouchingPlayer = _player.GetX() <= m_x + 3 && _player.GetX() >= m_x - 3 && _player.GetY() >= m_y - 1 && _player.GetY() <= m_y + 1;
        if (!_player.IsInvincible() && bTouchingPlayer)
        {
            _player.TakeDamage();
            m_score    = 10;
            m_lifeTime = 0;
        }
    }

    ApplyBulletHits(_playerBullets, _player.GetDamage());

    if (m_lifeTime <= 0 && m_life > 0)
    {
        m_life = 0;
    }
}

eColor StarmanEnemy::GetLiveColor() const
{
    return eColor::LightMagenta;
}
