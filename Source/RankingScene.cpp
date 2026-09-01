#include "RankingScene.h"

#include <conio.h>

#include "Config.h"
#include "Embed.h"
#include "Game.h"
#include "RankingData.h"

void RankingScene::OnEnter()
{
    Console& console = m_game.GetConsole();
    console.Clear();
    console.SetColor(eColor::White);

    Vector<RankEntry> entries;
    if (LoadRankingData(entries))
    {
        console.PrintAt(0, 0, BuildRankingTable(entries));
    }
    else
    {
        console.PrintAt(0, 10, BuildNoSaveDataBox());
    }
}

eSceneId RankingScene::Update()
{
    if (_kbhit() && _getch() == kKeyEnter)
    {
        return eSceneId::Title;
    }
    return eSceneId::None;
}
