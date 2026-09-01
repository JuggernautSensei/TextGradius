#include "Bullet.h"

#include "Config.h"

void Bullet::Spawn(
    const int        _x,
    const int        _y,
    const int        _speed,
    const StringView _shape,
    const eColor     _color)
{
    m_x          = _x;
    m_y          = _y;
    m_speed      = _speed;
    m_shape      = _shape;
    m_color      = _color;
    m_effectTime = kBulletEffectTicks;
    m_bHit       = false;
    m_bAlive     = true;
}

void Bullet::Update()
{
    if (!m_bAlive)
    {
        return;
    }

    if (m_x >= kFieldWidth - 4 || m_bHit)
    {
        m_speed = 0;
        m_shape = (m_effectTime == 3) ? "⊙" : (m_effectTime == 2) ? "◎"
                                                                  : "○";
        --m_effectTime;
        if (m_effectTime <= 0)
        {
            m_bAlive = false;
        }
        return;
    }

    m_x += m_speed;
}

void Bullet::MarkHit()
{
    m_bHit = true;
}

bool Bullet::IsHit() const
{
    return m_bHit;
}
