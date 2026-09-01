#include "GameObject.h"

void GameObject::Update()
{
}

void GameObject::Render(Console& _console) const
{
    if (!m_bAlive || m_shape.empty())
    {
        return;
    }
    _console.PrintAt(m_x, m_y, m_shape, m_color);
}

bool GameObject::IsAlive() const
{
    return m_bAlive;
}

void GameObject::SetAlive(const bool _bAlive)
{
    m_bAlive = _bAlive;
}

int GameObject::GetX() const
{
    return m_x;
}

int GameObject::GetY() const
{
    return m_y;
}

void GameObject::SetPosition(
    const int _x,
    const int _y)
{
    m_x = _x;
    m_y = _y;
}
