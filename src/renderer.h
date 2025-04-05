//
// Created by soloman on 29.03.25.
//

#ifndef _RENDERER_H
#define _RENDERER_H

#include "player.h"
#include "map.h"

#include <SFML/Graphics/RenderTarget.hpp>


class Renderer {
public:
  void drawRays(sf::RenderTarget &target, const Player &player, const Map &map);

private:
};

#endif // _RENDERER_H
