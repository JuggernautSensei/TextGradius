#include "StageIntroScene.h"

#include "Config.h"
#include "Embed.h"
#include "Game.h"
#include "Input.h"

constexpr int kIntroSeconds = 5;

void StageIntroScene::OnEnter()
{
    m_ticksLeft     = kIntroSeconds * kFps;
    m_lastCountdown = -1;
}

void StageIntroScene::Draw()
{
    const int countdown = (m_ticksLeft + kFps - 1) / kFps;
    if (countdown == m_lastCountdown)
    {
        return;
    }
    m_lastCountdown = countdown;

    const int score = (m_game.CurrentStage() >= 2) ? m_game.Score() : -1;

    Console& console = m_game.GetConsole();
    console.Clear();
    console.SetColor(eColor::White);
    console.PrintAt(0, 0, BuildMissionBrief(m_game.CurrentStage(), countdown, score));
}

eSceneId StageIntroScene::Update()
{
    Draw();

    if (_kbhit() && ReadKey() == kKeyEnter)
    {
        return eSceneId::Gameplay;
    }

    --m_ticksLeft;
    if (m_ticksLeft <= 0)
    {
        return eSceneId::Gameplay;
    }

    return eSceneId::None;
}
