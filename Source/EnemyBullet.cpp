#include "EnemyBullet.h"

#include "Config.h"

void EnemyBullet::Spawn(
    const float      _x,
    const float      _y,
    const float      _vx,
    const float      _vy,
    const StringView _shape)
{
    m_fx     = _x;
    m_fy     = _y;
    m_vx     = _vx;
    m_vy     = _vy;
    m_shape  = _shape;
    m_color  = eColor::Yellow;
    m_bDying = false;
    m_bAlive = true;
    m_x      = static_cast<int>(m_fx);
    m_y      = static_cast<int>(m_fy);
}

void EnemyBullet::Kill()
{
    m_bDying = true;
    m_shape  = "○";
}

void EnemyBullet::Update()
{
    if (!m_bAlive)
    {
        return;
    }

    if (m_bDying)
    {
        m_bAlive = false;
        return;
    }

    m_fx += m_vx;
    m_fy += m_vy;
    m_x = static_cast<int>(m_fx);
    m_y = static_cast<int>(m_fy);

    if (m_fx <= 3.f || m_fx >= kFieldWidth - 3.f || m_fy <= 2.f || m_fy >= kFieldHeight - 4.f)
    {
        Kill();
    }
}

bool EnemyBullet::CollidesWithPlayer(const Player& _player) const
{
    if (!m_bAlive || m_bDying)
    {
        return false;
    }
    return _player.GetX() >= m_x - 3 && _player.GetX() <= m_x + 3 && _player.GetY() == m_y;
}

void SpawnPooledEnemyBullet(
    Vector<EnemyBullet>& _pool,
    size_t&              _cursor,
    const float          _x,
    const float          _y,
    const float          _vx,
    const float          _vy,
    const StringView     _shape)
{
    _pool[_cursor].Spawn(_x, _y, _vx, _vy, _shape);
    _cursor = (_cursor + 1) % _pool.size();
}
