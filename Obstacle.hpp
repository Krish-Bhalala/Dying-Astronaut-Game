#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Obstacle {
public:
  std::unique_ptr<sf::Sprite> sprite;
  sf::Vector2f velocity;
  float radius;

  // Constructor takes a texture reference from main
  Obstacle(const sf::Texture &texture, sf::Vector2f pos, sf::Vector2f vel,
           float r)
      : velocity(vel), radius(r) {
    // Create sprite AFTER texture is passed
    sprite = std::make_unique<sf::Sprite>(texture);

    // Scale texture to fit radius
    sf::Vector2u texSize = texture.getSize();
    float scale = (radius * 2.0f) / static_cast<float>(texSize.x);
    sprite->setScale({scale, scale});
    sprite->setOrigin({texSize.x / 2.0f, texSize.y / 2.0f});
    sprite->setPosition(pos);
  }

  void update(float dt) {
    sprite->move(velocity * dt);

    // Slow rotation for visual effect
    sprite->rotate(sf::degrees(30.0f * dt));

    // Screen wrap
    sf::Vector2f pos = sprite->getPosition();
    if (pos.x < 0)
      pos.x = WINDOW_WIDTH;
    if (pos.x > WINDOW_WIDTH)
      pos.x = 0;
    if (pos.y < 0)
      pos.y = WINDOW_HEIGHT;
    if (pos.y > WINDOW_HEIGHT)
      pos.y = 0;
    sprite->setPosition(pos);
  }

  sf::Vector2f getPosition() const { return sprite->getPosition(); }
  float getRadius() const { return radius; }

  void draw(sf::RenderWindow &window) { window.draw(*sprite); }
};

#endif