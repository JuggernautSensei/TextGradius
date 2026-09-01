#pragma once
#include "Scene.h"
#include "Typedef.h"

class NameEntryScene : public Scene
{
public:
    using Scene::Scene;

    void     OnEnter() override;
    eSceneId Update() override;

private:
    void Draw();

    String m_input;
    bool   m_bDirty = true;
};
