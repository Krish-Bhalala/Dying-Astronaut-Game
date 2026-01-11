#ifndef ASTRONAUT_HPP
#define ASTRONAUT_HPP

#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

class Astronaut {
public:
  sf::Texture texHealthy, texDamaged, texBroken;
  std::unique_ptr<sf::Sprite> body;

  float angle = static_cast<float>(rand() % 360);
  float rotationSpeed = ROTATION_SPEED;
  sf::Vector2f velocity{0.f, 0.f};
  float thrustPower = THRUST_POWER;

  float oxygen = OXYGEN_MAX;
  float oxygenDrainRate = OXYGEN_DRAIN_NORMAL;
  float thrustCapacity = THRUST_CAPACITY_MAX;
  float thrustDrainRate = THRUST_DRAIN_RATE;
  bool isDead = false;

  int currentShipState = 0; // 0=healthy, 1=damaged, 2=broken
  bool isCurrentlyThrusting = false;

  Astronaut() {
    if (!texHealthy.loadFromFile(TEX_SHIP_HEALTHY)) {
      std::cerr << "Warning: Could not load " << TEX_SHIP_HEALTHY << std::endl;
    }
    if (!texDamaged.loadFromFile(TEX_SHIP_DAMAGED)) {
      std::cerr << "Warning: Could not load " << TEX_SHIP_DAMAGED << std::endl;
    }
    if (!texBroken.loadFromFile(TEX_SHIP_BROKEN)) {
      std::cerr << "Warning: Could not load " << TEX_SHIP_BROKEN << std::endl;
    }

    body = std::make_unique<sf::Sprite>(texHealthy);

    sf::Vector2u texSize = texHealthy.getSize();
    float scale = (ASTRO_RADIUS * 2.0f) / static_cast<float>(texSize.x);
    body->setScale({scale, scale});
    body->setOrigin({texSize.x / 2.0f, texSize.y / 2.0f});
    body->setPosition({ASTRO_START_POS_X, ASTRO_START_POS_Y});
  }

  void updateTexture() {
    // Texture switching logic driven by oxygen levels
    int newState;
    if (oxygen > SHIP_HEALTHY_THRESHOLD) {
      newState = 0;
    } else if (oxygen > SHIP_DAMAGED_THRESHOLD) {
      newState = 1;
    } else {
      newState = 2;
    }

    if (newState != currentShipState) {
      currentShipState = newState;
      sf::Vector2u texSize;

      switch (currentShipState) {
      case 0:
        body->setTexture(texHealthy);
        texSize = texHealthy.getSize();
        break;
      case 1:
        body->setTexture(texDamaged);
        texSize = texDamaged.getSize();
        break;
      case 2:
        body->setTexture(texBroken);
        texSize = texBroken.getSize();
        break;
      }

      // Re-center origin for geometric consistency
      body->setOrigin({texSize.x / 2.0f, texSize.y / 2.0f});
      float scale = (ASTRO_RADIUS * 2.0f) / static_cast<float>(texSize.x);
      body->setScale({scale, scale});
    }
  }

  void update(float dt, bool isThrusting) {
    if (isDead)
      return;

    isCurrentlyThrusting = isThrusting && has_thrust();

    // Temporal oxygen depletion
    deplet_oxygen(dt * oxygenDrainRate);

    updateTexture();

    // Impulse logic with engine degradation
    if (has_thrust()) {
      if (isThrusting) {
        float rad = angle * (3.14159f / 180.0f);
        sf::Vector2f thrustDir{std::cos(rad), std::sin(rad)};

        // Apply force scaled by engine integrity
        velocity += thrustDir *
                    (thrustPower * (thrustCapacity / THRUST_CAPACITY_MAX)) * dt;

        // Permanent engine wear
        deplet_thrust(thrustDrainRate * dt);
      } else {
        angle += rotationSpeed * dt;
        if (angle > 360.f)
          angle -= 360.f;
      }
    }

    // Integrate velocity to update position
    body->move(velocity * dt);

    // Periodic boundary conditions
    sf::Vector2f pos = body->getPosition();
    if (pos.x < 0)
      pos.x = WINDOW_WIDTH;
    if (pos.x > WINDOW_WIDTH)
      pos.x = 0;
    if (pos.y < 0)
      pos.y = WINDOW_HEIGHT;
    if (pos.y > WINDOW_HEIGHT)
      pos.y = 0;
    body->setPosition(pos);

    // Align sprite orientation with thrust vector
    body->setRotation(sf::degrees(angle + 180.0f));
  }

