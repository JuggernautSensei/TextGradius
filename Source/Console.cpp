#include "Console.h"

#include <algorithm>

namespace
{

[[nodiscard]] WORD MakeAttr(
    const eColor _foreground,
    const eColor _background)
{
    return static_cast<WORD>(_foreground) | (static_cast<WORD>(_background) << 4);
}

[[nodiscard]] std::wstring Utf8ToUtf16(const StringView _utf8)
{
    if (_utf8.empty())
    {
        return {};
    }
    const int    len = MultiByteToWideChar(CP_UTF8, 0, _utf8.data(), static_cast<int>(_utf8.size()), nullptr, 0);
    std::wstring wide(static_cast<size_t>(len), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, _utf8.data(), static_cast<int>(_utf8.size()), wide.data(), len);
    return wide;
}

[[nodiscard]] bool IsWideChar(const wchar_t _ch)
{
    const unsigned c = static_cast<unsigned>(_ch);
    return (c >= 0x1100 && c <= 0x115F)
        || (c >= 0x2E80 && c <= 0x303E)
        || (c >= 0x3041 && c <= 0x33FF)
        || (c >= 0x3400 && c <= 0x4DBF)
        || (c >= 0x4E00 && c <= 0x9FFF)
        || (c >= 0xAC00 && c <= 0xD7A3)
        || (c >= 0xF900 && c <= 0xFAFF)
        || (c >= 0xFF00 && c <= 0xFF60)
        || (c >= 0xFFE0 && c <= 0xFFE6);
}

}   // namespace

Console::~Console()
{
    if (m_hOriginal)
    {
        SetConsoleActiveScreenBuffer(m_hOriginal);
    }
    for (const HANDLE buffer: m_hBuffer)
    {
        if (buffer)
        {
            CloseHandle(buffer);
        }
    }
}

void Console::Init(
    const int _cols,
    const int _rows)
{
    m_cols = _cols;
    m_rows = _rows;
    m_cells.assign(static_cast<size_t>(_cols) * static_cast<size_t>(_rows), CHAR_INFO {});

    SetConsoleTitleW(L"TextGradius");

    m_hOriginal = GetStdHandle(STD_OUTPUT_HANDLE);

    const COORD      size   = { static_cast<SHORT>(_cols), static_cast<SHORT>(_rows) };
    const SMALL_RECT window = { 0, 0, static_cast<SHORT>(_cols - 1), static_cast<SHORT>(_rows - 1) };

    for (HANDLE& buffer: m_hBuffer)
    {
        buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);

        const SMALL_RECT shrink = { 0, 0, 0, 0 };
        SetConsoleWindowInfo(buffer, TRUE, &shrink);
        SetConsoleScreenBufferSize(buffer, size);
        SetConsoleWindowInfo(buffer, TRUE, &window);

        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.bVisible = FALSE;
        cursorInfo.dwSize   = 1;
        SetConsoleCursorInfo(buffer, &cursorInfo);
    }

    Clear();
    Blit(m_hBuffer[0]);
    Blit(m_hBuffer[1]);
    SetConsoleActiveScreenBuffer(m_hBuffer[0]);
    m_activeIndex = 0;
}

void Console::Clear()
{
    CHAR_INFO blank;
    blank.Char.UnicodeChar = L' ';
    blank.Attributes       = 0x00;
    std::ranges::fill(m_cells, blank);
}

void Console::SetColor(
    const eColor _foreground,
    const eColor _background)
{
    m_currentAttr = MakeAttr(_foreground, _background);
}

void Console::WriteCell(
    const int     _x,
    const int     _y,
    const wchar_t _ch,
    const WORD    _attr)
{
    if (_x < 0 || _x >= m_cols || _y < 0 || _y >= m_rows)
    {
        return;
    }
    CHAR_INFO& cell       = m_cells[static_cast<size_t>(_y) * m_cols + _x];
    cell.Char.UnicodeChar = _ch;
    cell.Attributes       = _attr;
}

void Console::PrintAt(
    const int        _x,
    const int        _y,
    const StringView _text)
{
    PrintAt(_x, _y, _text, static_cast<eColor>(m_currentAttr & 0x0F), static_cast<eColor>((m_currentAttr >> 4) & 0x0F));
}

void Console::PrintAt(
    const int        _x,
    const int        _y,
    const StringView _text,
    const eColor     _foreground,
    const eColor     _background)
{
    const WORD         attr = MakeAttr(_foreground, _background);
    const std::wstring wide = Utf8ToUtf16(_text);

    int cx = _x;
    int cy = _y;
    for (const wchar_t ch: wide)
    {
        if (ch == L'\n')
        {
            ++cy;
            cx = 0;
            continue;
        }
        if (ch == L'\t')
        {
            cx = ((cx / 8) + 1) * 8;
            continue;
        }
        if (IsWideChar(ch))
        {
            WriteCell(cx, cy, ch, attr | COMMON_LVB_LEADING_BYTE);
            WriteCell(cx + 1, cy, ch, attr | COMMON_LVB_TRAILING_BYTE);
            cx += 2;
            continue;
        }
        WriteCell(cx, cy, ch, attr);
        ++cx;
    }
}

void Console::Blit(const HANDLE _target) const
{
    const COORD bufferSize  = { static_cast<SHORT>(m_cols), static_cast<SHORT>(m_rows) };
    const COORD bufferCoord = { 0, 0 };
    SMALL_RECT  writeRegion = { 0, 0, static_cast<SHORT>(m_cols - 1), static_cast<SHORT>(m_rows - 1) };
    WriteConsoleOutputW(_target, m_cells.data(), bufferSize, bufferCoord, &writeRegion);
}

void Console::Present()
{
    const int target = 1 - m_activeIndex;
    Blit(m_hBuffer[target]);
    SetConsoleActiveScreenBuffer(m_hBuffer[target]);
    m_activeIndex = target;
}
