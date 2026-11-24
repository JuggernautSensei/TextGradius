#pragma once
#include "Embed.h"
#include "Scene.h"

class TitleScene : public Scene
{
public:
    using Scene::Scene;

    void     OnEnter() override;
    eSceneId Update() override;

private:
    void Draw();

    eMainMenuOption m_selected = eMainMenuOption::Start;
    bool            m_bDirty   = true;
};
