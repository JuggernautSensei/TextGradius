<h1 align="center">TextGradius</h1>

<p align="center">C++20과 Win32 콘솔 API로 만든 텍스트 횡스크롤 슈팅 게임</p>

<p align="center">
  <img src="https://img.shields.io/badge/C++-20-00599C?logo=cplusplus&logoColor=white" alt="C++ 20">
  <img src="https://img.shields.io/badge/Windows-x64-0078D4" alt="Windows x64">
  <img src="https://img.shields.io/badge/Win32-Console_API-2F7D6D" alt="Win32 Console API">
</p>

![스테이지 2의 전투 화면. 적과 탄환, 체력과 점수, 진행 바가 함께 움직입니다.](Resources/Gameplay.gif)

## Contents

* [프로젝트 소개](#프로젝트-소개)
* [게임 루프와 틱 순서](#게임-루프와-틱-순서)
* [콘솔 렌더링](#콘솔-렌더링)
* [오브젝트 풀](#오브젝트-풀)
* [빌드 및 실행](#빌드-및-실행)

## 프로젝트 소개

고전 게임 그라디우스에서 영감을 받아 Windows 콘솔 위에 구현한 횡스크롤 슈팅 게임입니다. 그래픽 라이브러리 없이 100×30칸의 콘솔 화면에 문자를 출력해 게임을 그립니다. 기체는 `▶`, `♣`, `◆`, 적은 `◐`, `☎`, `★`, 탄환은 `─`, `◈`, `♬` 같은 문자로 나타내고, 색은 콘솔의 글자색과 배경색 속성으로 구분합니다. 화면은 초당 30번 갱신됩니다.

플레이어는 방향키로 기체를 움직여 적의 공격을 피합니다. 사격 키는 없고 탄이 자동으로 발사되므로, 기체를 어디에 두느냐가 곧 사격입니다. 적이나 적의 탄환에 닿으면 체력이 줄고, 잠시 무적이 되어 기체가 노란색으로 바뀌며 머리 위에 `피격!`이 표시됩니다. 체력이 모두 사라지면 게임이 끝납니다. 스테이지는 두 개이고 한 판은 1분 남짓입니다.

기체는 Triangle, Clover, Diamond 세 가지입니다. 이동 속도는 같고 내구도, 파괴력, 연사력이 다릅니다. 모든 적의 체력이 같기 때문에, 이 차이는 적을 몇 발로 쓰러뜨리느냐와 얼마나 자주 쏘느냐로 나타납니다.

![Triangle을 선택하고 내구도, 파괴력, 연사력을 막대로 보여 주는 기체 선택 화면](Resources/ShipSelectScreen.png)

<table width="100%">
  <thead>
    <tr>
      <th align="left">기체</th>
      <th align="right">내구도</th>
      <th align="right">파괴력</th>
      <th align="right">연사력</th>
      <th align="left">특징</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>▶ Triangle</td>
      <td align="right">7</td>
      <td align="right">5</td>
      <td align="right">3</td>
      <td>내구도가 가장 높고 나머지는 두 기체의 중간입니다. 세 발이면 적을 쓰러뜨립니다.</td>
    </tr>
    <tr>
      <td>♣ Clover</td>
      <td align="right">5</td>
      <td align="right">3</td>
      <td align="right">6</td>
      <td>한 발은 약하지만 가장 빠르게 쏩니다. 내구도가 가장 낮고, 다섯 발이 필요합니다.</td>
    </tr>
    <tr>
      <td>◆ Diamond</td>
      <td align="right">6</td>
      <td align="right">7</td>
      <td align="right">2</td>
      <td>한 발이 가장 강해서 두 발이면 쓰러뜨리지만, 연사가 느립니다.</td>
    </tr>
  </tbody>
</table>

표의 수치는 기체 선택 화면에 막대로 표시되는 칸 수입니다.

적은 오른쪽에서 나타나 왼쪽으로 이동하며, 세 종류가 있습니다. 쓰러뜨리면 점수를 얻습니다.

* Uros(`◐`, 100점): 위아래로 흔들리며 다가오고, 일정한 간격으로 곧게 날아가는 탄을 쏩니다.
* Call(`☎`, 150점): Uros보다 빠르게 곧장 다가오고, 세 방향으로 퍼지는 탄을 쏩니다.
* Starman(`★`, 200점): 두 번째 스테이지에만 나옵니다. 탄은 쏘지 않고, 플레이어 쪽으로 방향을 바꿔 가며 부딪혀 옵니다. 등장하고 1초 남짓 지나면 스스로 사라지므로 그 전에 쓰러뜨려야 점수를 얻습니다. 부딪히면 피해를 입고, 점수는 10점만 남습니다.

![스테이지 1에서 기체가 적과 탄환 사이를 움직이며 싸우는 장면](Resources/Stage1.gif)

스테이지 1은 Uros가 먼저 나오고, 15초쯤 지나면 Call이 합류합니다. 스테이지 2는 처음부터 Uros와 Call이 번갈아 나오고, 12초쯤 지나면 Starman까지 등장합니다. Uros와 Call은 화면 왼쪽 끝을 지나가도 사라지지 않고 오른쪽에서 다시 나타납니다. 쓰러뜨리지 못한 적이 화면에 계속 쌓이므로, 놓친 적을 처리하는 것도 중요합니다. 한 번에 나올 수 있는 적의 수에는 상한이 있습니다(오브젝트 풀 참고).

결과 화면에서 Enter를 누르면 기록이 `gamedata.sav`에 플레이 ID, 이름, 점수 순으로 한 줄씩 추가됩니다. 플레이 ID는 플레이한 시간을 기반으로 정해집니다. 타이틀의 `RANKING`은 이 파일을 읽어 점수가 높은 순서로 상위 10개를 보여 줍니다. 이름은 공백을 제외한 영문, 숫자, 기호로 최대 10자까지 입력할 수 있습니다. 저장 형식이 공백으로 구분한 텍스트라서 이름에 공백을 넣을 수 없습니다.

![타이틀에서 RANKING을 골라 점수순 기록을 확인하는 화면](Resources/Ranking.gif)

## 게임 루프와 틱 순서

한 판은 타이틀, 이름 입력, 기체 선택, 스테이지 소개, 전투, 결과, 랭킹 화면의 순서로 진행합니다. 타이틀에서 `GAME START`를 고르면 이름을 입력하고 기체를 고른 뒤 전투에 들어갑니다. 스테이지를 하나씩 통과하고, 두 번째 스테이지까지 버티거나 체력이 모두 사라지면 결과 화면이 나옵니다. 여기서 Enter를 누르면 기록이 저장되고 타이틀로 돌아갑니다.

![GRADIUS 로고 아래에 GAME START, RANKING, EXIT 메뉴가 있는 타이틀 화면](Resources/TitleScreen.png)

전투 전에는 스테이지 소개 화면이 5초 동안 나옵니다. Enter를 누르면 건너뛸 수 있고, 두 번째 스테이지의 소개 화면에는 첫 스테이지까지 모은 점수가 함께 표시됩니다.

![Stage 1, Mission Goal, 5초 카운트다운이 표시된 스테이지 소개 화면](Resources/StageIntro.png)

전투 화면 위쪽에는 이름, 체력(`♥`와 `♡`), 점수, 현재 스테이지가 표시됩니다. 아래쪽의 진행 바는 `S`에서 `F`까지 이어지고, `>` 표시가 스테이지의 진행도에 맞춰 오른쪽으로 움직입니다. 진행 바가 끝에 닿으면 스테이지를 통과합니다. 아래 화면은 피격으로 무적 상태가 된 순간입니다.

![체력, 점수, 진행 바가 보이는 스테이지 1 전투 화면. 기체 위에 피격 표시가 나타나 있습니다.](Resources/Battle.png)

이 흐름의 화면들은 각각 `Scene`을 상속한 클래스입니다. 화면마다 입력을 받는 방식과 그리는 내용은 다르지만 화면을 바꾸는 규칙은 같으므로, 씬은 다음에 보여 줄 화면만 알려 주고 실제 교체는 `Game`이 맡도록 나눴습니다. 각 씬의 `Update()`는 다음 씬의 `eSceneId`를 반환합니다. 현재 화면에 머물 때는 `None`, 게임을 끝낼 때는 `Exit`입니다. `Game::Run()`은 이 값을 받아 `CreateScene()`으로 새 씬 객체를 만들고 `OnEnter()`를 호출합니다.

```cpp
while (true)
{
    Sleep(kFrameDelayMs);                       // 33ms

    const eSceneId next = m_scene->Update();    // 현재 씬 갱신과 백버퍼 그리기
    m_console.Present();                        // 콘솔로 한 번에 출력

    if (next == eSceneId::Exit)
    {
        break;
    }
    if (next != eSceneId::None)
    {
        m_scene = CreateScene(next);
        m_scene->OnEnter();
    }
}
```

씬은 서로를 직접 만들거나 참조하지 않습니다. 씬이 바뀌어도 이어져야 하는 플레이어 이름, 점수, 선택한 기체, 현재 스테이지는 `Game`이 갖고 있고, 씬은 생성될 때 받은 `Game` 참조로 이 값을 읽고 씁니다. 스테이지 1이 끝나면 전투 씬은 현재 스테이지를 2로 바꾸고 스테이지 소개 씬을 반환하며, 다음 전투 씬은 이 값을 보고 적 등장 규칙을 고릅니다. 씬은 전환할 때마다 새로 만들어지므로 이전 화면의 상태가 남지 않고, 각 씬은 `OnEnter()`에서 자기 상태만 초기화하면 됩니다.

타이틀, 이름 입력, 기체 선택 씬은 입력으로 상태가 바뀌었을 때만 백버퍼를 다시 그립니다. `Present()`는 매 프레임 호출되지만 백버퍼의 내용은 그대로 남아 있으므로, 바뀌지 않은 화면은 다시 그리지 않아도 같은 화면이 유지됩니다. 매 프레임 백버퍼를 지우고 전부 다시 그리는 것은 전투 씬뿐입니다.

루프는 `timeBeginPeriod(1)`로 타이머 해상도를 1ms로 높인 뒤, 매 프레임 33ms를 쉬고 갱신과 출력을 실행합니다. 갱신과 출력에 걸린 시간을 빼지 않고 고정된 시간만 쉬므로, 실제 프레임 간격은 33ms에 처리 시간이 더해진 값입니다. 앞에서 줄인 출력 시간이 이 간격을 일정하게 유지하는 데도 영향을 줍니다. 게임 안의 시간은 초가 아니라 한 프레임을 뜻하는 틱으로 정의되어 있어서, 스테이지 길이 940틱은 약 31초가 됩니다.

전투 씬은 한 틱마다 다음 순서로 처리합니다.

```cpp
SpawnWave();
UpdateActors();
ResolveContactDamage();
CollectScores();
Render();
```

`SpawnWave()`는 틱 번호와 현재 스테이지를 보고 적을 등장시킵니다. `UpdateActors()`는 플레이어의 이동과 발사, 별과 적의 이동, 적의 발사, 적 탄환의 이동을 처리합니다. 플레이어의 탄이 적에 맞았는지는 적이 자기 `Update()` 안에서 검사하고, 맞은 탄은 소멸 연출로 넘어갑니다. Starman이 부딪혀 오는 판정도 자기 `Update()` 안에 있습니다.

`ResolveContactDamage()`는 적의 몸과 적의 탄환이 플레이어와 겹치는지 검사합니다. 플레이어가 무적이면 검사하지 않고, 아니라면 한 틱에 피해를 한 번만 입히도록 첫 접촉에서 끝냅니다. 적의 탄환은 플레이어에 닿으면 사라집니다. 쓰러지는 중인 적은 접촉 판정에서 빠지므로 폭발 연출 중인 적에게 닿아도 피해가 없습니다.

`CollectScores()`는 이번 틱에 폭발 연출을 마친 적의 점수를 가져와 합산합니다. 적은 `Game`을 알지 못하고, 연출이 끝나는 틱에 한 번만 점수를 내놓습니다(`ConsumePendingScore()`). 점수를 합산하는 일은 전투 씬이 맡기 때문에, 적 클래스는 이동과 공격 방식만 갖습니다. 마지막으로 `Render()`가 별, 플레이어, 적, 적 탄환, HUD, 진행 바를 순서대로 백버퍼에 그립니다.

## 콘솔 렌더링

처음에는 매 프레임 `system("cls")`로 화면을 지운 뒤, 오브젝트마다 커서를 옮기고 `printf`로 문자를 썼습니다. 이 방식에서는 프레임이 크게 떨어져 게임을 진행하기 어려웠고, 화면도 심하게 깜빡였습니다.

![왼쪽은 cls와 한 글자씩 출력하는 방식, 오른쪽은 백버퍼 방식을 나란히 실행한 비교 화면](Resources/FlickerCompare.gif)

느린 이유는 출력 경로와 호출 횟수에 있었습니다. `printf`는 콘솔에 바로 쓰지 않고, 포맷 문자열을 해석하고, stdout 스트림에 잠금을 걸고, 버퍼와 인코딩을 처리한 뒤에야 Windows 콘솔 API를 호출합니다. 처음 방식은 문자 하나를 쓸 때마다 이 경로를 지났으므로, 오브젝트가 많아질수록 같은 비용이 그대로 늘어났습니다. `system("cls")`도 호출할 때마다 명령 해석기 프로세스를 새로 실행합니다. 깜빡임은 화면이 비워진 순간부터 오브젝트가 하나씩 채워질 때까지가 그대로 보였기 때문입니다.

그래서 한 프레임을 메모리에서 먼저 완성하고, 마지막에 콘솔로 한 번만 보내도록 바꿨습니다. 문자와 색상 속성을 담은 `CHAR_INFO` 배열을 백버퍼로 두고, 오브젝트는 `Console::PrintAt()`으로 이 배열의 셀만 바꿉니다. 그리기가 모두 끝나면 `Console::Present()`가 `WriteConsoleOutputW`로 배열 전체를 콘솔에 복사합니다.

```cpp
// 그리기: 콘솔에 쓰지 않고 백버퍼의 셀만 바꿉니다.
CHAR_INFO& cell       = m_cells[static_cast<size_t>(_y) * m_cols + _x];
cell.Char.UnicodeChar = _ch;
cell.Attributes       = _attr;

// 출력: 프레임마다 한 번, 100×30 영역 전체를 복사합니다.
WriteConsoleOutputW(m_hOutput, m_cells.data(), bufferSize, bufferCoord, &writeRegion);
```

콘솔에서는 화면을 지우는 단계가 없어졌으므로, 비워진 화면이 보일 틈 없이 이전 프레임 위에 완성된 다음 프레임이 덮어써집니다. 다만 그래픽스의 더블 버퍼링처럼 두 화면 버퍼를 교체하는 구조는 아닙니다. 콘솔 화면 버퍼는 하나이고, 메모리의 백버퍼를 그 위에 복사합니다. `GRAD_FLICKER_RENDER`를 1로 정의해 빌드하면 같은 백버퍼를 이전 방식으로 내보내며, 위의 비교 화면은 이 설정으로 만든 것입니다.

### 성능측정

원인으로 본 요소를 하나씩 바꿔 가며 프레임당 출력 시간을 쟀습니다. 100×30칸에 오브젝트 150개와 HUD 한 줄을 그리는 프레임을 60번 출력하고, 이를 3회 반복해 평균을 냈습니다.

![네 가지 출력 방식의 프레임당 시간을 로그 눈금으로 비교한 막대 그래프](Resources/RenderBenchmark.svg)

<table width="100%">
  <thead>
    <tr>
      <th align="left">출력 방식</th>
      <th align="left">바꾼 조건</th>
      <th align="right">프레임당 시간</th>
      <th align="right">직전 대비</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>cls + 오브젝트마다 printf</td>
      <td>—</td>
      <td align="right">40.04 ms</td>
      <td align="right">—</td>
    </tr>
    <tr>
      <td>오브젝트마다 printf</td>
      <td>cls 제거</td>
      <td align="right">4.96 ms</td>
      <td align="right">약 8배</td>
    </tr>
    <tr>
      <td>문자 배열 + printf 1회</td>
      <td>출력 호출 151회 → 1회</td>
      <td align="right">0.86 ms</td>
      <td align="right">약 5.8배</td>
    </tr>
    <tr>
      <td>백버퍼 + WriteConsoleOutputW 1회</td>
      <td>CRT 대신 콘솔 API</td>
      <td align="right">0.0819 ms</td>
      <td align="right">약 10.5배</td>
    </tr>
  </tbody>
</table>

처음 방식의 40.04 ms가 마지막 방식에서 0.0819 ms가 되어, 프레임당 출력 시간이 약 490배 짧아졌습니다. 30fps의 프레임 예산 33.3 ms 중 0.3%도 쓰지 않는 시간입니다. 두 번째 방식은 이전 프레임의 문자가 남아 실제로 쓸 수는 없고, `cls` 한 가지의 비용만 따로 보려고 넣은 조건입니다.

처음 방식의 프레임 기록은 남아 있지 않아 위 수치는 `RenderBenchmark`로 같은 조건을 재현한 값입니다. 모든 방식에서 한 가지 색과 한 칸짜리 문자만 썼고, 측정 PC와 콘솔 호스트는 기록하지 않았습니다.

## 오브젝트 풀

전투 중에는 적과 탄환이 계속 생기고 사라집니다. 이를 매번 할당하고 해제하지 않고, 전투 씬에 들어갈 때 종류별 최대 개수만큼 `Vector`에 미리 만들어 둔 뒤 살아 있는지 여부만 바꿔 다시 사용합니다.

<table width="100%">
  <thead>
    <tr>
      <th align="left">오브젝트</th>
      <th align="right">풀 크기</th>
      <th align="left">빈 슬롯이 없을 때</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Uros, Call</td>
      <td align="right">각 15</td>
      <td>새 적을 만들지 않음</td>
    </tr>
    <tr>
      <td>Starman</td>
      <td align="right">10</td>
      <td>새 적을 만들지 않음</td>
    </tr>
    <tr>
      <td>적 탄환</td>
      <td align="right">60</td>
      <td>순환 순서상 가장 오래된 슬롯을 덮어씀</td>
    </tr>
    <tr>
      <td>플레이어 탄환</td>
      <td align="right">20</td>
      <td>순환 순서상 가장 오래된 슬롯을 덮어씀</td>
    </tr>
  </tbody>
</table>

적은 `SpawnInFreeSlot()`이 풀에서 비활성 슬롯을 찾아 다시 등장시킵니다. 빈 슬롯이 없으면 그 틱에는 적이 나오지 않으므로, 풀 크기가 화면에 동시에 존재할 수 있는 적의 상한이 됩니다.

```cpp
template<typename T>
void SpawnInFreeSlot(Vector<T>& _pool)
{
    for (T& enemy: _pool)
    {
        if (!enemy.IsAlive())
        {
            enemy.Spawn();   // 위치와 체력을 초기화하고 다시 활성화합니다.
            return;
        }
    }
}
```

탄환은 빈 슬롯을 찾지 않습니다. 커서가 풀을 돌며 다음 슬롯에 새 탄환을 쓰기 때문에 발사할 때 검색 비용이 없습니다. 풀이 가득 찬 상태에서는 가장 먼저 쓴 슬롯의 탄환이 새 탄환으로 바뀝니다. 탄환은 화면 끝에 닿거나 명중하면 짧은 소멸 효과를 보여 준 뒤 비활성화됩니다.

세 종류의 적은 서로 다른 타입의 풀에 들어 있지만, 씬에 들어갈 때 공통 부모인 `Enemy`의 포인터 목록으로도 묶어 둡니다. 매 틱의 갱신, 발사, 접촉 판정, 점수 수집, 그리기는 이 목록을 한 번씩 순회하면 되고, 적마다 다른 이동과 공격 방식은 각 클래스의 `Update()`와 `Fire()`가 맡습니다. 이 포인터들이 가리키는 `Vector`는 `OnEnter()`에서 크기를 정한 뒤에는 늘어나거나 줄지 않으므로, 목록의 포인터는 전투가 끝날 때까지 유효합니다.

## 빌드 및 실행

Windows에서 C++20으로 빌드합니다. Visual Studio의 "C++를 사용한 데스크톱 개발" 워크로드와 Windows SDK가 필요하고, 프로젝트의 플랫폼 도구 집합이 `v145`로 지정되어 있어서 Visual Studio 2026을 기준으로 합니다. 더 낮은 버전을 쓴다면 프로젝트 속성에서 플랫폼 도구 집합을 설치된 버전으로 바꿔야 합니다. 외부 라이브러리는 없습니다.

1. 저장소를 받습니다.

   ```powershell
   git clone https://github.com/JuggernautSensei/TextGradius.git
   ```

2. `TextGradius.slnx`를 Visual Studio로 열고, `TextGradius` 프로젝트를 시작 프로젝트로 지정합니다.
3. 구성을 `Release | x64`로 두고 빌드한 뒤 실행합니다(`Ctrl+F5`).

창 크기는 실행할 때 100×30으로 맞춰집니다. Windows Terminal에서는 이 설정이 적용되지 않을 수 있으며, 그때는 창을 직접 100열×30행 이상으로 조절해야 합니다.

<table width="100%">
  <thead>
    <tr>
      <th align="left">상황</th>
      <th align="left">키</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>메뉴, 기체 선택</td>
      <td>방향키 위, 아래</td>
    </tr>
    <tr>
      <td>전투 중 기체 이동</td>
      <td>방향키 (사격은 자동)</td>
    </tr>
    <tr>
      <td>결정, 다음 화면, 스테이지 소개 건너뛰기</td>
      <td>Enter</td>
    </tr>
    <tr>
      <td>이름 입력 중 지우기</td>
      <td>Backspace</td>
    </tr>
    <tr>
      <td>전투 스테이지 건너뛰기</td>
      <td>Enter (Debug 빌드에서만)</td>
    </tr>
  </tbody>
</table>

랭킹 기록은 실행할 때의 작업 폴더에 있는 `gamedata.sav`에 저장됩니다. Visual Studio에서 실행하면 프로젝트 폴더, 곧 저장소 루트입니다. 저장소에는 개발 중에 쌓인 기록이 들어 있으므로, 기록 없이 시작하려면 이 파일을 지우면 됩니다.

성능측정은 `RenderBenchmark` 프로젝트로 다시 해 볼 수 있습니다. `Release | x64`로 빌드해 콘솔 창에서 실행하면 결과가 CSV로 저장됩니다. 차트는 CSV에서 다시 만들 수 있습니다(Python 3 필요).

```powershell
.\x64\Release\RenderBenchmark.exe Benchmark\benchmark_result.csv
python Benchmark/plot_benchmark.py Benchmark/benchmark_result.csv Resources/RenderBenchmark.svg
```
