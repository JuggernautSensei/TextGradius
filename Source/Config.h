#pragma once

#ifndef GRAD_ASSERT
#    include <cassert>
#    define GRAD_ASSERT(_x, _msg) assert((_x) && (_msg))
#endif

constexpr int kFieldWidth  = 100;
constexpr int kFieldHeight = 30;

constexpr int kFrameDelayMs      = 1000 / 60;
constexpr int kBulletEffectTicks = 3;

constexpr int kMaxPlayerBullet = 20;
constexpr int kMaxStar         = 50;
constexpr int kMaxUros         = 15;
constexpr int kMaxCall         = 15;
constexpr int kMaxStarman      = 10;
constexpr int kMaxUrosBullet   = 20;
constexpr int kMaxCallBullet   = 40;

constexpr int kStageClearTick = 940;

constexpr int kKeyUp        = 72;
constexpr int kKeyDown      = 80;
constexpr int kKeyEnter     = 13;
constexpr int kKeyBackspace = 8;
constexpr int kKeyEsc       = 27;
