#pragma once

#include "WTexture.h"
#include <memory>

namespace SharedTexture
{
    extern std::shared_ptr<WTexture> tileset;

    //enemies
    extern std::shared_ptr<WTexture> slime;
    extern std::shared_ptr<WTexture> spider;
    extern std::shared_ptr<WTexture> bat;
    extern std::shared_ptr<WTexture> floatingSkull;

    //enemy projectiles
    extern std::shared_ptr<WTexture> spiderShot;
    extern std::shared_ptr<WTexture> floatingSkullShot;
}