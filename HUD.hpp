#ifndef HUD_HPP
#define HUD_HPP

#include "Astronaut.hpp"
#include "Constants.h"
#include <SFML/Graphics.hpp>

class HUD {
  sf::RectangleShape oxygenBar;
  sf::RectangleShape thrustBar;

public:
  HUD() {
    // Oxygen Bar at top
    oxygenBar.setPosition({HUD_OXYGEN_BAR_POS_X, HUD_OXYGEN_BAR_POS_Y});
    oxygenBar.setFillColor(HUD_OXYGEN_BAR_COLOR);

    // Thrust Bar below astronaut
    thrustBar.setFillColor(HUD_THRUST_BAR_COLOR);
  }

  void update(const Astronaut &player) {
    // Map 0-100 to 200px width
    oxygenBar.setSize(
        {player.oxygen * HUD_OXYGEN_BAR_WIDTH_SCALE, HUD_OXYGEN_BAR_HEIGHT});

    // Position thrust bar slightly below the player
    thrustBar.setPosition(
        {player.body.getPosition().x + HUD_THRUST_BAR_OFFSET_X,
         player.body.getPosition().y + HUD_THRUST_BAR_OFFSET_Y});
    thrustBar.setSize({player.thrustCapacity * HUD_THRUST_BAR_WIDTH_SCALE,
                       HUD_THRUST_BAR_HEIGHT});
  }

  void draw(sf::RenderWindow &window) {
    window.draw(oxygenBar);
    window.draw(thrustBar);
  }
};

#endif