#include "Astronaut.hpp"
#include "AudioManager.hpp"
#include "Constants.h"
#include "Goal.hpp"
#include "HUD.hpp"
#include "Obstacle.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

void handleCollision(Astronaut &a, Obstacle &o, AudioManager &audioMgr) {
  sf::Vector2f diff = a.getPosition() - o.getPosition();
  float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
  float minDistance = a.getRadius() + o.getRadius();

  if (distance < minDistance) {
    // Resolve Overlap (prevent objects getting stuck inside each other)
    sf::Vector2f normal = diff / distance;
    float overlap = minDistance - distance;
    a.body->move(normal * overlap);

    // Elastic Bounce
    float dot = a.velocity.x * normal.x + a.velocity.y * normal.y;
    a.velocity -= COLLISION_BOUNCE_FACTOR * dot * normal;

    // Add a slight "kick" from the asteroid's own speed
    a.velocity += o.velocity * COLLISION_KICK_FACTOR;

    a.deplet_oxygen(OXYGEN_DRAIN_COLLISION);

    // Play collision sound
    audioMgr.playCollision();
  }
}

int main() {
  sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
                          WINDOW_TITLE);
  window.setFramerateLimit(FRAMERATE_LIMIT);

  // Load background texture
  sf::Texture backgroundTexture;
  if (!backgroundTexture.loadFromFile(TEX_BACKGROUND)) {
    std::cerr << "Warning: Could not load " << TEX_BACKGROUND << std::endl;
  }
  sf::Sprite background(backgroundTexture);
  // Scale background to fit window
  sf::Vector2u bgSize = backgroundTexture.getSize();
  background.setScale({static_cast<float>(WINDOW_WIDTH) / bgSize.x,
                       static_cast<float>(WINDOW_HEIGHT) / bgSize.y});

  // Load asteroid textures (4 variants)
  sf::Texture asteroidTex1, asteroidTex2, asteroidTex3, asteroidTex4;
  asteroidTex1.loadFromFile(TEX_ASTEROID_1);
  asteroidTex2.loadFromFile(TEX_ASTEROID_2);
  asteroidTex3.loadFromFile(TEX_ASTEROID_3);
  asteroidTex4.loadFromFile(TEX_ASTEROID_4);

  // Game objects
  Astronaut player;
  HUD hud;
  Goal wormhole;
  AudioManager audioManager;
  sf::Clock clock;

  // Start background music
  audioManager.startBackgroundMusic();

  // Create asteroids with different textures
  std::vector<Obstacle> asteroids;
  asteroids.push_back(Obstacle(asteroidTex1, {200, 150}, {50, 20}, 30));
  asteroids.push_back(Obstacle(asteroidTex2, {500, 400}, {-30, 60}, 45));
  asteroids.push_back(Obstacle(asteroidTex3, {100, 500}, {40, -40}, 25));
  asteroids.push_back(Obstacle(asteroidTex4, {650, 200}, {-20, 30}, 35));

  // Game state
  int gameState = GAME_STATE_PLAYING;

  // Load font for UI text
  sf::Font font;
  if (!font.openFromFile(FONT_PATH)) {
    std::cerr << "Warning: Could not load font from " << FONT_PATH << std::endl;
  }

  // Create text objects - SFML 3.0 requires font in constructor
  sf::Text gameOverText(font, "", TEXT_SIZE_LARGE);
  gameOverText.setStyle(sf::Text::Bold);

  sf::Text restartText(font, TEXT_RESTART, TEXT_SIZE_SMALL);
  restartText.setFillColor(sf::Color::White);

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();

    // Event handling
    while (auto eventOpt = window.pollEvent()) {
      if (eventOpt->is<sf::Event::Closed>()) {
        window.close();
      }

      // Handle restart on R key press
      if (eventOpt->is<sf::Event::KeyPressed>()) {
        auto keyEvent = eventOpt->getIf<sf::Event::KeyPressed>();
        if (keyEvent->code == sf::Keyboard::Key::R &&
            gameState != GAME_STATE_PLAYING) {
          // Reset game
          player.reset();
          wormhole.reset();
          gameState = GAME_STATE_PLAYING;
          audioManager.resetForRestart();
        }
      }
    }

    if (gameState == GAME_STATE_PLAYING) {
      // Update
      bool is_key_pressed =
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) &&
          player.has_thrust();

      // Audio for thrust
      if (is_key_pressed) {
        audioManager.playThrust();
      } else {
        audioManager.stopThrust();
      }

      player.update(dt, is_key_pressed);
      wormhole.update(dt);

      for (auto &ast : asteroids) {
        ast.update(dt);
        handleCollision(player, ast, audioManager);
      }

      hud.update(player);
      wormhole.checkCollision(player.getPosition(), player.getRadius());

      // Update breathing sound based on oxygen
      audioManager.updateBreathing(player.oxygen);

      // Check win/loss conditions
      if (wormhole.isReached) {
        gameState = GAME_STATE_WON;
        audioManager.stopAll();
        audioManager.playVictory();
      }
      if (player.isDead) {
        gameState = GAME_STATE_LOST;
        audioManager.stopAll();
        audioManager.playDeath();
        audioManager.playGameOver();
      }
    }

    // Draw
    window.clear(BACKGROUND_COLOR);
    window.draw(background);
    wormhole.draw(window);
    for (auto &ast : asteroids)
      ast.draw(window);
    player.draw(window);
    hud.draw(window);

    // Draw game over screen if needed
    if (gameState == GAME_STATE_WON) {
      gameOverText.setString(TEXT_MISSION_COMPLETE);
      gameOverText.setFillColor(sf::Color::Green);

      // Center the text
      sf::FloatRect textBounds = gameOverText.getLocalBounds();
      gameOverText.setOrigin(
          {textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
      gameOverText.setPosition(
          {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 50.0f});

      sf::FloatRect restartBounds = restartText.getLocalBounds();
      restartText.setOrigin(
          {restartBounds.size.x / 2.0f, restartBounds.size.y / 2.0f});
      restartText.setPosition(
          {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 50.0f});

      window.draw(gameOverText);
      window.draw(restartText);
    } else if (gameState == GAME_STATE_LOST) {
      gameOverText.setString(TEXT_OXYGEN_DEPLETED);
      gameOverText.setFillColor(sf::Color::Red);

      // Center the text
      sf::FloatRect textBounds = gameOverText.getLocalBounds();
      gameOverText.setOrigin(
          {textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
      gameOverText.setPosition(
          {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 50.0f});

      sf::FloatRect restartBounds = restartText.getLocalBounds();
      restartText.setOrigin(
          {restartBounds.size.x / 2.0f, restartBounds.size.y / 2.0f});
      restartText.setPosition(
          {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 50.0f});

      window.draw(gameOverText);
      window.draw(restartText);
    }

    window.display();
  }
  return 0;
}