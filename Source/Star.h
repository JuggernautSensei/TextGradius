#pragma once
#include "GameObject.h"

class Star : public GameObject
{
public:
    void Init();
    void Update(int _playerX, int _playerY);

private:
    void Update() override;
};
