#pragma once
#include "CallEnemy.h"
#include "Scene.h"
#include "Star.h"
#include "StarmanEnemy.h"
#include "UrosEnemy.h"

class GameplayScene : public Scene
{
public:
    using Scene::Scene;

    void     OnEnter() override;
    eSceneId Update() override;

private:
    void SpawnWave();
    void UpdateActors();
    void ResolveContactDamage();
    void CollectScores();
    void Render();

    int m_ct = 0;

    Vector<Star>         m_stars;
    Vector<UrosEnemy>    m_uros;
    Vector<CallEnemy>    m_call;
    Vector<StarmanEnemy> m_starman;
    Vector<Enemy*>       m_enemies;
    Vector<EnemyBullet>  m_enemyBullets;

    size_t m_enemyBulletCursor = 0;
};
