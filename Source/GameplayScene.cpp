#include "GameplayScene.h"

#include <conio.h>

#include "Config.h"
#include "Embed.h"
#include "Game.h"

void GameplayScene::OnEnter()
{
    m_ct = 0;

    m_urosSpawn        = 0;
    m_callSpawn        = 0;
    m_starmanSpawn     = 0;
    m_urosBulletCursor = 0;
    m_callBulletCursor = 0;

    m_stars.assign(kMaxStar, Star {});
    for (Star& star: m_stars)
    {
        star.Init();
    }

    m_uros.assign(kMaxUros, UrosEnemy {});
    m_call.assign(kMaxCall, CallEnemy {});
    m_starman.assign(kMaxStarman, StarmanEnemy {});
    m_urosBullets.assign(kMaxUrosBullet, EnemyBullet {});
    m_callBullets.assign(kMaxCallBullet, EnemyBullet {});

    m_game.GetPlayer().ResetForStage();
}

void GameplayScene::SpawnWave()
{
    if (m_game.CurrentStage() == 1)
    {
        if (m_ct > 10 && m_ct % 40 == 0)
        {
            m_uros[m_urosSpawn].Spawn();
            m_urosSpawn = (m_urosSpawn + 1) % m_uros.size();
        }
        if (m_ct > 400 && m_ct % 50 == 0)
        {
            m_call[m_callSpawn].Spawn();
            m_callSpawn = (m_callSpawn + 1) % m_call.size();
        }
        return;
    }

    if (m_ct > 10 && m_ct % 50 == 25)
    {
        m_uros[m_urosSpawn].Spawn();
        m_urosSpawn = (m_urosSpawn + 1) % m_uros.size();
    }
    if (m_ct > 10 && m_ct % 50 == 0)
    {
        m_call[m_callSpawn].Spawn();
        m_callSpawn = (m_callSpawn + 1) % m_call.size();
    }
    if (m_ct > 300 && m_ct % 70 == 0)
    {
        m_starman[m_starmanSpawn].Spawn();
        m_starmanSpawn = (m_starmanSpawn + 1) % m_starman.size();
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

    for (UrosEnemy& enemy: m_uros)
    {
        if (enemy.IsAlive())
        {
            enemy.Update(player, player.GetBullets());
        }
    }
    for (CallEnemy& enemy: m_call)
    {
        if (enemy.IsAlive())
        {
            enemy.Update(player, player.GetBullets());
        }
    }
    for (StarmanEnemy& enemy: m_starman)
    {
        if (enemy.IsAlive())
        {
            enemy.Update(player, player.GetBullets());
        }
    }

    for (UrosEnemy& enemy: m_uros)
    {
        if (enemy.IsAlive())
        {
            enemy.Fire(m_urosBullets, m_urosBulletCursor);
        }
    }
    for (CallEnemy& enemy: m_call)
    {
        if (enemy.IsAlive())
        {
            enemy.Fire(m_callBullets, m_callBulletCursor);
        }
    }

    for (EnemyBullet& bullet: m_urosBullets)
    {
        if (bullet.IsAlive())
        {
            bullet.Update();
        }
    }
    for (EnemyBullet& bullet: m_callBullets)
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

    bool bHit = false;
    for (const UrosEnemy& enemy: m_uros)
    {
        if (enemy.IsAlive() && enemy.CollidesWithPlayer(player))
        {
            bHit = true;
            break;
        }
    }
    if (!bHit)
    {
        for (const CallEnemy& enemy: m_call)
        {
            if (enemy.IsAlive() && enemy.CollidesWithPlayer(player))
            {
                bHit = true;
                break;
            }
        }
    }
    if (!bHit)
    {
        for (EnemyBullet& bullet: m_urosBullets)
        {
            if (bullet.CollidesWithPlayer(player))
            {
                bullet.Kill();
                bHit = true;
                break;
            }
        }
    }
    if (!bHit)
    {
        for (EnemyBullet& bullet: m_callBullets)
        {
            if (bullet.CollidesWithPlayer(player))
            {
                bullet.Kill();
                bHit = true;
                break;
            }
        }
    }

    if (bHit)
    {
        player.TakeDamage();
    }
}

void GameplayScene::CollectScores()
{
    int score = 0;
    for (UrosEnemy& enemy: m_uros)
    {
        if (enemy.ConsumePendingScore(score))
        {
            m_game.Score() += score;
        }
    }
    for (CallEnemy& enemy: m_call)
    {
        if (enemy.ConsumePendingScore(score))
        {
            m_game.Score() += score;
        }
    }
    for (StarmanEnemy& enemy: m_starman)
    {
        if (enemy.ConsumePendingScore(score))
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

    for (const UrosEnemy& enemy: m_uros)
    {
        enemy.Render(console);
    }
    for (const CallEnemy& enemy: m_call)
    {
        enemy.Render(console);
    }
    for (const StarmanEnemy& enemy: m_starman)
    {
        enemy.Render(console);
    }
    for (const EnemyBullet& bullet: m_urosBullets)
    {
        bullet.Render(console);
    }
    for (const EnemyBullet& bullet: m_callBullets)
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
    if (_kbhit() && _getch() == kKeyEnter)
    {
        bStageFinished = true;
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
