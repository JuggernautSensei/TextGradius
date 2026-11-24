#pragma once
#include "Console.h"
#include "Player.h"
#include "Scene.h"
#include "Typedef.h"

class Game
{
public:
    Game();
    ~Game();

    void Run();

    [[nodiscard]] Console& GetConsole();
    [[nodiscard]] Player&  GetPlayer();

    [[nodiscard]] String& PlayerName();
    [[nodiscard]] int&    Score();
    [[nodiscard]] int&    CurrentStage();
    [[nodiscard]] int     GetPlayerId() const;

    void StartNewRun();

    void               SetResult(bool _bWon);
    [[nodiscard]] bool DidWin() const;

private:
    [[nodiscard]] UniquePtr<Scene> CreateScene(eSceneId _id);

    Console          m_console;
    UniquePtr<Scene> m_scene;
    eSceneId         m_pendingScene = eSceneId::Title;

    Player m_player;
    String m_playerName;
    int    m_score        = 0;
    int    m_playerId     = 0;
    int    m_currentStage = 1;
    bool   m_bWon         = false;
};
