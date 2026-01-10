#ifndef ASTRONAUT_HPP
#define ASTRONAUT_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

#define DEFAULT_ROTATION_SPEED 150.0f // Degrees per second
#define DEFAULT_THRUST_POWER 300.0f

class Astronaut {
public:
  sf::CircleShape body;
  sf::RectangleShape pointer;

  // Physics
  float angle = 0.0f;
  float rotationSpeed = DEFAULT_ROTATION_SPEED;
  sf::Vector2f velocity{0.f, 0.f};          // Current movement vector
  float thrustPower = DEFAULT_THRUST_POWER; // How fast we accelerate

  // Survival Stats
  float oxygen = 100.0f;
  float oxygenDrainRate = 1.f;
  float thrustCapacity = 100.0f;
  float thrustDrainRate = 10.f;
  bool isDead = false;

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

  void update(float dt, bool isThrusting) {
    if (isDead)
      return;

    // Oxygen Drain
    if (oxygen > 0)
      oxygen -= dt * oxygenDrainRate;
    else
      isDead = true;

    // Thrust Logic with Decay
    if (thrustCapacity <= 0) {
      kill_thruster();
    } else if (isThrusting) {
      pointer.setFillColor(sf::Color::Red);
      float rad = angle * (3.14159f / 180.0f);
      sf::Vector2f thrustDir{std::cos(rad), std::sin(rad)};

      // Apply thrust scaled by current capacity
      velocity += thrustDir * (thrustPower * (thrustCapacity / 100.0f)) * dt;

      // Permanent engine wear (0.5% per second of thrust)
      thrustCapacity -= thrustDrainRate * dt;
    } else if (!isThrusting) {
      angle += rotationSpeed * dt;
      if (angle > 360.f)
        angle -= 360.f;
      pointer.setFillColor(sf::Color::Cyan);
    }
    // Apply Velocity to Position (Inertia)
    body.move(velocity * dt);

    // Screen Wrapping
    sf::Vector2f pos = body.getPosition();
    if (pos.x < 0)
      pos.x = 800;
    if (pos.x > 800)
      pos.x = 0;
    if (pos.y < 0)
      pos.y = 600;
    if (pos.y > 600)
      pos.y = 0;
    body.setPosition(pos);

    pointer.setPosition(body.getPosition());
    // make the arrow point in the opposite direction of the velocity for
    // intutitve gameplay
    pointer.setRotation(sf::degrees(angle + 180.0f));
  }

  void kill_thruster() {
    thrustCapacity = 0;
    pointer.setFillColor(sf::Color::Black);
    pointer.setOutlineColor(sf::Color::White);
    pointer.setOutlineThickness(1.f);
    rotationSpeed = 0.f;
    thrustPower = 0.f;
  }

  void draw(sf::RenderWindow &window) {
    window.draw(pointer);
    window.draw(body);
  }
};

#endif