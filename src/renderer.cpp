#include "renderer.h"
#include "map.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <cmath>

#include "SFML/Graphics/VertexArray.hpp"

constexpr float PI = 3.14159265359f;
constexpr size_t MAX_RAYCASTING_DEPTH = 16;

void Renderer::drawRays(sf::RenderTarget &target, const Player &player, const Map &map) {
  const float angle = player.angle * PI / 180.0f;
  const float vTan = -std::tan(angle), hTan = -1.0f / std::tan(angle);
  const float cellSize = map.getCellSize();
  const auto &grid = map.getGrid();

  size_t vdof = 0, hdof = 0;
  float hdist = std::numeric_limits<float>::max();
  float vdist = std::numeric_limits<float>::max();

  sf::Vector2f vRayPos, hRayPos, offset;

  // Vertical
  if (std::cos(angle) > 0.001f) {
    vRayPos.x = std::floor(player.position.x / cellSize) * cellSize + cellSize;
    vRayPos.y = (player.position.x - vRayPos.x) * vTan + player.position.y;
    offset.x = cellSize;
    offset.y = -offset.x * vTan;
  } else if (std::cos(angle) < -0.001f) {
    vRayPos.x = std::floor(player.position.x / cellSize) * cellSize - 0.01f;
    vRayPos.y = (player.position.x - vRayPos.x) * vTan + player.position.y;
    offset.x = -cellSize;
    offset.y = -offset.x * vTan;
  } else {
    vdof = MAX_RAYCASTING_DEPTH;
  }


  for (; vdof < MAX_RAYCASTING_DEPTH; vdof++) {
    int mapX = (int) (vRayPos.x / cellSize);
    int mapY = (int) (vRayPos.y / cellSize);

    if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {\
      vdist = std::sqrt(
        (vRayPos.x - player.position.x) * (vRayPos.x - player.position.x) +
        (vRayPos.y - player.position.y) * (vRayPos.y - player.position.y)
      );
      break;
    }

    vRayPos += offset;
  }

  // Horizontal
  if (std::sin(angle) > 0.001f) {
    hRayPos.y = std::floor(player.position.y / cellSize) * cellSize + cellSize;
    hRayPos.x = (player.position.y - hRayPos.y) * hTan + player.position.x;
    offset.y = cellSize;
    offset.x = -offset.y * hTan;
  } else if (std::sin(angle) < -0.001f) {
    hRayPos.y = std::floor(player.position.y / cellSize) * cellSize - 0.01f;
    hRayPos.x = (player.position.y - hRayPos.y) * hTan + player.position.x;
    offset.y = -cellSize;
    offset.x = -offset.y * hTan;
  } else {
    hdof = MAX_RAYCASTING_DEPTH;
  }


  for (; hdof < MAX_RAYCASTING_DEPTH; hdof++) {
    int mapX = (int) (hRayPos.x / cellSize);
    int mapY = (int) (hRayPos.y / cellSize);

    if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {
      hdist = std::sqrt(
        (hRayPos.x - player.position.x) * (hRayPos.x - player.position.x) +
        (hRayPos.y - player.position.y) * (hRayPos.y - player.position.y)
      );
    }

    hRayPos += offset;
  }

  sf::VertexArray line(sf::PrimitiveType::Lines, 2);
  line[0].position = player.position;
  line[1].position = hdist < vdist ? hRayPos : vRayPos;

  target.draw(line);
}
