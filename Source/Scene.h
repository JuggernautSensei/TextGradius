#pragma once

class Game;

enum class eSceneId
{
    None,
    Title,
    NameEntry,
    ShipSelect,
    StageIntro,
    Gameplay,
    Result,
    Ranking,
    Exit,
};

class Scene
{
public:
    explicit Scene(Game& _game);
    virtual ~Scene() = default;

    virtual void OnEnter();

    [[nodiscard]] virtual eSceneId Update() = 0;

protected:
    Game& m_game;
};
