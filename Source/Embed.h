#pragma once
#include "Player.h"
#include "RankingData.h"
#include "Typedef.h"

constexpr StringView kTitleArt =
    "\n"
    "     　　　　　　　 ██████╗  ██████╗   █████╗  ██████╗  ██╗ ██╗   ██╗ ███████╗\n"
    "     　　　　　　　██╔════╝  ██╔══██╗ ██╔══██╗ ██╔══██╗ ██║ ██║   ██║ ██╔════╝\n"
    "     　　　　　　　██║  ███╗ ██████╔╝ ███████║ ██║  ██║ ██║ ██║   ██║ ███████╗\n"
    "     　　　　　　　██║   ██║ ██╔══██╗ ██╔══██║ ██║  ██║ ██║ ██║   ██║ ╚════██║\n"
    "     　　　　　　　╚██████╔╝ ██║  ██║ ██║  ██║ ██████╔╝ ██║ ╚██████╔╝ ███████║\n"
    "     　　　　　　　 ╚═════╝  ╚═╝  ╚═╝ ╚═╝  ╚═╝ ╚═════╝  ╚═╝  ╚═════╝  ╚══════╝\n"
    "\n";

constexpr StringView kCopyright = "                           　      　 ⓒ Made by Ahn Jiwoo";

enum class eMainMenuOption
{
    Start,
    Ranking,
    Exit,
    Count,
};

[[nodiscard]] String BuildMainMenu(eMainMenuOption _selected);

[[nodiscard]] String BuildNameEntryPrompt(StringView _currentInput);

[[nodiscard]] String BuildShipSelect(StringView _playerName, eShipType _selected);

[[nodiscard]] String BuildMissionBrief(int _stage, int _countdown, int _score);

[[nodiscard]] String BuildResultScreen(bool _bCleared, StringView _playerName, int _score);

[[nodiscard]] String BuildRankingTable(const Vector<RankEntry>& _entries);

[[nodiscard]] String BuildNoSaveDataBox();

[[nodiscard]] String BuildHudHeader(StringView _playerName, int _life, int _maxLife, int _score, int _stage);

[[nodiscard]] String BuildProgressBar(int _tick, int _stageClearTick);
