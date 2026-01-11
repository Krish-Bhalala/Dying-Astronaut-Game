#ifndef GOAL_HPP
#define GOAL_HPP

#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>

class Goal {
public:
  sf::Texture texture;
  std::unique_ptr<sf::Sprite> sprite;
  bool isReached = false;
  float rotationSpeed = 45.0f; // Slow spin for wormhole effect

  Goal() {
    // Load wormhole texture FIRST
    if (!texture.loadFromFile(TEX_WORMHOLE)) {
      std::cerr << "Warning: Could not load " << TEX_WORMHOLE << std::endl;
    }

    // NOW create sprite with loaded texture
    sprite = std::make_unique<sf::Sprite>(texture);
    sf::Vector2u texSize = texture.getSize();

    // Scale to desired goal radius
    float scale = (GOAL_RADIUS * 2.0f) / static_cast<float>(texSize.x);
    sprite->setScale({scale, scale});
    sprite->setOrigin({texSize.x / 2.0f, texSize.y / 2.0f});
    sprite->setPosition({GOAL_START_POS_X, GOAL_START_POS_Y});
  }

  void update(float dt) {
    // Rotate wormhole for visual effect
    sprite->rotate(sf::degrees(rotationSpeed * dt));
  }

  void checkCollision(sf::Vector2f playerPos, float playerRadius) {
    sf::Vector2f diff = sprite->getPosition() - playerPos;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    float minDistance = GOAL_RADIUS + playerRadius;

    if (distance < minDistance) {
      isReached = true;
    }
  }

  void reset() { isReached = false; }

  sf::Vector2f getPosition() const { return sprite->getPosition(); }

  void draw(sf::RenderWindow &window) { window.draw(*sprite); }
};

#endif