#include "Game.h"

#include <ctime>
#include <Windows.h>

#include "Config.h"
#include "NameEntryScene.h"
#include "RankingScene.h"
#include "Random.h"
#include "ResultScene.h"
#include "ShipSelectScene.h"
#include "StageIntroScene.h"
#include "GameplayScene.h"
#include "TitleScene.h"

Game::Game() = default;

Game::~Game() = default;

void Game::StartNewRun()
{
    m_score        = 0;
    m_currentStage = 1;
    m_playerId     = static_cast<int>(time(nullptr));
}

Console& Game::GetConsole()
{
    return m_console;
}

Player& Game::GetPlayer()
{
    return m_player;
}

String& Game::PlayerName()
{
    return m_playerName;
}

int& Game::Score()
{
    return m_score;
}

int& Game::CurrentStage()
{
    return m_currentStage;
}

int Game::GetPlayerId() const
{
    return m_playerId;
}

void Game::SetResult(const bool _bWon)
{
    m_bWon = _bWon;
}

bool Game::DidWin() const
{
    return m_bWon;
}

UniquePtr<Scene> Game::CreateScene(const eSceneId _id)
{
    switch (_id)
    {
        case eSceneId::Title:
            return std::make_unique<TitleScene>(*this);
        case eSceneId::NameEntry:
            return std::make_unique<NameEntryScene>(*this);
        case eSceneId::ShipSelect:
            return std::make_unique<ShipSelectScene>(*this);
        case eSceneId::StageIntro:
            return std::make_unique<StageIntroScene>(*this);
        case eSceneId::Gameplay:
            return std::make_unique<GameplayScene>(*this);
        case eSceneId::Result:
            return std::make_unique<ResultScene>(*this);
        case eSceneId::Ranking:
            return std::make_unique<RankingScene>(*this);
        default:
            GRAD_ASSERT(false, "No scene registered for this id.\n");
            return std::make_unique<TitleScene>(*this);
    }
}

void Game::Run()
{
    RandomSeed();
    m_console.Init(kFieldWidth, kFieldHeight);

    m_scene = CreateScene(m_pendingScene);
    m_scene->OnEnter();

    while (true)
    {
        Sleep(kFrameDelayMs);

        const eSceneId next = m_scene->Update();
        m_console.Present();

        if (next == eSceneId::Exit)
        {
            break;
        }
        if (next != eSceneId::None)
        {
            m_scene = CreateScene(next);
            m_scene->OnEnter();
        }
    }
}
