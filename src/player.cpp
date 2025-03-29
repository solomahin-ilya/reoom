#include "player.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/Graphics/RectangleShape.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

constexpr float PI = 3.141592653589793f;
constexpr float TURN_SPEED = 120.0f;
constexpr float MAX_SPEED = 100.0f;

void Player::draw(sf::RenderTarget &target) {
  sf::CircleShape circle(8.0f);
  circle.setOrigin(sf::Vector2(circle.getRadius(), circle.getRadius()));
  circle.setPosition(position);
  circle.setFillColor(sf::Color::Red);

  sf::RectangleShape line(sf::Vector2(24.0f, 2.0f));
  line.setPosition(position);
  sf::Angle lineAngle = sf::degrees(angle);
  line.setRotation(lineAngle);
  line.setFillColor(sf::Color::Blue);

  target.draw(line);
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
