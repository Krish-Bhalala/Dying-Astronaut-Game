#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <SFML/Graphics.hpp>

class Obstacle {
public:
  sf::CircleShape shape;
  sf::Vector2f velocity;
  float radius;

  Obstacle(sf::Vector2f pos, sf::Vector2f vel, float r)
      : velocity(vel), radius(r) {
    shape.setRadius(radius);
    shape.setOrigin({radius, radius});
    shape.setFillColor(sf::Color(100, 100, 120)); // Stony grey
    shape.setPosition(pos);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(150, 150, 150));
  }

  void update(float dt) {
    shape.move(velocity * dt);

    // Screen wrap
    sf::Vector2f pos = shape.getPosition();
    if (pos.x < 0)
      pos.x = 800;
    if (pos.x > 800)
      pos.x = 0;
    if (pos.y < 0)
      pos.y = 600;
    if (pos.y > 600)
      pos.y = 0;
    shape.setPosition(pos);
  }

  void draw(sf::RenderWindow &window) { window.draw(shape); }
};

#endif