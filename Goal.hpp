#ifndef GOAL_HPP
#define GOAL_HPP

#include <SFML/Graphics.hpp>

class Goal {
public:
  sf::RectangleShape shape;
  bool isReached = false;

  Goal() {
    shape.setSize({60.f, 60.f});
    shape.setOrigin({30.f, 30.f});
    shape.setFillColor(sf::Color::Green);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);
    // Position it far from the start
    shape.setPosition({700.f, 500.f});
  }

  void checkCollision(const sf::CircleShape &player) {
    if (shape.getGlobalBounds().findIntersection(player.getGlobalBounds())) {
      isReached = true;
    }
  }

  void draw(sf::RenderWindow &window) { window.draw(shape); }
};

#endif