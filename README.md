# TextGradius

![게임플레이](Resources/Gameplay.gif)

TextGradius는 Windows 콘솔에서 동작하는 C++20 횡스크롤 슈팅 게임입니다. 100×30 크기의 콘솔 화면을 30fps로 갱신하며, 모든 그래픽은 문자와 콘솔 색상 속성만으로 표현합니다.

## Contents

- [게임 소개](#게임-소개)
- [콘솔 렌더링](#콘솔-렌더링)
- [코드 구조](#코드-구조)

## 게임 소개

### 화면 흐름

타이틀 → 이름 입력 → 기체 선택 → 스테이지 소개(5초 카운트다운) → 스테이지 1 → 스테이지 2 → 결과 → 랭킹 순서로 진행합니다. 각 스테이지는 940틱(약 31초) 동안 살아남으면 클리어됩니다. 방향키로 기체를 움직이며, 사격은 자동으로 이루어집니다.

![타이틀 화면](Resources/TitleScreen.png)

### 기체

기체 선택 화면에서 세 기체 중 하나를 고릅니다. 모든 적의 체력은 14로 같기 때문에, 파괴력이 높을수록 적을 적은 탄으로 격추할 수 있습니다.

![기체 선택 화면](Resources/ShipSelectScreen.png)

| 기체 | 내구도 | 파괴력 | 발사 간격 |
|---|---|---|---|
| ▶ Triangle | 7 | 5 | 4틱 |
| ♣ Clover | 5 | 3 | 2틱 |
| ◆ Diamond | 6 | 7 | 6틱 |

### 적

| 적 | 이동 | 공격 | 점수 | 등장 |
|---|---|---|---|---|
| ◐ Uros | 지그재그로 왼쪽 이동 | 30틱마다 직선탄 | 100 | 스테이지 1부터 |
| ☎ Call | 왼쪽으로 직진 | 45틱마다 3방향 탄 | 150 | 스테이지 1의 400틱 이후, 스테이지 2 |
| ★ Starman | 플레이어를 추적 | 몸통 박치기 | 처치 200, 충돌 10 | 스테이지 2의 300틱 이후 |

Starman은 등장 후 40틱이 지나면 스스로 사라지며, 이때는 점수를 주지 않습니다.

### 랭킹

결과 화면에서 Enter를 누르면 이름과 점수가 `gamedata.sav`에 누적 저장되고, 랭킹 화면에서 점수순으로 정렬해 보여 줍니다.

![랭킹](Resources/Ranking.gif)

## 콘솔 렌더링

### 문제

처음에는 매 프레임 `system("cls")`로 화면을 지운 뒤 오브젝트마다 커서를 옮기고 `printf`로 출력했습니다. 이 방식은 화면이 지워진 순간과 다시 그려지는 과정이 그대로 보여 깜빡임이 심했고, 실제 플레이에서는 게임을 진행하기 어려울 만큼 프레임이 떨어졌습니다.

![출력 방식별 깜빡임 비교](Resources/FlickerCompare.gif)

*왼쪽은 cls 후 한 글자씩 출력하는 방식, 오른쪽은 현재의 백버퍼 방식입니다.*

### 원인

`printf`는 콘솔에 바로 쓰지 않습니다. 포맷 문자열을 파싱하고 값을 문자열로 변환한 뒤, CRT의 stdout 스트림에서 잠금, 버퍼 관리, 스트림 상태 확인, 로케일 처리를 거쳐야 Windows 출력 API에 도달합니다. 이 비용이 오브젝트 수만큼 반복되고, 콘솔로 향하는 호출 횟수 역시 오브젝트 수만큼 늘어납니다.

### 해결

CRT를 거치지 않고 콘솔 API인 `WriteConsoleOutputW`를 직접 사용했습니다. 이 API도 호출할 때마다 콘솔 호스트와 통신하는 비용이 들기 때문에, 출력 함수들이 내부 버퍼에 모았다가 한 번에 flush하는 방식을 따랐습니다. 그래픽스의 더블 버퍼링에서 착안해, 한 프레임을 `CHAR_INFO` 배열인 백버퍼에 먼저 그린 뒤 프레임 끝에서 한 번만 콘솔로 복사합니다. 화면 버퍼를 교체하는 대신 완성된 프레임을 한 번에 덮어쓰므로, 지워진 화면이 노출되지 않습니다.

```cpp
// 그리기: 백버퍼의 셀만 갱신합니다.
CHAR_INFO& cell       = m_cells[static_cast<size_t>(_y) * m_cols + _x];
cell.Char.UnicodeChar = _ch;
cell.Attributes       = _attr;

// 출력: 프레임마다 한 번 호출합니다.
SMALL_RECT writeRegion = { 0, 0, static_cast<SHORT>(m_cols - 1), static_cast<SHORT>(m_rows - 1) };
WriteConsoleOutputW(m_hOutput, m_cells.data(), bufferSize, bufferCoord, &writeRegion);
```

`GRAD_FLICKER_RENDER`를 1로 정의하고 빌드하면 같은 백버퍼를 cls 후 한 글자씩 출력하는 이전 방식으로 그려, 두 방식을 직접 비교할 수 있습니다.

### 검증

원인을 확인하기 위해 `Benchmark/RenderBenchmark.cpp`에서 조건을 하나씩 바꾼 네 가지 출력 방식을 측정했습니다. 100×30 화면에 오브젝트 150개를 그리는 프레임을 60번 출력하고, 이를 3회 반복한 평균입니다.

![프레임당 출력 시간](Resources/RenderBenchmark.svg)

| 방식 | ms/프레임 | 바뀐 조건 |
|---|---|---|
| cls + 오브젝트마다 printf | 40.04 | — |
| 오브젝트마다 printf | 4.96 | cls 제거 |
| 문자 배열 + printf 1회 | 0.86 | 출력 호출 150회 → 1회 |
| 백버퍼 + WriteConsoleOutputW 1회 | 0.08 | CRT 대신 콘솔 API |

cls를 없애면 약 8배, 출력 호출을 한 번으로 모으면 다시 약 6배, CRT 대신 콘솔 API를 쓰면 다시 약 10배 빨라져, 처음 방식보다 약 490배 짧은 시간에 한 프레임을 출력합니다. 마지막 방식은 30fps의 프레임 예산인 33.3ms의 0.3%도 쓰지 않습니다.

## 코드 구조

### 게임 루프

`Game::Run()`은 `timeBeginPeriod(1)`로 타이머 해상도를 1ms로 높인 뒤, 매 프레임 33ms를 쉬고 현재 씬을 갱신한 다음 백버퍼를 출력합니다.

```cpp
while (true)
{
    Sleep(kFrameDelayMs);

    const eSceneId next = m_scene->Update();
    m_console.Present();
    ...
}
```

### 씬 전환

타이틀, 이름 입력, 기체 선택, 스테이지 소개, 게임플레이, 결과, 랭킹의 7개 씬이 `Scene`을 상속합니다. 각 씬의 `Update()`는 전환할 다음 씬의 `eSceneId`를 반환하고, `Game`이 `CreateScene()`으로 새 씬을 만들어 교체합니다. 씬끼리는 서로를 알지 못하며, 점수와 플레이어 같은 공유 상태는 `Game`이 가집니다.

```cpp
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

### 오브젝트 풀

적과 탄환은 스테이지가 시작될 때 최대 개수만큼 `Vector`에 미리 만들어 두고, 게임 중에는 새로 할당하지 않습니다. 적은 비활성 슬롯을 찾아 다시 사용하고, 탄환은 커서가 풀을 순환하며 다음 슬롯을 덮어씁니다.

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
