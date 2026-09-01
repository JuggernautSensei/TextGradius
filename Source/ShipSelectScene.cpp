#include "ShipSelectScene.h"

#include <conio.h>

#include "Config.h"
#include "Embed.h"
#include "Game.h"

void ShipSelectScene::OnEnter()
{
    m_selected = eShipType::Triangle;
    m_bDirty   = true;
}

void ShipSelectScene::Draw()
{
    Console& console = m_game.GetConsole();
    console.Clear();
    console.SetColor(eColor::White);
    console.PrintAt(0, 0, BuildShipSelect(m_game.PlayerName(), m_selected));
}

eSceneId ShipSelectScene::Update()
{
    if (m_bDirty)
    {
        Draw();
        m_bDirty = false;
    }

    if (!_kbhit())
    {
        return eSceneId::None;
    }

    const int key = _getch();
    switch (key)
    {
        case kKeyUp:
            if (m_selected != eShipType::Triangle)
            {
                m_selected = static_cast<eShipType>(static_cast<int>(m_selected) - 1);
                m_bDirty   = true;
            }
            break;
        case kKeyDown:
            if (m_selected != eShipType::Diamond)
            {
                m_selected = static_cast<eShipType>(static_cast<int>(m_selected) + 1);
                m_bDirty   = true;
            }
            break;
        case kKeyEnter:
            m_game.GetPlayer().Init(m_selected);
            m_game.StartNewRun();
            return eSceneId::StageIntro;
        default:
            break;
    }

    return eSceneId::None;
}
