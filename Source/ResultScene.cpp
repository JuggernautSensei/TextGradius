#include "ResultScene.h"

#include <conio.h>

#include "Config.h"
#include "Embed.h"
#include "Game.h"
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
    if (!_kbhit() || _getch() != kKeyEnter)
    {
        return eSceneId::None;
    }

    AppendRankingData(m_game.GetPlayerId(), m_game.PlayerName(), m_game.Score());
    return eSceneId::Exit;
}
