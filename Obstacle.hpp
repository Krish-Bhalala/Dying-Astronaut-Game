#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "Constants.h"
#include <SFML/Graphics.hpp>

class Obstacle {
public:
  sf::CircleShape shape;
  sf::Vector2f velocity;
  sf::Vector2f maxVelocity;
  float radius;

  Obstacle(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f maxVel, float r)
      : velocity(vel), maxVelocity(maxVel), radius(r) {
    shape.setRadius(radius);
    shape.setOrigin({radius, radius});
    shape.setFillColor(OBSTACLE_COLOR);
    shape.setPosition(pos);
    shape.setOutlineThickness(OBSTACLE_OUTLINE_THICKNESS);
    shape.setOutlineColor(OBSTACLE_OUTLINE_COLOR);
  }

  void update(float dt) {
    shape.move(velocity * dt);

    // Screen wrap
    sf::Vector2f pos = shape.getPosition();
    if (pos.x < 0)
      pos.x = WINDOW_WIDTH;
    if (pos.x > WINDOW_WIDTH)
      pos.x = 0;
    if (pos.y < 0)
      pos.y = WINDOW_HEIGHT;
    if (pos.y > WINDOW_HEIGHT)
      pos.y = 0;
    shape.setPosition(pos);
  }

  void draw(sf::RenderWindow &window) { window.draw(shape); }
};

#endif