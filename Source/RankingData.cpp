#include "RankingData.h"

#include <algorithm>
#include <fstream>

static constexpr const char* kSaveFile = "gamedata.sav";

bool LoadRankingData(Vector<RankEntry>& _outEntries)
{
    std::ifstream file(kSaveFile);
    if (!file.is_open())
    {
        return false;
    }

    Vector<RankEntry> entries;
    RankEntry         entry;
    while (file >> entry.id >> entry.name >> entry.score)
    {
        entries.push_back(entry);
    }

    std::sort(entries.begin(), entries.end(), [](const RankEntry& _a, const RankEntry& _b) { return _a.score > _b.score; });

    _outEntries = std::move(entries);
    return true;
}

void AppendRankingData(
    const int        _id,
    const StringView _name,
    const int        _score)
{
    std::ofstream file(kSaveFile, std::ios::app);
    file << _id << ' ' << _name << ' ' << _score << '\n';
}
