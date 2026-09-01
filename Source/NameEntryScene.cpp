#include "NameEntryScene.h"

#include <cctype>
#include <conio.h>

#include "Config.h"
#include "Embed.h"
#include "Game.h"

constexpr size_t kMaxNameLength = 10;

void NameEntryScene::OnEnter()
{
    m_input.clear();
    m_bDirty = true;
}

void NameEntryScene::Draw()
{
    Console& console = m_game.GetConsole();
    console.Clear();
    console.SetColor(eColor::White);
    console.PrintAt(0, 0, BuildNameEntryPrompt(m_input));
}

eSceneId NameEntryScene::Update()
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

    if (key == kKeyEnter)
    {
        if (m_input.empty())
        {
            return eSceneId::None;
        }
        m_game.PlayerName() = m_input;
        return eSceneId::ShipSelect;
    }
    if (key == kKeyBackspace)
    {
        if (!m_input.empty())
        {
            m_input.pop_back();
            m_bDirty = true;
        }
        return eSceneId::None;
    }
    if (std::isprint(key) && m_input.size() < kMaxNameLength)
    {
        m_input.push_back(static_cast<char>(key));
        m_bDirty = true;
    }

    return eSceneId::None;
}
