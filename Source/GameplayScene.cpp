#include "GameplayScene.h"

#include "Config.h"
#include "Embed.h"
#include "Game.h"
#include "Input.h"

namespace
{

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

}   // namespace

void GameplayScene::OnEnter()
{
    m_ct = 0;

    m_stars.assign(kMaxStar, Star {});
    for (Star& star: m_stars)
    {
        star.Init();
    }

    m_uros.assign(kMaxUros, UrosEnemy {});
    m_call.assign(kMaxCall, CallEnemy {});
    m_starman.assign(kMaxStarman, StarmanEnemy {});

    m_enemies.clear();
    for (UrosEnemy& enemy: m_uros)
    {
        m_enemies.push_back(&enemy);
    }
    for (CallEnemy& enemy: m_call)
    {
        m_enemies.push_back(&enemy);
    }
    for (StarmanEnemy& enemy: m_starman)
    {
        m_enemies.push_back(&enemy);
    }

    m_enemyBullets.assign(kMaxEnemyBullet, EnemyBullet {});
    m_enemyBulletCursor = 0;

    m_game.GetPlayer().ResetForStage();
}

void GameplayScene::SpawnWave()
{
    if (m_game.CurrentStage() == 1)
    {
        if (m_ct > 10 && m_ct % 40 == 0)
        {
            SpawnInFreeSlot(m_uros);
        }
        if (m_ct > 400 && m_ct % 50 == 0)
        {
            SpawnInFreeSlot(m_call);
        }
        return;
    }

    if (m_ct > 10 && m_ct % 50 == 25)
    {
        SpawnInFreeSlot(m_uros);
    }
    if (m_ct > 10 && m_ct % 50 == 0)
    {
        SpawnInFreeSlot(m_call);
    }
    if (m_ct > 300 && m_ct % 70 == 0)
    {
        SpawnInFreeSlot(m_starman);
    }
}

void GameplayScene::UpdateActors()
{
    Player& player = m_game.GetPlayer();
    player.Update(m_ct);

    for (Star& star: m_stars)
    {
        star.Update(player.GetX(), player.GetY());
    }

    for (Enemy* enemy: m_enemies)
    {
        if (enemy->IsAlive())
        {
            enemy->Update(player, player.GetBullets());
        }
    }
    for (Enemy* enemy: m_enemies)
    {
        if (enemy->IsAlive())
        {
            enemy->Fire(m_enemyBullets, m_enemyBulletCursor);
        }
    }

    for (EnemyBullet& bullet: m_enemyBullets)
    {
        if (bullet.IsAlive())
        {
            bullet.Update();
        }
    }
}

void GameplayScene::ResolveContactDamage()
{
    Player& player = m_game.GetPlayer();
    if (player.IsInvincible())
    {
        return;
    }

    for (const Enemy* enemy: m_enemies)
    {
        if (enemy->IsAlive() && enemy->CollidesWithPlayer(player))
        {
            player.TakeDamage();
            return;
        }
    }
    for (EnemyBullet& bullet: m_enemyBullets)
    {
        if (bullet.CollidesWithPlayer(player))
        {
            bullet.Kill();
            player.TakeDamage();
            return;
        }
    }
}

void GameplayScene::CollectScores()
{
    int score = 0;
    for (Enemy* enemy: m_enemies)
    {
        if (enemy->ConsumePendingScore(score))
        {
            m_game.Score() += score;
        }
    }
}

void GameplayScene::Render()
{
    Console& console = m_game.GetConsole();
    console.Clear();

    for (const Star& star: m_stars)
    {
        star.Render(console);
    }

    Player& player = m_game.GetPlayer();
    player.Render(console);

    for (const Enemy* enemy: m_enemies)
    {
        enemy->Render(console);
    }
    for (const EnemyBullet& bullet: m_enemyBullets)
    {
        bullet.Render(console);
    }

    console.PrintAt(0, 0, BuildHudHeader(m_game.PlayerName(), player.GetLife(), player.GetMaxLife(), m_game.Score(), m_game.CurrentStage()), eColor::LightGray);
    console.PrintAt(3, kFieldHeight - 2, BuildProgressBar(m_ct, kStageClearTick), eColor::LightGray);
}

eSceneId GameplayScene::Update()
{
    ++m_ct;

    SpawnWave();
    UpdateActors();
    ResolveContactDamage();
    CollectScores();
    Render();

    Player& player = m_game.GetPlayer();
    if (player.IsDead())
    {
        m_game.SetResult(false);
        return eSceneId::Result;
    }

    bool bStageFinished = (m_ct >= kStageClearTick);

    while (_kbhit())
    {
        if (ReadKey() == kKeyEnter && kEnableStageSkip)
        {
            bStageFinished = true;
        }
    }

    if (bStageFinished)
    {
        if (m_game.CurrentStage() == 1)
        {
            m_game.CurrentStage() = 2;
            return eSceneId::StageIntro;
        }
        m_game.SetResult(true);
        return eSceneId::Result;
    }

    return eSceneId::None;
}
