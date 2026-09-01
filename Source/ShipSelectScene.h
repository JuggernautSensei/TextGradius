#pragma once
#include "Player.h"
#include "Scene.h"

class ShipSelectScene : public Scene
{
public:
    using Scene::Scene;

    void     OnEnter() override;
    eSceneId Update() override;

private:
    void Draw();

    eShipType m_selected = eShipType::Triangle;
    bool      m_bDirty   = true;
};
