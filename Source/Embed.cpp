#include "Embed.h"

#include <algorithm>
#include <format>
#include <sstream>

struct DisplayBars
{
    int durability = 0;
    int speed      = 0;
    int power      = 0;
    int rateOfFire = 0;
};

[[nodiscard]] static const DisplayBars& GetDisplayBars(const eShipType _type)
{
    static const DisplayBars kTriangle { 5, 3, 4, 3 };
    static const DisplayBars kClover { 3, 4, 3, 5 };
    static const DisplayBars kDiamond { 4, 3, 5, 3 };

    switch (_type)
    {
        case eShipType::Triangle:
            return kTriangle;
        case eShipType::Clover:
            return kClover;
        case eShipType::Diamond:
        default:
            return kDiamond;
    }
}

[[nodiscard]] static String Bar(const int _count)
{
    String result;
    for (int i = 0; i < _count; ++i)
    {
        result += "■ ";
    }
    return result;
}

String BuildMainMenu(const eMainMenuOption _selected)
{
    std::ostringstream out;
    out << kTitleArt << '\n';
    out << (_selected == eMainMenuOption::Start ? "\t\t\t\t\t>  GAME START\n" : "\t\t\t\t\t   GAME START\n");
    out << '\n';
    out << (_selected == eMainMenuOption::Ranking ? "\t\t\t\t\t>   RANKING\n" : "\t\t\t\t\t    RANKING\n");
    out << '\n';
    out << (_selected == eMainMenuOption::Exit ? "\t\t\t\t\t>    EXIT\n" : "\t\t\t\t\t     EXIT\n");
    out << "\n\n";
    out << kCopyright << '\n';
    return out.str();
}

String BuildNameEntryPrompt(const StringView _currentInput)
{
    std::ostringstream out;
    out << "\n";
    out << "     ==================================================     \n";
    out << "               플레이어의 이름을 기록해주세요!     \n";
    out << "     ==================================================     \n";
    out << "\n\n";
    out << "    >> " << _currentInput << "▏\n";
    return out.str();
}

String BuildShipSelect(
    const StringView _playerName,
    const eShipType  _selected)
{
    static constexpr StringView kNames[] = { "Triangle", "Clover", "Diamond" };
    static constexpr StringView kMarks[] = { "▶", "♣", "◆" };

    std::ostringstream out;
    out << "    플레이어의 이름: " << _playerName << "\n\n";
    out << "   ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n";

    for (int i = 0; i < 3; ++i)
    {
        const auto  type      = static_cast<eShipType>(i);
        const bool  bSelected = (type == _selected);
        const char* cursor    = bSelected ? "→ " : "   ";
        out << std::format("   ▒   {}{} {:<9}                                                                     ▒\n", cursor, kMarks[i], kNames[i]);
    }

    out << "   ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n\n";

    const DisplayBars& bars = GetDisplayBars(_selected);
    out << "    기체 이름: " << kNames[static_cast<int>(_selected)] << "\n\n";
    out << "    내구도: " << Bar(bars.durability) << "\n";
    out << "    스피드: " << Bar(bars.speed) << "\n";
    out << "    파괴력: " << Bar(bars.power) << "\n";
    out << "    연사력: " << Bar(bars.rateOfFire) << "\n";
    return out.str();
}

String BuildMissionBrief(
    const int _stage,
    const int _countdown,
    const int _score)
{
    std::ostringstream out;
    out << "\n\n\n\n\n\n\n\n\n";
    out << std::format("                                           Stage {}\n\n", _stage);
    out << "                     --------------------------------------------------------\n\n";
    out << "                                         Mission Goal\n\n";
    out << "                                   적을 최대한 많이 제거하고\n\n";
    out << "                              죽지않고 최종 목적지까지 도달하라!\n\n\n";
    if (_score >= 0)
    {
        out << std::format("                                  현재 플레이어의 점수: {}\n\n\n", _score);
    }
    out << std::format("                                      {}초 후 게임 시작...", _countdown);
    return out.str();
}

String BuildResultScreen(
    const bool       _bCleared,
    const StringView _playerName,
    const int        _score)
{
    std::ostringstream out;
    out << "\n\n\n\n\n\n\n\n\n\n";
    out << (_bCleared ? "                                        All Stage Clear!!\n\n"
                      : "                                             You Die...\n\n");
    out << "                          -----------------------------------------------\n\n";
    out << std::format("                                        Player Name: {}\n\n", _playerName);
    out << std::format("                                        Your Score: {}\n\n", _score);
    out << "                                   Enter를 누르면 종료됩니다..";
    return out.str();
}

String BuildRankingTable(const Vector<RankEntry>& _entries)
{
    std::ostringstream out;
    out << "\n\n\n";
    out << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n\n\n\n";
    out << "\t순위 \tPlayerID\t플레이어 이름\t\t점수\t\t\t\n";

    const size_t shown = std::min<size_t>(_entries.size(), 10);
    for (size_t i = 0; i < shown; ++i)
    {
        const RankEntry& entry = _entries[i];
        out << std::format("{:9} 위 {:11} {:>16}{:20}\n", i + 1, entry.id, entry.name, entry.score);
    }

    out << "\n\n";
    out << "                 주의!: 랭킹은 10위 까지만 표시됩니다!                \n\n\n";
    out << "                                ENTER                                 \n\n";
    out << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";
    return out.str();
}

String BuildHudHeader(
    const StringView _playerName,
    const int        _life,
    const int        _maxLife,
    const int        _score,
    const int        _stage)
{
    String lifeBar;
    for (int i = 0; i < _maxLife; ++i)
    {
        lifeBar += (i < _life) ? "♥" : "♡";
    }

    std::ostringstream out;
    out << " ==================================================================================================\n";
    out << std::format("  1P: {}    Life: {}    Score: {}                           Stage {}\n", _playerName, lifeBar, _score, _stage);
    return out.str();
}

String BuildProgressBar(
    const int _tick,
    const int _stageClearTick)
{
    constexpr int kBarWidth = 96;
    String        bar(kBarWidth, '-');
    bar.front() = 'S';
    bar.back()  = 'F';

    const int markerPos = std::clamp((_tick * kBarWidth) / std::max(_stageClearTick, 1), 0, kBarWidth - 1);
    bar[markerPos]      = '>';

    return " " + bar;
}

String BuildNoSaveDataBox()
{
    std::ostringstream out;
    out << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";
    out << "■                                        ■\n";
    out << "■                < 알림 >                ■\n";
    out << "■                                        ■\n";
    out << "■     세이브 정보가 존재하지 않습니다.   ■\n";
    out << "■                                        ■\n";
    out << "■                                        ■\n";
    out << "■                 ENTER                  ■\n";
    out << "■                                        ■\n";
    out << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";
    return out.str();
}
