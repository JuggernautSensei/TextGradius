#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
    void Spawn(int _x, int _y, int _speed, StringView _shape, eColor _color);
    void Update() override;

    void               MarkHit();
    [[nodiscard]] bool IsHit() const;

private:
    int  m_speed      = 0;
    int  m_effectTime = 0;
    bool m_bHit       = false;
};
