#pragma once
#include "Bullet.h"
#include "Config.h"
#include "GameObject.h"

enum class eShipType
{
    Triangle,
    Clover,
    Diamond,
};

struct ShipStats
{
    int    maxLife   = 0;
    int    speed     = 0;
    int    damage    = 0;
    int    shotSpeed = 0;
    String shape;
    String bulletShape;
    eColor bulletColor = eColor::White;
};

[[nodiscard]] const ShipStats& GetShipStats(eShipType _type);

class Player : public GameObject
{
public:
    void Init(eShipType _type);
    void ResetForStage();

    void Update(int _ct);
    void Render(Console& _console) const override;

    void TakeDamage();

    [[nodiscard]] bool IsInvincible() const;
    [[nodiscard]] bool IsDead() const;

    [[nodiscard]] int GetLife() const;
    [[nodiscard]] int GetMaxLife() const;
    [[nodiscard]] int GetDamage() const;

    [[nodiscard]] Vector<Bullet>&       GetBullets();
    [[nodiscard]] const Vector<Bullet>& GetBullets() const;

private:
    void Update() override;
    void HandleInput();
    void FireBullet(int _ct);

    ShipStats      m_stats;
    int            m_maxLife    = 0;
    int            m_life       = 0;
    int            m_undieTime  = 0;
    size_t         m_nextBullet = 0;
    Vector<Bullet> m_bullets;
};
