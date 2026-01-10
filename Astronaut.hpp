#ifndef ASTRONAUT_HPP
#define ASTRONAUT_HPP

#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class Astronaut {
public:
  sf::CircleShape body;
  sf::RectangleShape pointer;

  // Physics
  float angle = 0.0f;
  float rotationSpeed = ROTATION_SPEED;
  sf::Vector2f velocity{0.f, 0.f};  // Current movement vector
  float thrustPower = THRUST_POWER; // How fast we accelerate

  // Survival Stats
  float oxygen = OXYGEN_MAX;
  float oxygenDrainRate = OXYGEN_DRAIN_NORMAL;
  float thrustCapacity = THRUST_CAPACITY_MAX;
  float thrustDrainRate = THRUST_DRAIN_RATE;
  bool isDead = false;

  Astronaut() {
    // Astronaut Body
    body.setRadius(ASTRO_RADIUS);
    body.setOrigin({ASTRO_RADIUS, ASTRO_RADIUS});
    body.setFillColor(ASTRO_BODY_COLOR);
    body.setPosition({ASTRO_START_POS_X, ASTRO_START_POS_Y});

    // Directional Pointer (The "Arrow")
    pointer.setSize({POINTER_WIDTH, POINTER_HEIGHT});
    pointer.setOrigin(
        {POINTER_ORIGIN_X,
         POINTER_ORIGIN_Y}); // Rotate around the start of the rectangle
    pointer.setFillColor(POINTER_COLOR_IDLE);
  }

  void update(float dt, bool isThrusting) {
    if (isDead)
      return;

    // Oxygen Drain
    deplet_oxygen(dt * oxygenDrainRate);

    // Thrust Logic with Decay
    if (has_thrust()) {
      if (isThrusting) {
        pointer.setFillColor(POINTER_COLOR_THRUST);
        float rad = angle * (3.14159f / 180.0f);
        sf::Vector2f thrustDir{std::cos(rad), std::sin(rad)};

        // Apply thrust scaled by current capacity
        velocity += thrustDir *
                    (thrustPower * (thrustCapacity / THRUST_CAPACITY_MAX)) * dt;

        // Permanent engine wear
        deplet_thrust(thrustDrainRate * dt);
      } else {
        angle += rotationSpeed * dt;
        if (angle > 360.f)
          angle -= 360.f;
        pointer.setFillColor(POINTER_COLOR_IDLE);
      }
    }
    // Apply Velocity to Position (Inertia)
    body.move(velocity * dt);

    // Screen Wrapping
    sf::Vector2f pos = body.getPosition();
    if (pos.x < 0)
      pos.x = WINDOW_WIDTH;
    if (pos.x > WINDOW_WIDTH)
      pos.x = 0;
    if (pos.y < 0)
      pos.y = WINDOW_HEIGHT;
    if (pos.y > WINDOW_HEIGHT)
      pos.y = 0;
    body.setPosition(pos);

    pointer.setPosition(body.getPosition());
    // make the arrow point in the opposite direction of the velocity for
    // intutitve gameplay
    pointer.setRotation(sf::degrees(angle + 180.0f));
  }

  void kill_thruster() {
    thrustCapacity = 0;
    pointer.setFillColor(POINTER_COLOR_DEAD);
    pointer.setOutlineColor(POINTER_OUTLINE_COLOR);
    pointer.setOutlineThickness(1.f);
    rotationSpeed = 0.f;
    thrustPower = 0.f;
  }

  void deplet_oxygen(float value) {
    if (oxygen > 0)
      oxygen -= value;
    if (oxygen <= 0)
      isDead = true;
  }

  void deplet_thrust(float value) {
    if (thrustCapacity > 0)
      thrustCapacity -= value;
    if (thrustCapacity <= 0)
      kill_thruster();
  }

  bool has_thrust() { return thrustCapacity > 0; }

  void draw(sf::RenderWindow &window) {
    window.draw(pointer);
    window.draw(body);
  }
};

#endif