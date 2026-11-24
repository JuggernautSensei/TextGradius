#pragma once
#include "Scene.h"

class RankingScene : public Scene
{
public:
    using Scene::Scene;

    void     OnEnter() override;
    eSceneId Update() override;
};
