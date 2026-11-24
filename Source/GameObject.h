#pragma once
#include "Console.h"
#include "Typedef.h"

class GameObject
{
public:
    GameObject()          = default;
    virtual ~GameObject() = default;

    virtual void Update();
    virtual void Render(Console& _console) const;

    [[nodiscard]] bool IsAlive() const;
    void               SetAlive(bool _bAlive);

    [[nodiscard]] int GetX() const;
    [[nodiscard]] int GetY() const;
    void              SetPosition(int _x, int _y);

protected:
    int    m_x = 0;
    int    m_y = 0;
    String m_shape;
    eColor m_color  = eColor::White;
    bool   m_bAlive = false;
};
