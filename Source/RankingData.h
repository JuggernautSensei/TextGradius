#pragma once
#include "Typedef.h"

struct RankEntry
{
    int    id = 0;
    String name;
    int    score = 0;
};

[[nodiscard]] bool LoadRankingData(Vector<RankEntry>& _outEntries);

void AppendRankingData(int _id, StringView _name, int _score);
