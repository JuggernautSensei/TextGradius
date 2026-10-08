#pragma once

#ifndef GRAD_ASSERT
#    include <cassert>
#    define GRAD_ASSERT(_x, _msg) assert((_x) && (_msg))
#endif

#ifndef GRAD_FLICKER_RENDER
#    define GRAD_FLICKER_RENDER 1
#endif

constexpr int kFieldWidth  = 100;
constexpr int kFieldHeight = 30;

constexpr int kFps               = 30;
constexpr int kFrameDelayMs      = 1000 / kFps;
constexpr int kBulletEffectTicks = 3;
constexpr int kInvincibleTicks   = 30;

constexpr int kMaxPlayerBullet = 20;
constexpr int kMaxStar         = 50;
constexpr int kMaxUros         = 15;
constexpr int kMaxCall         = 15;
constexpr int kMaxStarman      = 10;
constexpr int kMaxEnemyBullet  = 60;

constexpr int kStageClearTick = 940;
constexpr int kEnemyLife      = 14;

#ifdef _DEBUG
constexpr bool kEnableStageSkip = true;
#else
constexpr bool kEnableStageSkip = false;
#endif

constexpr int kKeyExtended  = 256;
constexpr int kKeyUp        = kKeyExtended + 72;
constexpr int kKeyDown      = kKeyExtended + 80;
constexpr int kKeyEnter     = 13;
constexpr int kKeyBackspace = 8;
constexpr int kKeyEsc       = 27;
