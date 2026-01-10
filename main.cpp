#include "Astronaut.hpp"
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define REFRESH_RATE 60

int main() {
  sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                          "Space Drift - Sprint 2");
  window.setFramerateLimit(REFRESH_RATE);

  Astronaut player;
  sf::Clock clock;

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();

    while (auto eventOpt = window.pollEvent()) {
      if (eventOpt->is<sf::Event::Closed>())
        window.close();
    }

    // Logic: Is the "Thrust" key held?
    bool isSpaceHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    player.update(dt, isSpaceHeld);

    window.clear(sf::Color(10, 10, 25)); // Deep Space Blue
    player.draw(window);
    window.display();
  }
  return 0;
}