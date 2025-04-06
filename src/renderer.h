//
// Created by soloman on 29.03.25.
//

#ifndef _RENDERER_H
#define _RENDERER_H

#include "player.h"
#include "map.h"

#include <SFML/Graphics/RenderTarget.hpp>

constexpr int SCREEN_H = 675;
constexpr int SCREEN_W = 1200;

class Renderer {
public:
  void drawRays(sf::RenderTarget &target, const Player &player, const Map &map);

  void draw3dView(sf::RenderTarget &target, const Player &player, const Map &map);

private:
};

#endif // _RENDERER_H
