#include "Astronaut.hpp"
#include "Constants.h"
#include "Goal.hpp"
#include "HUD.hpp"
#include "Obstacle.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

void handleCollision(Astronaut &a, Obstacle &o) {
  sf::Vector2f diff = a.body.getPosition() - o.shape.getPosition();
  float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
  float minDistance = a.body.getRadius() + o.shape.getRadius();

  if (distance < minDistance) {
    // Resolve Overlap (prevent objects getting stuck inside each other)
    sf::Vector2f normal = diff / distance;
    float overlap = minDistance - distance;
    a.body.move(normal * overlap);

    // Elastic Bounce
    // Reflect the astronaut's velocity based on the impact normal
    float dot = a.velocity.x * normal.x + a.velocity.y * normal.y;
    a.velocity -= COLLISION_BOUNCE_FACTOR * dot * normal;

    // Add a slight "kick" from the asteroid's own speed
    a.velocity += o.velocity * COLLISION_KICK_FACTOR;

    a.deplet_oxygen(OXYGEN_DRAIN_COLLISION);
  }
}

int main() {
  sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                          WINDOW_TITLE);
  window.setFramerateLimit(FRAMERATE_LIMIT);

  Astronaut player;
  HUD hud;
  Goal rescueShip;
  sf::Clock clock;

  // Create a belt of asteroids
  std::vector<Obstacle> asteroids;
  asteroids.push_back(Obstacle({200, 150}, {50, 20}, {50, 20}, 30));
  asteroids.push_back(Obstacle({500, 400}, {-30, 60}, {30, 10}, 45));
  asteroids.push_back(Obstacle({100, 500}, {40, -40}, {20, 20}, 25));

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();
    while (auto eventOpt = window.pollEvent()) {
      if (eventOpt->is<sf::Event::Closed>())
        window.close();
    }

    // Update
    bool is_key_pressed =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) &&
        player.has_thrust();
    player.update(dt, is_key_pressed);
    for (auto &ast : asteroids) {
      ast.update(dt);
      handleCollision(player, ast);
    }
    hud.update(player);
    rescueShip.checkCollision(player.body);
    if (rescueShip.isReached) {
      window.close();
    }
    if (player.isDead) {
      window.close();
    }

    // Draw
    window.clear(BACKGROUND_COLOR);
    rescueShip.draw(window);
    for (auto &ast : asteroids)
      ast.draw(window);
    player.draw(window);
    hud.draw(window);
    window.display();
  }
  return 0;
}