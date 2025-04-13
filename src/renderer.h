//
// Created by soloman on 29.03.25.
//

#ifndef _RENDERER_H
#define _RENDERER_H

#include "player.h"
#include "map.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

constexpr int SCREEN_H = 675;
constexpr int SCREEN_W = 1200;


class Renderer {
public:
  Renderer(sf::Texture wallTexture);

  void init();

  void draw3dView(sf::RenderTarget &target, const Player &player, const Map &map);

private:
  sf::Texture wallTexture;

  sf::Sprite wallSprite;
};
#endif // _RENDERER_H
