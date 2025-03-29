#include "map.h"
#include "player.h"

#include <SFML/Graphics.hpp>
#include <vector>

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({1200, 675}), "Reoom");

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

    Map map(48.0f, grid);
    Player player;
    player.position = sf::Vector2f(50, 50);

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
        map.draw(window);
        player.draw(window);
        window.display();
    }
}
