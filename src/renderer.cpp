#include "renderer.h"
#include "map.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <cmath>


constexpr float PI = 3.14159265359f;
constexpr float PLAYER_FOV = 60.0f;
constexpr size_t NUM_RAYS = 600;
constexpr size_t MAX_RAYCASTING_DEPTH = 64;
constexpr float COLUMN_WIDTH = SCREEN_W / (float) NUM_RAYS;


struct Ray {
  sf::Vector2f hitPosition;
  sf::Vector2u mapPosition;
  float distance;
  bool hit;
  bool isHitVertical;
};

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


  float angle = player.angle - PLAYER_FOV / 2.f;
  float maxRenderDistance = MAX_RAYCASTING_DEPTH * map.getCellSize();
  float maxFogDistance = maxRenderDistance / 4.f;
  float angleIncrement = PLAYER_FOV / (float) NUM_RAYS;

  for (size_t i = 0; i < NUM_RAYS; i++, angle += angleIncrement) {
    Ray ray = castRay(player.position, angle, map);

    if (ray.hit) {
      ray.distance *= std::cos((player.angle - angle) * PI / 180.f);
      float wallHeight = (map.getCellSize() * SCREEN_H) / ray.distance;
      if (wallHeight > SCREEN_H) {
        wallHeight = SCREEN_H;
      }

      float brightness = std::max(1.f - (ray.distance / maxRenderDistance), 0.f);
      float shade = (ray.isHitVertical ? 0.8f : 1.f) * brightness;
      // float shade = (ray.isHitVertical ? 0.8f : 1.f); // without brightness
      float wallOffset = SCREEN_H / 2.f - wallHeight / 2.f;

      sf::RectangleShape column(sf::Vector2f(COLUMN_WIDTH, wallHeight));
      column.setPosition(sf::Vector2f(i * COLUMN_WIDTH, wallOffset));

      float fogPercentage = std::max(std::min(ray.distance / maxFogDistance, 1.f), 0.f);

      sf::Color color = map.getGrid()[ray.mapPosition.y][ray.mapPosition.x];
      sf::Color colorWithShade = sf::Color(color.r * shade, color.g * shade, color.b * shade);
      column.setFillColor(sf::Color((1.f - fogPercentage) * colorWithShade.r + fogPercentage * SKY_COLOR.r,
                                    (1.f - fogPercentage) * colorWithShade.g + fogPercentage * SKY_COLOR.g,
                                    (1.f - fogPercentage) * colorWithShade.b + fogPercentage * SKY_COLOR.b));
      target.draw(column);
    }
  }
}

void Renderer::drawRays(sf::RenderTarget &target, const Player &player, const Map &map) {
  for (float angle = player.angle - PLAYER_FOV / 2.f; angle < player.angle + PLAYER_FOV; angle += 0.5f) {
    Ray ray = castRay(player.position, angle, map);

    if (ray.hit) {
      sf::VertexArray line(sf::PrimitiveType::Lines, 2);
      line[0].position = player.position;
      line[1].position = ray.hitPosition;

      target.draw(line);
    }
  }
}


Ray castRay(sf::Vector2f start, float angleInDegrees, const Map &map) {
  const float angle = angleInDegrees * PI / 180.0f;
  const float vTan = -std::tan(angle), hTan = -1.0f / std::tan(angle);
  const float cellSize = map.getCellSize();
  const auto &grid = map.getGrid();

  size_t vDof = 0, hDof = 0;
  float hDist = std::numeric_limits<float>::max();
  float vDist = std::numeric_limits<float>::max();

  bool hit = false;

  sf::Vector2u vMapPos, hMapPos;
  sf::Vector2f vRayPos, hRayPos, offset;

  // Vertical
  if (std::cos(angle) > 0.001f) {
    vRayPos.x = std::floor(start.x / cellSize) * cellSize + cellSize;
    vRayPos.y = (start.x - vRayPos.x) * vTan + start.y;
    offset.x = cellSize;
    offset.y = -offset.x * vTan;
  } else if (std::cos(angle) < -0.001f) {
    vRayPos.x = std::floor(start.x / cellSize) * cellSize - 0.01f;
    vRayPos.y = (start.x - vRayPos.x) * vTan + start.y;
    offset.x = -cellSize;
    offset.y = -offset.x * vTan;
  } else {
    vDof = MAX_RAYCASTING_DEPTH;
  }


  for (; vDof < MAX_RAYCASTING_DEPTH; vDof++) {
    int mapX = (int) (vRayPos.x / cellSize);
    int mapY = (int) (vRayPos.y / cellSize);

    if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX] != sf::Color::Black) {
      hit = true;
      vDist = std::sqrt(
        (vRayPos.x - start.x) * (vRayPos.x - start.x) +
        (vRayPos.y - start.y) * (vRayPos.y - start.y)
      );
      vMapPos = sf::Vector2u(mapX, mapY);
      break;
    }

    vRayPos += offset;
  }

  // Horizontal
  if (std::sin(angle) > 0.001f) {
    hRayPos.y = std::floor(start.y / cellSize) * cellSize + cellSize;
    hRayPos.x = (start.y - hRayPos.y) * hTan + start.x;
    offset.y = cellSize;
    offset.x = -offset.y * hTan;
  } else if (std::sin(angle) < -0.001f) {
    hRayPos.y = std::floor(start.y / cellSize) * cellSize - 0.01f;
    hRayPos.x = (start.y - hRayPos.y) * hTan + start.x;
    offset.y = -cellSize;
    offset.x = -offset.y * hTan;
  } else {
    hDof = MAX_RAYCASTING_DEPTH;
  }


  for (; hDof < MAX_RAYCASTING_DEPTH; hDof++) {
    int mapX = (int) (hRayPos.x / cellSize);
    int mapY = (int) (hRayPos.y / cellSize);

    if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX] != sf::Color::Black) {
      hit = true;
      hDist = std::sqrt(
        (hRayPos.x - start.x) * (hRayPos.x - start.x) +
        (hRayPos.y - start.y) * (hRayPos.y - start.y)
      );
      hMapPos = sf::Vector2u(mapX, mapY);
      break;
    }

    hRayPos += offset;
  }

  return Ray{
    hDist < vDist
      ? hRayPos
      : vRayPos,
    hDist < vDist
      ? hMapPos
      : vMapPos,
    std::min(hDist, vDist),
    hit,
    vDist < hDist
  };
}
