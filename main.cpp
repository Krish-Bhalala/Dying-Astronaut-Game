#include "Astronaut.hpp"
#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode({800, 600}),
                          "Space Drift - Sprint 3: Physics");
  window.setFramerateLimit(60);

  Astronaut player;
  sf::Clock clock;

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();

    while (auto eventOpt = window.pollEvent()) {
      if (eventOpt->is<sf::Event::Closed>())
        window.close();
    }

    bool isSpaceHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    player.update(dt, isSpaceHeld);

    window.clear(sf::Color(10, 10, 25));
    player.draw(window);
    window.display();
  }
  return 0;
}