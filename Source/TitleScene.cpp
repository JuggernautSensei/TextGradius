#include "TitleScene.h"

#include <conio.h>

#include "Config.h"
#include "Game.h"

void TitleScene::OnEnter()
{
    m_selected = eMainMenuOption::Start;
    m_bDirty   = true;
}

void TitleScene::Draw()
{
    Console& console = m_game.GetConsole();
    console.Clear();
    console.SetColor(eColor::White);
    console.PrintAt(0, 0, BuildMainMenu(m_selected));
}

eSceneId TitleScene::Update()
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
            if (m_selected != eMainMenuOption::Start)
            {
                m_selected = static_cast<eMainMenuOption>(static_cast<int>(m_selected) - 1);
                m_bDirty   = true;
            }
            break;
        case kKeyDown:
            if (m_selected != eMainMenuOption::Exit)
            {
                m_selected = static_cast<eMainMenuOption>(static_cast<int>(m_selected) + 1);
                m_bDirty   = true;
            }
            break;
        case kKeyEnter:
            switch (m_selected)
            {
                case eMainMenuOption::Start:
                    return eSceneId::NameEntry;
                case eMainMenuOption::Ranking:
                    return eSceneId::Ranking;
                case eMainMenuOption::Exit:
                    return eSceneId::Exit;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return eSceneId::None;
}
