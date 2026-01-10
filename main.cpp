#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode{sf::Vector2u{800, 600}}, "Space Drift");
  window.setFramerateLimit(60);

  while (window.isOpen()) {
    while (auto eventOpt = window.pollEvent()) {
      auto &event = *eventOpt;

      // SFML 3.0 event handling - check subtype with is<>()
      if (event.is<sf::Event::Closed>())
        window.close();
    }

    window.clear(sf::Color::Black);
    window.display();
  }
  return 0;
}
