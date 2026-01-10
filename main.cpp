#include "Astronaut.hpp"
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

    // Elastic Bounce (Simplified: swap/reflect velocity)
    // We reflect the astronaut's velocity based on the impact normal
    float dot = a.velocity.x * normal.x + a.velocity.y * normal.y;
    a.velocity -= 2.0f * dot * normal;

    // Add a slight "kick" from the asteroid's own speed
    a.velocity += o.velocity * 0.5f;
  }
}

int main() {
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Space Drift");
  window.setFramerateLimit(60);

  Astronaut player;
  HUD hud;
  Goal rescueShip;
  sf::Clock clock;

  // Create a belt of asteroids
  std::vector<Obstacle> asteroids;
  asteroids.push_back(Obstacle({200, 150}, {50, 20}, 30));
  asteroids.push_back(Obstacle({500, 400}, {-30, 60}, 45));
  asteroids.push_back(Obstacle({100, 500}, {40, -40}, 25));

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();
    while (auto eventOpt = window.pollEvent()) {
      if (eventOpt->is<sf::Event::Closed>())
        window.close();
    }

    // Update
    player.update(dt, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space));
    for (auto &ast : asteroids) {
      ast.update(dt);
      handleCollision(player, ast);
    }
    hud.update(player);
    rescueShip.checkCollision(player.body);

    // Draw
    window.clear(sf::Color(5, 5, 15));
    rescueShip.draw(window);
    for (auto &ast : asteroids)
      ast.draw(window);
    player.draw(window);
    hud.draw(window);
    window.display();
  }
  return 0;
}