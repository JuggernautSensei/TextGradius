#include "Star.h"

#include "Config.h"
#include "Random.h"

void Star::Init()
{
    m_x      = RandomRange(4, kFieldWidth - 4);
    m_y      = RandomRange(2, kFieldHeight - 2);
    m_shape  = "ㆍ";
    m_color  = eColor::DarkGray;
    m_bAlive = true;
}

void Star::Update(
    const int _playerX,
    const int _playerY)
{
    const bool bUnderPlayer = (m_x >= _playerX - 2 && m_x <= _playerX + 2 && m_y == _playerY);
    m_shape                 = bUnderPlayer ? "" : "ㆍ";

    if (m_x < 2)
    {
        m_x = kFieldWidth - 2;
        m_y = RandomRange(1, kFieldHeight - 1);
    }
    --m_x;
}

void Star::Update()
{
}
