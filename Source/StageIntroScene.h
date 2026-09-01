#pragma once
#include "Scene.h"

class StageIntroScene : public Scene
{
public:
    using Scene::Scene;

    void     OnEnter() override;
    eSceneId Update() override;

private:
    void Draw();

    int m_ticksLeft     = 0;
    int m_lastCountdown = -1;
};
