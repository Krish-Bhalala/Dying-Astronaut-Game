#ifndef GOAL_HPP
#define GOAL_HPP

#include "Constants.h"
#include <SFML/Graphics.hpp>

class Goal {
public:
  sf::CircleShape shape;
  bool isReached = false;

  Goal() {
    shape.setRadius(GOAL_RADIUS);
    shape.setOrigin({GOAL_RADIUS, GOAL_RADIUS});
    shape.setFillColor(GOAL_COLOR);
    shape.setOutlineThickness(GOAL_OUTLINE_THICKNESS);
    shape.setOutlineColor(GOAL_OUTLINE_COLOR);
    shape.setPosition({GOAL_START_POS_X, GOAL_START_POS_Y});
  }

  void checkCollision(const sf::CircleShape &player) {
    if (shape.getGlobalBounds().findIntersection(player.getGlobalBounds())) {
      isReached = true;
    }
  }

  void draw(sf::RenderWindow &window) { window.draw(shape); }
};

#endif