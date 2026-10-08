#include "ResultScene.h"

#include "Config.h"
#include "Embed.h"
#include "Game.h"
#include "Input.h"
#include "RankingData.h"

void ResultScene::OnEnter()
{
    Console& console = m_game.GetConsole();
    console.Clear();
    console.SetColor(eColor::White);
    console.PrintAt(0, 0, BuildResultScreen(m_game.DidWin(), m_game.PlayerName(), m_game.Score()));
}

eSceneId ResultScene::Update()
{
    if (!_kbhit() || ReadKey() != kKeyEnter)
    {
        return eSceneId::None;
    }

    AppendRankingData(m_game.GetPlayerId(), m_game.PlayerName(), m_game.Score());
    return eSceneId::Title;
}
