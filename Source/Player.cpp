#include "Player.h"

#include <Windows.h>

const ShipStats& GetShipStats(const eShipType _type)
{
    static const ShipStats kTriangle { 5, 1, 5, 4, "▶", "─", eColor::LightCyan };
    static const ShipStats kClover { 3, 1, 3, 2, "♣", "ㆍ", eColor::Yellow };
    static const ShipStats kDiamond { 4, 1, 7, 6, "◆", "⊙", eColor::White };

    switch (_type)
    {
        case eShipType::Triangle:
            return kTriangle;
        case eShipType::Clover:
            return kClover;
        case eShipType::Diamond:
            return kDiamond;
        default:
            GRAD_ASSERT(false, "Invalid ship type.\n");
            return kTriangle;
    }
}

void Player::Init(const eShipType _type)
{
    m_stats   = GetShipStats(_type);
    m_maxLife = m_stats.maxLife;
    m_shape   = m_stats.shape;
    m_bullets.resize(kMaxPlayerBullet);
    ResetForStage();
}

void Player::ResetForStage()
{
    m_x         = 10;
    m_y         = 15;
    m_life      = m_maxLife;
    m_undieTime = 0;
    m_bAlive    = true;

    for (Bullet& bullet: m_bullets)
    {
        bullet.SetAlive(false);
    }
}

void Player::HandleInput()
{
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        if (m_x > m_stats.speed * 2)
        {
            m_x -= m_stats.speed * 2;
        }
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        if (m_x < kFieldWidth - m_stats.speed * 2)
        {
            m_x += m_stats.speed * 2;
        }
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        if (m_y > m_stats.speed * 2 + 1)
        {
            m_y -= m_stats.speed;
        }
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        if (m_y < kFieldHeight - m_stats.speed * 2 - 2)
        {
            m_y += m_stats.speed;
        }
    }
}

void Player::FireBullet(const int _ct)
{
    if (m_stats.shotSpeed <= 0 || _ct % m_stats.shotSpeed != 0)
    {
        return;
    }

    m_bullets[m_nextBullet].Spawn(m_x + 2, m_y, 3, m_stats.bulletShape, m_stats.bulletColor);
    m_nextBullet = (m_nextBullet + 1) % m_bullets.size();
}

void Player::Update(const int _ct)
{
    HandleInput();
    FireBullet(_ct);

    for (Bullet& bullet: m_bullets)
    {
        if (bullet.IsAlive())
        {
            bullet.Update();
        }
    }

    if (m_undieTime > 0)
    {
        --m_undieTime;
    }
}

void Player::TakeDamage()
{
    --m_life;
    m_undieTime = 10;
}

void Player::Update()
{
}

bool Player::IsInvincible() const
{
    return m_undieTime > 0;
}

bool Player::IsDead() const
{
    return m_life <= 0;
}

int Player::GetLife() const
{
    return m_life;
}

int Player::GetMaxLife() const
{
    return m_maxLife;
}

int Player::GetDamage() const
{
    return m_stats.damage;
}

Vector<Bullet>& Player::GetBullets()
{
    return m_bullets;
}

const Vector<Bullet>& Player::GetBullets() const
{
    return m_bullets;
}

void Player::Render(Console& _console) const
{
    _console.PrintAt(m_x, m_y, m_shape, IsInvincible() ? eColor::Yellow : eColor::White);

    if (IsInvincible())
    {
        _console.PrintAt(m_x - 1, m_y - 1, "피격!", eColor::Yellow);
    }

    for (const Bullet& bullet: m_bullets)
    {
        bullet.Render(_console);
    }
}
