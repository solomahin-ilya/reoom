#include "player.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

constexpr float PI = 3.141592653589793f;
constexpr float TURN_SPEED = 120.0f;
constexpr float MAX_SPEED = 100.0f;

void Player::draw(sf::RenderTarget &target) {
  sf::CircleShape circle(5.0f);
  circle.setPosition(position);

  target.draw(circle);
}

void Player::update(float deltaTime) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
     angle -= TURN_SPEED * deltaTime;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
     angle += TURN_SPEED * deltaTime;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
     float radians = angle * PI / 180.0f;

     position.x += cos(radians) * MAX_SPEED * deltaTime;
     position.y += sin(radians) * MAX_SPEED * deltaTime;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
     float radians = angle * PI / 180.0f;

     position.x -= cos(radians) * MAX_SPEED * deltaTime;
     position.y -= sin(radians) * MAX_SPEED * deltaTime;
  }
}