  void kill_thruster() {
    thrustCapacity = 0;
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

  // Physics interface
  float getRadius() const { return ASTRO_RADIUS; }
  sf::Vector2f getPosition() const { return body->getPosition(); }

  void reset() {
    // Reset position
    body->setPosition({ASTRO_START_POS_X, ASTRO_START_POS_Y});

    // Reset physics
    angle = 0.0f;
    rotationSpeed = ROTATION_SPEED;
    velocity = {0.f, 0.f};
    thrustPower = THRUST_POWER;

    // Reset survival stats
    oxygen = OXYGEN_MAX;
    oxygenDrainRate = OXYGEN_DRAIN_NORMAL;
    thrustCapacity = THRUST_CAPACITY_MAX;
    thrustDrainRate = THRUST_DRAIN_RATE;
    isDead = false;

    // Reset textures
    currentShipState = 0;
    body->setTexture(texHealthy);
    body->setRotation(sf::degrees(0.f));
  }

  // Render procedural engine exhaust
  void drawThruster(sf::RenderWindow &window) {
    if (!has_thrust())
      return;

    sf::Vector2f pos = body->getPosition();
    float rotatedAngle = angle + 180.0f;
    float rad = rotatedAngle * (3.14159f / 180.0f);
    sf::Vector2f perpendicular(-std::sin(rad), std::cos(rad));

    // NOZZLE
    float nLen = 8.0f;
    float nWid = 6.0f;
    sf::ConvexShape nozzle;
    nozzle.setPointCount(4);
    sf::Vector2f nOffset(std::cos(rad) * nLen, std::sin(rad) * nLen);

    nozzle.setPoint(0, pos + perpendicular * nWid);
    nozzle.setPoint(1, pos - perpendicular * nWid);
    nozzle.setPoint(2, pos + nOffset - perpendicular * (nWid * 0.8f));
    nozzle.setPoint(3, pos + nOffset + perpendicular * (nWid * 0.8f));

    if (isCurrentlyThrusting) {
      nozzle.setFillColor(sf::Color(100, 100, 110)); // Active grey
    } else {
      nozzle.setFillColor(sf::Color(30, 30, 35)); // Cold dark grey
    }
    window.draw(nozzle);

    if (isCurrentlyThrusting) {
      float tRatio = thrustCapacity / THRUST_CAPACITY_MAX;
      float maxLen = POINTER_WIDTH * 1.5f * tRatio;
      float maxWid = 10.0f;

      // Flickering physics
      static float time = 0.0f;
      time += 0.4f;
      float flicker = 0.95f + (std::sin(time) * 0.05f);

      sf::Color pColor = (currentShipState < 2) ? sf::Color(0, 255, 255)
                                                : sf::Color(255, 100, 0);

      // Three-layer bloom/glow
      for (int i = 3; i > 0; i--) {
        sf::ConvexShape plume;
        plume.setPointCount(4);
        float s = i * 0.33f;
        float l = maxLen * s * flicker;
        float w = maxWid * s;

        sf::Vector2f tip(std::cos(rad) * l, std::sin(rad) * l);
        plume.setPoint(0, pos + perpendicular * w);
        plume.setPoint(1, pos - perpendicular * w);
        plume.setPoint(2, pos + tip);
        plume.setPoint(3, pos + tip);

        sf::Color c = pColor;
        c.a = static_cast<std::uint8_t>(80 / i);
        plume.setFillColor(c);
        window.draw(plume);
      }

      // High-intensity plasma core
      sf::ConvexShape core;
      core.setPointCount(3);
      core.setPoint(0, pos + perpendicular * (maxWid * 0.3f));
      core.setPoint(1, pos - perpendicular * (maxWid * 0.3f));
      core.setPoint(2, pos + sf::Vector2f(std::cos(rad) * (maxLen * 0.4f),
                                          std::sin(rad) * (maxLen * 0.4f)));
      core.setFillColor(sf::Color::White);
      window.draw(core);
    } else {
      sf::CircleShape dot(2.0f);
      dot.setOrigin({2.0f, 2.0f});
      dot.setPosition(pos + nOffset * 0.6f);
      dot.setFillColor(sf::Color(80, 80, 80, 100));
      window.draw(dot);
    }
  }

  void reset_state() {
    velocity = {0.f, 0.f};
    angle = 0.0f;
    rotationSpeed = ROTATION_SPEED;
    thrustCapacity = THRUST_CAPACITY_MAX;
    thrustDrainRate = THRUST_DRAIN_RATE;
    isDead = false;
    currentShipState = 0;
    body->setTexture(texHealthy);
    body->setRotation(sf::degrees(0.f));
  }

  void draw(sf::RenderWindow &window) {
    drawThruster(window);
    window.draw(*body);
  }
};

#endif