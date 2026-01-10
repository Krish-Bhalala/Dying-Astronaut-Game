#include "Astronaut.hpp"
#include "Goal.hpp"
#include "HUD.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

enum class GameState { Playing, Won, Lost };

int main() {
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Space Drift");
  window.setFramerateLimit(60);

  Astronaut player;
  HUD hud;
  Goal rescueShip;
  GameState state = GameState::Playing;
  sf::Clock clock;

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();

    while (auto eventOpt = window.pollEvent()) {
      if (eventOpt->is<sf::Event::Closed>())
        window.close();
    }

    if (state == GameState::Playing) {
      bool isSpaceHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

      player.update(dt, isSpaceHeld);
      hud.update(player);
      rescueShip.checkCollision(player.body);

      // Logic Checks
      if (rescueShip.isReached)
        state = GameState::Won;
      else if (player.isDead)
        state = GameState::Lost;
    }

    window.clear(sf::Color(5, 5, 15));

    rescueShip.draw(window);
    player.draw(window);
    hud.draw(window);

    // Simple Overlay for Win/Loss
    if (state == GameState::Won) {
      window.clear(sf::Color::Green);
    } else if (state == GameState::Lost) {
      window.clear(sf::Color::Red);
    }

    window.display();
  }
  return 0;
}