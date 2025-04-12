#include "map.h"
#include "player.h"
#include "renderer.h"

#include <SFML/Graphics.hpp>
#include <vector>


int main() {
  auto window = sf::RenderWindow(sf::VideoMode({SCREEN_W, SCREEN_H}), "Reoom", sf::Style::Close | sf::Style::Titlebar);

  std::vector<std::vector<int> > grid = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  };

  // std::vector<std::vector<int> > grid = {
  //   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  // };

  // std::vector<std::vector<int> > grid = {
  //   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
  //   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  // };

  Map map(48.0f, "resources/map.png");
  Player player;
  player.position = sf::Vector2f(70, 70);

  Renderer renderer = Renderer(sf::Texture("resources/cobblestone.png"));
  renderer.init();

  sf::Clock gameClock;
  while (window.isOpen()) {
    float deltaTime = gameClock.restart().asSeconds();

    while (const std::optional event = window.pollEvent()) {
      bool isCloseEvent = event->is<sf::Event::Closed>();
      if (isCloseEvent) {
        window.close();
      }
    }

    player.update(deltaTime);

    window.clear();
    // map.draw(window); // for editor
    // player.draw(window); // for editor
    renderer.draw3dView(window, player, map);
    // renderer.drawRays(window, player, map); // for editor
    window.display();
  }
}
