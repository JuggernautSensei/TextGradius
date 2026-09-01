#pragma once
#ifndef NOMINMAX
#    define NOMINMAX
#endif
#include <Windows.h>

#include "Typedef.h"

enum class eColor : int
{
    Black        = 0,
    Blue         = 1,
    Green        = 2,
    Cyan         = 3,
    Red          = 4,
    Magenta      = 5,
    Brown        = 6,
    LightGray    = 7,
    DarkGray     = 8,
    LightBlue    = 9,
    LightGreen   = 10,
    LightCyan    = 11,
    LightRed     = 12,
    LightMagenta = 13,
    Yellow       = 14,
    White        = 15,
};

class Console
{
public:
    Console() = default;
    ~Console();

    Console(const Console&)            = delete;
    Console& operator=(const Console&) = delete;
    Console(Console&&)                 = delete;
    Console& operator=(Console&&)      = delete;

    void Init(int _cols, int _rows);

    void Clear();
    void SetColor(eColor _foreground, eColor _background = eColor::Black);

    void PrintAt(int _x, int _y, StringView _text);
    void PrintAt(int _x, int _y, StringView _text, eColor _foreground, eColor _background = eColor::Black);

    void Present();

private:
    void WriteCell(int _x, int _y, wchar_t _ch, WORD _attr);

    HANDLE m_hOutput     = nullptr;
    int    m_cols        = 0;
    int    m_rows        = 0;
    WORD   m_currentAttr = 0x0F;

    Vector<CHAR_INFO> m_cells;
};
