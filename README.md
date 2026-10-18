<div align="center">

# TextGradius

**콘솔 텍스트 출력으로 동작하는 횡스크롤 슈팅 게임**

![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white)
![Windows](https://img.shields.io/badge/Platform-Windows-0078D4)
![Console](https://img.shields.io/badge/Render-Win32_Console-4D4D4D)

<p align="center"><img src="Resources/Gameplay.gif" alt="적과 탄환, 체력과 점수 표시가 함께 움직이는 스테이지 2 플레이 화면" width="840"></p>

</div>

## Contents

- [프로젝트 소개](#프로젝트-소개)
- [콘솔 렌더링](#콘솔-렌더링)
- [씬 전환](#씬-전환)
- [오브젝트 풀](#오브젝트-풀)
- [빌드 및 실행](#빌드-및-실행)

<br>

## 프로젝트 소개

고전 슈팅 게임 그라디우스의 횡스크롤 전투를 Windows 콘솔 위에 구현한 프로젝트입니다. 화면은 100×30 칸의 문자로 이루어져 있고, 기체와 적, 탄환과 배경의 별은 모두 `▶`, `◐`, `☎`, `★` 같은 문자와 콘솔 색상 속성으로 표현합니다. 게임은 30fps로 갱신됩니다.

게임은 타이틀에서 이름을 입력하고 기체를 고르는 것으로 시작합니다. 스테이지마다 5초의 소개 화면이 지나면 전투가 시작되고, 940틱(약 31초) 동안 살아남으면 다음 스테이지로 넘어갑니다. 두 스테이지를 모두 버티거나 체력을 모두 잃으면 결과 화면이 나오고, 이름과 점수는 랭킹에 기록됩니다.

<p align="center"><img src="Resources/TitleScreen.png" alt="GRADIUS 로고 아래 GAME START, RANKING, EXIT 메뉴가 있는 타이틀 화면" width="840"></p>

기체는 Triangle, Clover, Diamond 세 가지입니다. 모든 적의 체력이 14로 같기 때문에 기체의 차이는 몇 발로 적을 격추하느냐와 얼마나 자주 쏘느냐에서 드러납니다. Triangle은 내구도 7과 파괴력 5로 균형 잡힌 기체이고, Clover는 파괴력이 3으로 낮은 대신 2틱마다 탄을 쏩니다. Diamond는 파괴력 7로 두 발이면 적을 격추하지만 6틱마다 한 발만 쏩니다.

<p align="center"><img src="Resources/ShipSelectScreen.png" alt="Clover를 선택하고 내구도, 파괴력, 연사력을 막대로 보여 주는 기체 선택 화면" width="840"></p>
<p align="center"><sub>사격은 자동으로 이루어지며, 플레이어는 방향키로 이동만 조작합니다.</sub></p>

적은 세 종류입니다. Uros(`◐`)는 위아래로 흔들리며 다가와 직선탄을 쏘고, Call(`☎`)은 곧게 날아오며 세 방향으로 탄을 퍼뜨립니다. 두 번째 스테이지 중반부터 등장하는 Starman(`★`)은 탄을 쏘지 않는 대신 플레이어를 향해 가속하며 부딪혀 옵니다. 40틱 안에 격추하면 200점을 얻지만, 그 전에 부딪히면 피해를 입고 10점만 남습니다.

결과 화면에서 Enter를 누르면 기록이 `gamedata.sav`에 누적되고, 랭킹 화면은 이 파일을 읽어 점수순으로 정렬해 보여 줍니다.

<p align="center"><img src="Resources/Ranking.gif" alt="타이틀에서 RANKING을 골라 점수순 기록을 확인하는 화면" width="840"></p>

<br>

## 콘솔 렌더링

콘솔 게임은 매 프레임 화면 전체를 다시 출력해야 합니다. 처음에는 `system("cls")`로 화면을 지운 뒤, 오브젝트마다 커서를 옮겨 `printf`로 한 글자씩 썼습니다. 이 방식은 화면이 지워진 순간과 다시 채워지는 과정이 그대로 보여 깜빡임이 심했고, 실제 플레이에서는 게임을 진행하기 어려울 만큼 프레임이 떨어졌습니다.

<p align="center"><img src="Resources/FlickerCompare.gif" alt="왼쪽 cls와 한 글자씩 출력하는 방식은 화면이 깜빡이고, 오른쪽 백버퍼 방식은 안정적으로 보이는 비교 화면" width="960"></p>
<p align="center"><sub>왼쪽은 처음 방식, 오른쪽은 현재 방식입니다. 같은 게임을 출력 방식만 바꿔 실행했습니다.</sub></p>

### printf의 비용

원인은 출력 경로에 있었습니다. `printf`는 콘솔에 바로 쓰지 않습니다. 포맷 문자열을 파싱하고 값을 문자열로 바꾼 다음, CRT의 stdout 스트림에서 잠금과 버퍼 관리, 스트림 상태 확인, 로케일 처리를 거친 뒤에야 Windows 출력 API에 도달합니다. 한 글자를 쓰는 데에도 이 과정을 모두 거치고, 그 호출이 화면의 오브젝트 수만큼 반복됩니다.

### 백버퍼와 한 번의 출력

그래서 포맷 처리가 필요 없는 콘솔 API `WriteConsoleOutputW`를 직접 사용했습니다. 이 API도 호출할 때마다 콘솔 호스트와 통신하는 비용이 들기 때문에, 출력 함수들이 내부 버퍼에 모았다가 한 번에 내보내는 방식을 따르기로 했습니다.

그래픽스의 더블 버퍼링에서 착안해, 한 프레임은 `CHAR_INFO` 배열로 된 백버퍼에 먼저 그립니다. 오브젝트의 `Render()`는 콘솔에 직접 쓰지 않고 이 배열의 셀만 바꿉니다. 모든 그리기가 끝나면 `Present()`가 배열 전체를 한 번의 호출로 콘솔에 복사합니다.

```cpp
// 그리기: 백버퍼의 셀만 바꿉니다.
CHAR_INFO& cell       = m_cells[static_cast<size_t>(_y) * m_cols + _x];
cell.Char.UnicodeChar = _ch;
cell.Attributes       = _attr;

// 출력: 프레임마다 한 번만 호출합니다.
WriteConsoleOutputW(m_hOutput, m_cells.data(), bufferSize, bufferCoord, &writeRegion);
```

화면 버퍼를 교체하는 대신 완성된 프레임으로 덮어쓰므로, 지워진 화면이 노출될 틈이 없습니다. 색상도 셀마다 속성으로 함께 담기기 때문에 글자마다 색을 바꾸는 호출도 사라졌습니다. `GRAD_FLICKER_RENDER`를 1로 정의해 빌드하면 같은 백버퍼를 처음 방식으로 출력해, 위의 비교 화면처럼 두 방식을 직접 확인할 수 있습니다.

### 출력 방식 비교

어느 요인이 얼마나 영향을 주는지 확인하기 위해, `Benchmark/RenderBenchmark.cpp`에서 조건을 하나씩 바꾼 네 가지 출력 방식을 측정했습니다. 게임 화면과 같은 100×30 크기에 오브젝트 150개와 HUD 한 줄을 그리는 프레임을 60번 출력하고, 이를 3회 반복한 평균입니다.

<p align="center"><img src="Resources/RenderBenchmark.svg" alt="네 가지 출력 방식의 프레임당 출력 시간을 로그 눈금으로 비교한 막대 그래프" width="760"></p>

| 방식 | 바뀐 조건 | 프레임당 시간 |
|---|---|---:|
| cls + 오브젝트마다 printf | — | 40.04ms |
| 오브젝트마다 printf | cls 제거 | 4.96ms |
| 문자 배열 + printf 1회 | 출력 호출 151회 → 1회 | 0.86ms |
| 백버퍼 + WriteConsoleOutputW 1회 | CRT 대신 콘솔 API | 0.08ms |

> [!NOTE]
> 측정 결과는 `Benchmark/benchmark_result.csv`에 있으며, 차트는 `Benchmark/plot_benchmark.py`로 생성했습니다. 측정 환경의 하드웨어 정보는 기록되어 있지 않습니다.

`cls`를 없애는 것만으로 약 8배 빨라졌습니다. 출력을 하나의 문자열로 모아 `printf`를 한 번만 호출하면 다시 약 6배가 줄어, 호출 횟수 자체가 큰 비용이었음을 보여 줍니다. 마지막으로 같은 한 번의 출력을 CRT 대신 콘솔 API로 바꾸면 다시 약 10배 빨라집니다. 처음 방식과 비교하면 약 490배로, 30fps의 프레임 예산 33.3ms 가운데 0.3%도 쓰지 않습니다.

벤치마크의 처음 방식은 프레임당 40ms로 약 25fps에 해당하지만, 이는 고정된 오브젝트 150개를 단색으로 그린 조건의 값입니다. 처음 게임에서 겪은 프레임 저하를 그대로 재현한 수치는 아닙니다.

관련 구현: [`Console.cpp`](Source/Console.cpp), [`RenderBenchmark.cpp`](Benchmark/RenderBenchmark.cpp).

<br>

## 씬 전환

타이틀, 이름 입력, 기체 선택, 스테이지 소개, 게임플레이, 결과, 랭킹의 7개 화면은 각각 `Scene`을 상속한 씬입니다. 씬은 다음에 어디로 갈지만 알면 되고, 다른 씬을 직접 만들지 않습니다. 각 씬의 `Update()`는 다음 씬의 `eSceneId`를 돌려주고, 전환은 `Game`이 맡습니다.

```cpp
const eSceneId next = m_scene->Update();
m_console.Present();

if (next == eSceneId::Exit)
{
    break;
}
if (next != eSceneId::None)
{
    m_scene = CreateScene(next);
    m_scene->OnEnter();
}
```

`Game::Run()`은 `timeBeginPeriod(1)`로 타이머 해상도를 1ms로 높인 뒤, 매 프레임 33ms를 쉬고 현재 씬을 갱신한 다음 백버퍼를 출력합니다. 씬이 바뀌어도 이어져야 하는 플레이어 이름과 점수, 기체와 현재 스테이지는 `Game`이 가지고 있으며, 씬은 생성할 때 받은 `Game` 참조로 이 값에 접근합니다.

관련 구현: [`Game.cpp`](Source/Game.cpp), [`Scene.h`](Source/Scene.h).

<br>

## 오브젝트 풀

전투 중에는 적과 탄환이 끊임없이 생기고 사라집니다. 이를 매번 할당하고 해제하는 대신, 스테이지가 시작될 때 종류별 최대 개수만큼 `Vector`에 미리 만들어 두고 살아 있는지 여부만 바꿔 재사용합니다. 적은 Uros와 Call이 15개씩, Starman이 10개이고, 적 탄환은 60개, 플레이어 탄환은 20개입니다.

적은 비활성 슬롯을 찾아 다시 등장시킵니다. 빈 슬롯이 없으면 그 프레임에는 새 적이 나오지 않습니다.

```cpp
template<typename T>
void SpawnInFreeSlot(Vector<T>& _pool)
{
    for (T& enemy: _pool)
    {
        if (!enemy.IsAlive())
        {
            enemy.Spawn();
            return;
        }
    }
}
```

탄환은 빈 슬롯을 찾지 않고, 커서가 풀을 순환하며 다음 슬롯을 덮어씁니다. 풀이 가득 찬 상태에서는 가장 오래된 탄환이 새 탄환으로 바뀝니다. 세 종류의 적은 공통 부모인 `Enemy` 포인터 목록으로도 묶어, 갱신과 충돌 검사, 그리기를 한 번의 순회로 처리합니다.

관련 구현: [`GameplayScene.cpp`](Source/GameplayScene.cpp), [`EnemyBullet.cpp`](Source/EnemyBullet.cpp), [`Enemy.cpp`](Source/Enemy.cpp).

<br>

## 빌드 및 실행

Windows에서 C++20으로 빌드합니다. Visual Studio의 C++ 데스크톱 개발 도구와 프로젝트가 지정한 MSVC `v145` 도구 집합이 필요하며, 외부 라이브러리는 사용하지 않습니다.

1. `TextGradius.slnx`를 Visual Studio로 엽니다.
2. `TextGradius` 프로젝트를 시작 프로젝트로 지정하고 빌드한 뒤 실행합니다.

랭킹 기록은 실행 위치의 `gamedata.sav`에 저장됩니다. Debug 빌드에서는 게임 중 Enter로 현재 스테이지를 건너뛸 수 있습니다.

| 조작 | 키 |
|---|---|
| 기체 이동, 메뉴 선택 | 방향키 |
| 결정 | Enter |
| 이름 입력 중 지우기 | Backspace |

출력 방식 비교는 `RenderBenchmark` 프로젝트를 빌드해 실행하면 `benchmark_result.csv`로 저장되며, 차트는 다음 명령으로 다시 만들 수 있습니다.

```powershell
python Benchmark/plot_benchmark.py Benchmark/benchmark_result.csv Resources/RenderBenchmark.svg
```
