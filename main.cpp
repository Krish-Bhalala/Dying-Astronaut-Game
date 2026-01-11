#include "Astronaut.hpp"
#include "AudioManager.hpp"
#include "Constants.h"
#include "Goal.hpp"
#include "HUD.hpp"
#include "Obstacle.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

void handleCollision(Astronaut &a, Obstacle &o, AudioManager &audioMgr) {
  sf::Vector2f diff = a.getPosition() - o.getPosition();
  float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
  float minDistance = a.getRadius() + o.getRadius();

  if (distance < minDistance) {
    // Static resolution: correction for overlap
    sf::Vector2f normal = diff / distance;
    float overlap = minDistance - distance;
    a.body->move(normal * overlap);

    // Dynamic resolution: inelastic impact with rotational transfer
    // Collision arm vectors from centers to impact point
    sf::Vector2f rA = normal * a.getRadius();
    sf::Vector2f rB = -normal * o.radius;

    // Relative velocity at impact point (including angular components)
    // v_rel = (vB + wB x rB) - (vA + wA x rA)
    auto crossZ = [](float w, sf::Vector2f r) {
      return sf::Vector2f(-w * r.y, w * r.x);
    };
    sf::Vector2f vA_total =
        a.velocity + crossZ(a.angularVelocity * (3.14159f / 180.0f), rA);
    sf::Vector2f vB_total =
        o.velocity + crossZ(o.angularVelocity * (3.14159f / 180.0f), rB);
    sf::Vector2f v_rel = vB_total - vA_total;

    float rel_norm = v_rel.x * normal.x + v_rel.y * normal.y;

    // Only resolve if objects are approaching
    if (rel_norm < 0) {
      // Linear impulse magnitude (simplified for circular friction-less feel,
      // but we add tangential later)
      float e = COLLISION_BOUNCE_FACTOR;
      float j = -(1.0f + e) * rel_norm;
      j /= (1.0f / a.mass + 1.0f / o.mass);

      sf::Vector2f impulse = normal * j;

      // Apply linear impulse
      a.velocity -= impulse / a.mass;
      o.velocity += impulse / o.mass;

      // Tangential impulse (Friction/Torque transfer)
      sf::Vector2f tangent{-normal.y, normal.x};
      float rel_tan = v_rel.x * tangent.x + v_rel.y * tangent.y;
      float jt = -rel_tan * COLLISION_FRICTION;
      jt /= (1.0f / a.mass + 1.0f / o.mass);

      sf::Vector2f frictionImpulse = tangent * jt;

      // Apply torque: torque = r x impulse
      auto cross2D = [](sf::Vector2f r, sf::Vector2f f) {
        return r.x * f.y - r.y * f.x;
      };

      float torqueA = cross2D(rA, -frictionImpulse);
      float torqueB = cross2D(rB, frictionImpulse);

      // Convert torque to angular velocity change: dw = torque / inertia
      // Astronaut has explicit inertia, Obstacles have simulated inertia (mr^2)
      a.angularVelocity += (torqueA / a.inertia) * (180.0f / 3.14159f);
      o.angularVelocity +=
          (torqueB / (o.mass * o.radius * o.radius)) * (180.0f / 3.14159f);

      // Momentum transfer from obstacle scale
      a.velocity += o.velocity * COLLISION_KICK_FACTOR;
    }

    float velocityMagnitude =
        std::sqrt(a.velocity.x * a.velocity.x + a.velocity.y * a.velocity.y);
    if (velocityMagnitude > 10) {
      float oxygen_drain = velocityMagnitude * OXYGEN_DRAIN_COLLISION;
      a.deplet_oxygen(oxygen_drain);
    }
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
  // Scale background to viewport dimensions
  sf::Vector2u bgSize = backgroundTexture.getSize();
  background.setScale({static_cast<float>(WINDOW_WIDTH) / bgSize.x,
                       static_cast<float>(WINDOW_HEIGHT) / bgSize.y});

  // Load asteroid textures (4 variants)
  sf::Texture asteroidTex1, asteroidTex2, asteroidTex3, asteroidTex4;
  if (!asteroidTex1.loadFromFile(TEX_ASTEROID_1)) {
    std::cerr << "Failed to load asteroid texture 1\n";
  }
  if (!asteroidTex2.loadFromFile(TEX_ASTEROID_2)) {
    std::cerr << "Failed to load asteroid texture 2\n";
  }
  if (!asteroidTex3.loadFromFile(TEX_ASTEROID_3)) {
    std::cerr << "Failed to load asteroid texture 3\n";
  }
  if (!asteroidTex4.loadFromFile(TEX_ASTEROID_4)) {
    std::cerr << "Failed to load asteroid texture 4\n";
  }

  // Game objects
  Astronaut player;
  HUD hud;
  Goal wormhole;
  AudioManager audioManager;
  sf::Clock clock;

  audioManager.startBackgroundMusic();

  // Populate world with randomized obstacles
  std::vector<sf::Texture> asteroidTextures = {asteroidTex1, asteroidTex2,
                                               asteroidTex3, asteroidTex4};
  std::vector<Obstacle> asteroids;
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    int textureIndex = i % asteroidTextures.size();
    asteroids.push_back(
        Obstacle(asteroidTextures[textureIndex],
                 {static_cast<float>(rand() % WINDOW_WIDTH * 0.8f),
                  static_cast<float>(rand() % WINDOW_HEIGHT * 0.8f)},
                 {static_cast<float>(rand() % 100 - 50),
                  static_cast<float>(rand() % 100 - 50)},
                 static_cast<float>((rand() % MAX_OBSTACLE_RADIUS) +
                                    MIN_OBSTACLE_RADIUS)));
  }

  // Game state
  int gameState = GAME_STATE_START;

  // Load font for UI text
  sf::Font font;
  if (!font.openFromFile(FONT_PATH)) {
    std::cerr << "Warning: Could not load font from " << FONT_PATH << std::endl;
  }

  // Create text objects
  sf::Text gameOverText(font, "", TEXT_SIZE_LARGE);
  gameOverText.setStyle(sf::Text::Bold);

  sf::Text restartText(font, TEXT_RESTART, TEXT_SIZE_SMALL);
  restartText.setFillColor(sf::Color::White);

  sf::Text gameTitle(font, TEXT_GAME_TITLE, TEXT_SIZE_LARGE);
  gameTitle.setFillColor(sf::Color::White);
  gameTitle.setOutlineThickness(6.0f);
  gameTitle.setStyle(sf::Text::Bold);

  sf::Text gameStartInstructions(font, TEXT_GAME_START_INSTRUCTIONS,
                                 TEXT_SIZE_SMALL);
  gameStartInstructions.setFillColor(sf::Color::White);
  gameStartInstructions.setOutlineThickness(2.5f);
  gameStartInstructions.setOutlineColor(sf::Color(0, 0, 0, 200));

  sf::Text rule1(font, TEXT_RULE_1, TEXT_SIZE_SMALL);
  rule1.setFillColor(sf::Color(255, 255, 255));
  rule1.setOutlineThickness(1.5f);
  rule1.setOutlineColor(sf::Color(0, 0, 0, 150));

  sf::Text rule2(font, TEXT_RULE_2, TEXT_SIZE_SMALL);
  rule2.setFillColor(sf::Color(255, 255, 255));
  rule2.setOutlineThickness(1.5f);
  rule2.setOutlineColor(sf::Color(0, 0, 0, 150));

  sf::Text rule3(font, TEXT_RULE_3, TEXT_SIZE_SMALL);
  rule3.setFillColor(sf::Color(255, 255, 255));
  rule3.setOutlineThickness(1.5f);
  rule3.setOutlineColor(sf::Color(0, 0, 0, 150));

  sf::Text rule4(font, TEXT_RULE_4, TEXT_SIZE_SMALL);
  rule4.setFillColor(sf::Color(255, 255, 255));
  rule4.setOutlineThickness(1.5f);
  rule4.setOutlineColor(sf::Color(0, 0, 0, 150));

  // Text alignment helper
  auto centerText = [](sf::Text &text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    text.setPosition({x, y});
  };

  centerText(gameTitle, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.25f);
  centerText(gameStartInstructions, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.45f);
  centerText(rule1, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.60f);
  centerText(rule2, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.65f);
  centerText(rule3, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.70f);
  centerText(rule4, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT * 0.75f);

  // Launch screen state
  while (gameState == GAME_STATE_START) {
    float time = clock.getElapsedTime().asSeconds();

    // Dynamic chromatic oscillation for title
    std::uint8_t r =
        static_cast<std::uint8_t>(127 * (1 + std::sin(time * 2.0f)));
    std::uint8_t g =
        static_cast<std::uint8_t>(127 * (1 + std::sin(time * 2.0f + 2.0f)));
    std::uint8_t b =
        static_cast<std::uint8_t>(127 * (1 + std::sin(time * 2.0f + 4.0f)));
    gameTitle.setFillColor(sf::Color::White);
    gameTitle.setOutlineColor(sf::Color(r, g, b));

    // Temporal alpha modulation for instructions
    std::uint8_t alpha =
        static_cast<std::uint8_t>(155 + 100 * std::sin(time * 4.0f));
    gameStartInstructions.setFillColor(sf::Color(255, 255, 255, alpha));
    gameStartInstructions.setOutlineColor(sf::Color(0, 0, 0, alpha));

    window.clear();
    window.draw(background);
    window.draw(gameTitle);
    window.draw(gameStartInstructions);
    window.draw(rule1);
    window.draw(rule2);
    window.draw(rule3);
    window.draw(rule4);
    window.display();

    // Event handling
    while (auto eventOpt = window.pollEvent()) {
      if (eventOpt->is<sf::Event::Closed>()) {
        window.close();
        gameState = -2; // Exit
      }

      // Transition to gameplay state on 'S' key press
      if (eventOpt->is<sf::Event::KeyPressed>()) {
        auto keyEvent = eventOpt->getIf<sf::Event::KeyPressed>();
        if (keyEvent->code == sf::Keyboard::Key::S) {
          gameState = GAME_STATE_PLAYING;
          player.reset_state();
          clock.restart(); // Synchronize delta-time
        }
      }
    }
  }

  while (window.isOpen() && gameState >= 0) {
    float dt = clock.restart().asSeconds();

    // Event handling
    while (auto eventOpt = window.pollEvent()) {
      if (eventOpt->is<sf::Event::Closed>()) {
        window.close();
      }

      // Transition to reset state on 'R' key press
      if (eventOpt->is<sf::Event::KeyPressed>()) {
        auto keyEvent = eventOpt->getIf<sf::Event::KeyPressed>();
        if (keyEvent->code == sf::Keyboard::Key::R &&
            gameState != GAME_STATE_PLAYING) {
          player.reset(); // Re-initialize system state
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

      // Oxygen-dependent frequency modulation for breathing audio
      audioManager.updateBreathing(player.oxygen);

      // Termination condition evaluation
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

    // Render termination graphics
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