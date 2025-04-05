#include "renderer.h"
#include "map.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <cmath>


constexpr float PI = 3.14159265359f;
constexpr float PLAYER_FOV = 60.0f;
constexpr size_t MAX_RAYCASTING_DEPTH = 16;

struct Ray {
  sf::Vector2f hitPosition;
  float distance;
  bool hit;
};

Ray castRay(sf::Vector2f start, float angleInDegrees, const Map &map);

void Renderer::drawRays(sf::RenderTarget &target, const Player &player, const Map &map) {
  for (float angle = player.angle - PLAYER_FOV / 2.0f; angle < player.angle + PLAYER_FOV; angle += 0.5f) {
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

    if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {
      hit = true;
      vDist = std::sqrt(
        (vRayPos.x - start.x) * (vRayPos.x - start.x) +
        (vRayPos.y - start.y) * (vRayPos.y - start.y)
      );
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

    if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {
      hit = true;
      hDist = std::sqrt(
        (hRayPos.x - start.x) * (hRayPos.x - start.x) +
        (hRayPos.y - start.y) * (hRayPos.y - start.y)
      );
      break;
    }

    hRayPos += offset;
  }

  return Ray{
    hDist < vDist
      ? hRayPos
      : vRayPos,
    std::min(hDist, vDist),
    hit
  };
}
