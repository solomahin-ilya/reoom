//
// Created by soloman on 29.03.25.
//

#ifndef _PLAYER_H
#define _PLAYER_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

class Player {
public:
  void draw(sf::RenderTarget &target);

  void update(float deltaTime);

  sf::Vector2f position;
  float angle;
};


#endif // _PLAYER_H
