#ifndef NOMINMAX
#    define NOMINMAX
#endif
#include <Windows.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <vector>

namespace
{

constexpr int kWidth       = 100;
constexpr int kHeight      = 30;
constexpr int kFrameCount  = 60;
constexpr int kObjectCount = 150;
constexpr int kRunCount    = 3;

constexpr const char* kHud = " 1P: Bench    Life: 5    Score: 1234                           Stage 1";

struct Object
{
    int x = 0;
    int y = 0;
};

using MeasureFunc = double (*)(HANDLE, const std::vector<Object>&);

struct Method
{
    const char* name    = nullptr;
    MeasureFunc measure = nullptr;
};

[[nodiscard]] double NowMs()
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return static_cast<double>(counter.QuadPart) * 1000.0 / static_cast<double>(frequency.QuadPart);
}

[[nodiscard]] int MovedX(
    const Object& _object,
    const int     _frame)
{
    return (_object.x + _frame) % (kWidth - 2);
}

[[nodiscard]] double MeasureClsAndPrintPerObject(
    const HANDLE                _hOutput,
    const std::vector<Object>& _objects)
{
    const double start = NowMs();
    for (int frame = 0; frame < kFrameCount; ++frame)
    {
        system("cls");
        for (const Object& object: _objects)
        {
            SetConsoleCursorPosition(_hOutput, { static_cast<SHORT>(MovedX(object, frame)), static_cast<SHORT>(object.y) });
            printf("*");
        }
        SetConsoleCursorPosition(_hOutput, { 0, 0 });
        printf("%s", kHud);
        fflush(stdout);
    }
    return (NowMs() - start) / kFrameCount;
}

[[nodiscard]] double MeasurePrintPerObject(
    const HANDLE                _hOutput,
    const std::vector<Object>& _objects)
{
    const double start = NowMs();
    for (int frame = 0; frame < kFrameCount; ++frame)
    {
        for (const Object& object: _objects)
        {
            SetConsoleCursorPosition(_hOutput, { static_cast<SHORT>(MovedX(object, frame)), static_cast<SHORT>(object.y) });
            printf("*");
        }
        SetConsoleCursorPosition(_hOutput, { 0, 0 });
        printf("%s", kHud);
        fflush(stdout);
    }
    return (NowMs() - start) / kFrameCount;
}

[[nodiscard]] double MeasureArrayAndPrintOnce(
    const HANDLE                _hOutput,
    const std::vector<Object>& _objects)
{
    constexpr int     kRowLength = kWidth + 1;
    std::vector<char> screen(static_cast<size_t>(kRowLength) * kHeight + 1);

    const double start = NowMs();
    for (int frame = 0; frame < kFrameCount; ++frame)
    {
        for (int y = 0; y < kHeight; ++y)
        {
            std::memset(&screen[static_cast<size_t>(y) * kRowLength], ' ', kWidth);
            screen[static_cast<size_t>(y) * kRowLength + kWidth] = '\n';
        }
        std::memcpy(screen.data(), kHud, std::strlen(kHud));
        for (const Object& object: _objects)
        {
            screen[static_cast<size_t>(object.y) * kRowLength + MovedX(object, frame)] = '*';
        }
        screen.back() = '\0';

        SetConsoleCursorPosition(_hOutput, { 0, 0 });
        printf("%s", screen.data());
        fflush(stdout);
    }
    return (NowMs() - start) / kFrameCount;
}

[[nodiscard]] double MeasureBackBufferAndWriteOnce(
    const HANDLE                _hOutput,
    const std::vector<Object>& _objects)
{
    std::vector<CHAR_INFO> cells(static_cast<size_t>(kWidth) * kHeight);
    const size_t           hudLength = std::strlen(kHud);

    const double start = NowMs();
    for (int frame = 0; frame < kFrameCount; ++frame)
    {
        for (CHAR_INFO& cell: cells)
        {
            cell.Char.UnicodeChar = L' ';
            cell.Attributes       = 0x07;
        }
        for (size_t i = 0; i < hudLength; ++i)
        {
            cells[i].Char.UnicodeChar = static_cast<wchar_t>(kHud[i]);
        }
        for (const Object& object: _objects)
        {
            cells[static_cast<size_t>(object.y) * kWidth + MovedX(object, frame)].Char.UnicodeChar = L'*';
        }

        SMALL_RECT writeRegion = { 0, 0, kWidth - 1, kHeight - 1 };
        WriteConsoleOutputW(_hOutput, cells.data(), { kWidth, kHeight }, { 0, 0 }, &writeRegion);
    }
    return (NowMs() - start) / kFrameCount;
}

}   // namespace

int main(
    const int _argc,
    char*     _argv[])
{
    const char*  outputPath = (_argc > 1) ? _argv[1] : "benchmark_result.csv";
    const HANDLE hOutput    = GetStdHandle(STD_OUTPUT_HANDLE);

    std::vector<Object> objects(kObjectCount);
    for (int i = 0; i < kObjectCount; ++i)
    {
        objects[i].x = (i * 37) % (kWidth - 2);
        objects[i].y = 2 + (i * 13) % (kHeight - 4);
    }

    const Method methods[] = {
        { "cls_printf_per_object", MeasureClsAndPrintPerObject },
        { "printf_per_object", MeasurePrintPerObject },
        { "array_printf_once", MeasureArrayAndPrintOnce },
        { "back_buffer_write_console_output", MeasureBackBufferAndWriteOnce },
    };

    double msPerFrame[std::size(methods)] = {};
    for (size_t i = 0; i < std::size(methods); ++i)
    {
        for (int run = 0; run < kRunCount; ++run)
        {
            msPerFrame[i] += methods[i].measure(hOutput, objects);
        }
        msPerFrame[i] /= kRunCount;
    }

    system("cls");
    printf("%d x %d, %d objects, %d frames x %d runs\n\n", kWidth, kHeight, kObjectCount, kFrameCount, kRunCount);

    std::ofstream file(outputPath);
    file << "method,ms_per_frame\n";
    for (size_t i = 0; i < std::size(methods); ++i)
    {
        printf("%-34s %10.4f ms/frame\n", methods[i].name, msPerFrame[i]);
        file << methods[i].name << ',' << msPerFrame[i] << '\n';
    }
    printf("\nsaved: %s\n", outputPath);
    return 0;
}
