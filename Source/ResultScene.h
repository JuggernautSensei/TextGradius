#pragma once
#include "Scene.h"

class ResultScene : public Scene
{
public:
    using Scene::Scene;

    void     OnEnter() override;
    eSceneId Update() override;
};
