#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include "Constants.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include <optional>

class AudioManager {
private:
  // Music (for long looping tracks) - use optional since they may fail to load
  std::optional<sf::Music> backgroundMusic;
  std::optional<sf::Music> scaryBackgroundMusic;

  // Sound buffers (must persist)
  sf::SoundBuffer thrustBuffer;
  sf::SoundBuffer collisionBuffer;
  sf::SoundBuffer breathingBuffer;
  sf::SoundBuffer deathScreamBuffer;
  sf::SoundBuffer gameOverBuffer;
  sf::SoundBuffer victoryBuffer;
  sf::SoundBuffer warpBuffer;
  sf::SoundBuffer sosBuffer;
  sf::SoundBuffer hullBreachBuffer;
  sf::SoundBuffer impactBuffer;
  sf::SoundBuffer metalImpactBuffer;
  sf::SoundBuffer ouchBuffer;

  // Sound instances - using unique_ptr since sf::Sound needs buffer at
  // construction
  std::unique_ptr<sf::Sound> thrustSound;
  std::unique_ptr<sf::Sound> collisionSound;
  std::unique_ptr<sf::Sound> breathingSound;
  std::unique_ptr<sf::Sound> deathScreamSound;
  std::unique_ptr<sf::Sound> gameOverSound;
  std::unique_ptr<sf::Sound> victorySound;
  std::unique_ptr<sf::Sound> warpSound;
  std::unique_ptr<sf::Sound> sosSound;
  std::unique_ptr<sf::Sound> hullBreachSound;
  std::unique_ptr<sf::Sound> impactSound;
  std::unique_ptr<sf::Sound> metalImpactSound;
  std::unique_ptr<sf::Sound> ouchSound;

