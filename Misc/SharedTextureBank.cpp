#include "SharedTextureBank.h"

std::shared_ptr<WTexture> SharedTexture::tileset{ std::make_shared<WTexture>() };

std::shared_ptr<WTexture> SharedTexture::slime{ std::make_shared<WTexture>() };
std::shared_ptr<WTexture> SharedTexture::spider{ std::make_shared<WTexture>() };
std::shared_ptr<WTexture> SharedTexture::bat{ std::make_shared<WTexture>() };
std::shared_ptr<WTexture> SharedTexture::floatingSkull{ std::make_shared<WTexture>() };

std::shared_ptr<WTexture> SharedTexture::spiderShot{ std::make_shared<WTexture>() };
std::shared_ptr<WTexture> SharedTexture::floatingSkullShot{ std::make_shared<WTexture>() };