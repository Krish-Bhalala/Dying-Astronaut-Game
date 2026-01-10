#ifndef ASTRONAUT_HPP
#define ASTRONAUT_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

#define ROTATION_SPEED 150.0f // Degrees per second

class Astronaut {
public:
  sf::CircleShape body;
  sf::RectangleShape pointer;
  float angle = 0.0f;
  float rotationSpeed = ROTATION_SPEED;

  Astronaut() {
    // Astronaut Body
    body.setRadius(15.f);
    body.setOrigin({15.f, 15.f});
    body.setFillColor(sf::Color::White);
    body.setPosition({400.f, 300.f});

    // Directional Pointer (The "Arrow")
    pointer.setSize({40.f, 4.f});
    pointer.setOrigin({0.f, 2.f}); // Rotate around the start of the rectangle
    pointer.setFillColor(sf::Color::Cyan);
  }

  void update(float dt, bool isKeyPressed) {
    // If key NOT pressed, rotate. If pressed, "Lock" direction.
    if (!isKeyPressed) {
      angle += rotationSpeed * dt;
      if (angle > 360.f)
        angle -= 360.f;
    }

    pointer.setPosition(body.getPosition());
    pointer.setRotation(sf::degrees(angle));
  }

  void draw(sf::RenderWindow &window) {
    window.draw(pointer);
    window.draw(body);
  }
};

#endif