  bool isBreathingPlaying = false;
  bool isSosPlaying = false;
  bool hasPlayedDeathScream = false;

public:
  AudioManager() {
    // Load background music
    backgroundMusic.emplace();
    if (!backgroundMusic->openFromFile(SOUND_BACKGROUND)) {
      std::cerr << "Warning: Could not load " << SOUND_BACKGROUND << std::endl;
      backgroundMusic.reset();
    } else {
      backgroundMusic->setLooping(true);
      backgroundMusic->setVolume(30.0f);
    }

    scaryBackgroundMusic.emplace();
    if (!scaryBackgroundMusic->openFromFile(SOUND_BACKGROUND_SCARY)) {
      std::cerr << "Warning: Could not load " << SOUND_BACKGROUND_SCARY
                << std::endl;
      scaryBackgroundMusic.reset();
    } else {
      scaryBackgroundMusic->setLooping(true);
      scaryBackgroundMusic->setVolume(50.0f);
    }

    // Load sound buffers and create sounds
    if (thrustBuffer.loadFromFile(SOUND_THRUST_HISS)) {
      thrustSound = std::make_unique<sf::Sound>(thrustBuffer);
      thrustSound->setLooping(true);
      thrustSound->setVolume(40.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_THRUST_HISS << std::endl;
    }

    if (collisionBuffer.loadFromFile(SOUND_COLLISION)) {
      collisionSound = std::make_unique<sf::Sound>(collisionBuffer);
      collisionSound->setVolume(40.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_COLLISION << std::endl;
    }

    if (breathingBuffer.loadFromFile(SOUND_BREATHING)) {
      breathingSound = std::make_unique<sf::Sound>(breathingBuffer);
      breathingSound->setLooping(true);
      breathingSound->setVolume(70.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_BREATHING << std::endl;
    }

    if (hullBreachBuffer.loadFromFile(SOUND_HULL_BREACH)) {
      hullBreachSound = std::make_unique<sf::Sound>(hullBreachBuffer);
      hullBreachSound->setVolume(40.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_HULL_BREACH << std::endl;
    }

    if (impactBuffer.loadFromFile(SOUND_IMPACT)) {
      impactSound = std::make_unique<sf::Sound>(impactBuffer);
      impactSound->setVolume(40.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_IMPACT << std::endl;
    }

    if (metalImpactBuffer.loadFromFile(SOUND_METAL_IMPACT)) {
      metalImpactSound = std::make_unique<sf::Sound>(metalImpactBuffer);
      metalImpactSound->setVolume(40.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_METAL_IMPACT
                << std::endl;
    }

    if (ouchBuffer.loadFromFile(SOUND_OUCH)) {
      ouchSound = std::make_unique<sf::Sound>(ouchBuffer);
      ouchSound->setVolume(40.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_OUCH << std::endl;
    }

    if (deathScreamBuffer.loadFromFile(SOUND_DEATH_SCREAM)) {
      deathScreamSound = std::make_unique<sf::Sound>(deathScreamBuffer);
      deathScreamSound->setVolume(100.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_DEATH_SCREAM
                << std::endl;
    }

    if (gameOverBuffer.loadFromFile(SOUND_GAME_OVER)) {
      gameOverSound = std::make_unique<sf::Sound>(gameOverBuffer);
      gameOverSound->setVolume(70.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_GAME_OVER << std::endl;
    }

    if (victoryBuffer.loadFromFile(SOUND_VICTORY)) {
      victorySound = std::make_unique<sf::Sound>(victoryBuffer);
      victorySound->setVolume(80.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_VICTORY << std::endl;
    }

    if (warpBuffer.loadFromFile(SOUND_WARP)) {
      warpSound = std::make_unique<sf::Sound>(warpBuffer);
      warpSound->setVolume(70.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_WARP << std::endl;
    }

    if (sosBuffer.loadFromFile(SOUND_SOS)) {
      sosSound = std::make_unique<sf::Sound>(sosBuffer);
      sosSound->setLooping(true);
      sosSound->setVolume(50.0f);
    } else {
      std::cerr << "Warning: Could not load " << SOUND_SOS << std::endl;
    }
  }

  void startBackgroundMusic() {
    if (backgroundMusic &&
        backgroundMusic->getStatus() != sf::SoundSource::Status::Playing) {
      backgroundMusic->play();
    }
  }

  void playThrust() {
    if (thrustSound &&
        thrustSound->getStatus() != sf::SoundSource::Status::Playing) {
      thrustSound->play();
    }
  }

  void stopThrust() {
    if (thrustSound &&
        thrustSound->getStatus() == sf::SoundSource::Status::Playing) {
      thrustSound->stop();
    }
  }

  void playCollision() {
    if (collisionSound) {
      collisionSound->play();
      hullBreachSound->play();
      impactSound->play();
      metalImpactSound->play();
      ouchSound->play();
    }
  }

  void updateBreathing(float oxygen) {
    // Regular breathing when low oxygen
    if (oxygen < LOW_OXYGEN_THRESHOLD && oxygen > CRITICAL_OXYGEN_THRESHOLD) {
      if (!isBreathingPlaying && breathingSound) {
        breathingSound->play();
        isBreathingPlaying = true;
      }
    } else if (oxygen > LOW_OXYGEN_THRESHOLD) {
      if (isBreathingPlaying && breathingSound) {
        breathingSound->stop();
        isBreathingPlaying = false;
      }
    }

    // SOS morse at critical oxygen levels
    if (oxygen < CRITICAL_OXYGEN_THRESHOLD && oxygen > 0) {
      if (!isSosPlaying) {
        if (sosSound)
          sosSound->play();
        isSosPlaying = true;
        // Switch to scary background
        if (backgroundMusic)
          backgroundMusic->stop();
        if (scaryBackgroundMusic && scaryBackgroundMusic->getStatus() !=
                                        sf::SoundSource::Status::Playing) {
          scaryBackgroundMusic->play();
        }
      }
    } else if (oxygen > CRITICAL_OXYGEN_THRESHOLD) {
      if (isSosPlaying && sosSound) {
        sosSound->stop();
        isSosPlaying = false;
      }
    }
  }

  void playDeath() {
    if (!hasPlayedDeathScream && deathScreamSound) {
      deathScreamSound->play();
      hasPlayedDeathScream = true;
    }
  }

  void playGameOver() {
    if (gameOverSound)
      gameOverSound->play();
  }

  void playVictory() {
    if (victorySound)
      victorySound->play();
    if (warpSound)
      warpSound->play();
  }

  void stopAll() {
    if (thrustSound)
      thrustSound->stop();
    if (breathingSound)
      breathingSound->stop();
    if (sosSound)
      sosSound->stop();
    if (backgroundMusic)
      backgroundMusic->stop();
    if (scaryBackgroundMusic)
      scaryBackgroundMusic->stop();
    isBreathingPlaying = false;
    isSosPlaying = false;
  }

  void resetForRestart() {
    stopAll();
    hasPlayedDeathScream = false;
    startBackgroundMusic();
  }
};

#endif
