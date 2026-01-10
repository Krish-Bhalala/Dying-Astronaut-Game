#ifndef HUD_HPP
#define HUD_HPP

#include "Astronaut.hpp"
#include <SFML/Graphics.hpp>

class HUD {
  sf::RectangleShape oxygenBar;
  sf::RectangleShape thrustBar;

public:
  HUD() {
    // Oxygen Bar at top
    oxygenBar.setPosition({10.f, 10.f});
    oxygenBar.setFillColor(sf::Color::Red);

    // Thrust Bar below astronaut (or static at bottom)
    thrustBar.setFillColor(sf::Color::Yellow);
  }

  void update(const Astronaut &player) {
    // Map 0-100 to 200px width
    oxygenBar.setSize({player.oxygen * 2.0f, 10.f});

    // Position thrust bar slightly below the player
    thrustBar.setPosition({player.body.getPosition().x - 25.f,
                           player.body.getPosition().y + 25.f});
    thrustBar.setSize({player.thrustCapacity * 0.5f, 5.f});
  }

  void draw(sf::RenderWindow &window) {
    window.draw(oxygenBar);
    window.draw(thrustBar);
  }
};

#endif