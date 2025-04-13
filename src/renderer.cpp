#include "renderer.h"
#include "map.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <cmath>
#include <iostream>

constexpr float PI = 3.14159265359f;
constexpr float PLAYER_FOV = 60.0f;
constexpr size_t MAX_RAYCASTING_DEPTH = 64;


struct Ray {
  sf::Vector2f hitPosition;
  sf::Vector2u mapPosition;
  float distance;
  bool hit;
  bool isHitVertical;
};

Renderer::Renderer(sf::Texture wallTexture): wallSprite(sf::Sprite(wallTexture)) {
  // default textures
  if (!wallTexture.loadFromFile("resources/cobblestone.png")) {
    std::cerr << "Failed to load wall texture" << std::endl;
  };

  if (wallTexture.getSize().x != wallTexture.getSize().y) {
    std::cerr << "ERROR: Texture in not square" << std::endl;
  }

  wallSprite = sf::Sprite(wallTexture);
}

void Renderer::init() {
  // current textures
  if (!wallTexture.loadFromFile("resources/cobblestone1.png")) {
    std::cerr << "Failed to load wall sprite" << std::endl;
    return;
  }

  if (wallTexture.getSize().x != wallTexture.getSize().y) {
    std::cerr << "ERROR: Texture in not square" << std::endl;
    return;
  }

  wallSprite = sf::Sprite(wallTexture);
}


Ray castRay(sf::Vector2f start, float angleInDegrees, const Map &map);

void Renderer::draw3dView(sf::RenderTarget &target, const Player &player, const Map &map) {
  const sf::Color SKY_COLOR = sf::Color(100, 170, 250);

  sf::RectangleShape skyBox(sf::Vector2f(SCREEN_W, SCREEN_H / 2));
  sf::RectangleShape floor(sf::Vector2f(SCREEN_W, SCREEN_H / 2));
  floor.setFillColor(sf::Color(80, 80, 80));
  floor.setPosition(sf::Vector2f(0, SCREEN_H / 2));
  skyBox.setFillColor(SKY_COLOR);
  target.draw(skyBox);
  target.draw(floor);

  sf::RectangleShape column{sf::Vector2f(1.f, 1.f)};

  float radians = player.angle * PI / 180.f;
  sf::Vector2f direction{std::cos(radians), std::sin(radians)};
  sf::Vector2f plane{-direction.y, direction.x};

  sf::VertexArray walls{
    sf::PrimitiveType::Lines
  };
  for (size_t i = 0; i < SCREEN_W; i++) {
    float cameraX = i * 2.f / SCREEN_W - 1.f; // -1.f -> 0.f -> 1.f
    sf::Vector2f rayPos = player.position / map.getCellSize();
    sf::Vector2f rayDir = direction + plane * cameraX;

    sf::Vector2f deltaDist{
      std::abs(1.f / rayDir.x),
      std::abs(1.f / rayDir.y),
    };

    sf::Vector2i mapPos{rayPos};
    sf::Vector2i step;
    sf::Vector2f sideDist;

    if (rayDir.x < 0.f) {
      step.x = -1;
      sideDist.x = (-mapPos.x + rayPos.x) * deltaDist.x;
    } else {
      step.x = 1;
      sideDist.x = (mapPos.x - rayPos.x + 1.f) * deltaDist.x;
    }


    if (rayDir.y < 0.f) {
      step.y = -1;
      sideDist.y = (-mapPos.y + rayPos.y) * deltaDist.y;
    } else {
      step.y = 1;
      sideDist.y = (mapPos.y - rayPos.y + 1.f) * deltaDist.y;
    }

    bool didHit{}, isHitVertical{};

    size_t depth = 0;
    while (!didHit && depth < MAX_RAYCASTING_DEPTH) {
      if (sideDist.x < sideDist.y) {
        sideDist.x += deltaDist.x;
        mapPos.x += step.x;
        isHitVertical = false;
      } else {
        sideDist.y += deltaDist.y;
        mapPos.y += step.y;
        isHitVertical = true;
      }

      int x = mapPos.x, y = mapPos.y;
      const auto &grid = map.getGrid();

      if (y >= 0 && y < grid.size() && x >= 0 && x < grid[y].size() && grid[y][x] != sf::Color::Black) {
        didHit = true;
      }

      depth++;
    }

    if (didHit) {
      float perpWallDist = isHitVertical ? sideDist.y - deltaDist.y : sideDist.x - deltaDist.x;
      float wallHeight = SCREEN_H / perpWallDist;

      float wallStart = (-wallHeight + SCREEN_H) / 2.f;
      float wallEnd = (wallHeight + SCREEN_H) / 2.f;

      float textureSize = wallTexture.getSize().x;

      float wallX = isHitVertical ? rayPos.x + perpWallDist * rayDir.x : rayPos.y + perpWallDist * rayDir.y;

      wallX -= std::floor(wallX);
      float textureX = wallX * textureSize;

      float brightness = 1.f - (perpWallDist / (float) MAX_RAYCASTING_DEPTH);

      if (isHitVertical) {
        brightness *= 0.7f;
      }

      sf::Color color = sf::Color(255 * brightness, 255 * brightness, 255 * brightness);

      walls.append(sf::Vertex({sf::Vector2f((float) i, wallStart), color, sf::Vector2f(textureX, 0.f)}));
      walls.append(
        sf::Vertex({sf::Vector2f((float) i, wallEnd), color, sf::Vector2f(textureX, textureSize)}));
    }
  }

  sf::RenderStates states{&wallTexture};
  target.draw(walls, states);
}

