#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>

// Window
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define WINDOW_TITLE "Space Drift"
#define FRAMERATE_LIMIT 60
#define BACKGROUND_COLOR sf::Color(5, 5, 15)

// Astronaut
#define ASTRO_RADIUS 25.0f
#define ASTRO_START_POS_X WINDOW_WIDTH * 0.1f
#define ASTRO_START_POS_Y WINDOW_HEIGHT * 0.1f
#define ASTRO_BODY_COLOR sf::Color::White

#define POINTER_WIDTH 50.0f
#define POINTER_HEIGHT 10.0f
#define POINTER_ORIGIN_X 0.0f
#define POINTER_ORIGIN_Y 2.0f
#define POINTER_COLOR_IDLE sf::Color::Cyan
#define POINTER_COLOR_THRUST sf::Color::Red
#define POINTER_COLOR_DEAD sf::Color::Black
#define POINTER_OUTLINE_COLOR sf::Color::White

#define ROTATION_SPEED 150.0f
#define THRUST_POWER 300.0f
#define OXYGEN_MAX 100.0f
#define OXYGEN_DRAIN_NORMAL 1.0f
#define OXYGEN_DRAIN_COLLISION 0.05f
#define THRUST_CAPACITY_MAX 100.0f
#define THRUST_DRAIN_RATE 10.0f

// HUD
#define HUD_OXYGEN_BAR_POS_X 10.0f
#define HUD_OXYGEN_BAR_POS_Y 10.0f
#define HUD_OXYGEN_BAR_HEIGHT 10.0f
#define HUD_OXYGEN_BAR_WIDTH_SCALE 2.0f
#define HUD_OXYGEN_BAR_COLOR sf::Color::Red

#define HUD_THRUST_BAR_OFFSET_X -25.0f
#define HUD_THRUST_BAR_OFFSET_Y 25.0f
#define HUD_THRUST_BAR_HEIGHT 5.0f
#define HUD_THRUST_BAR_WIDTH_SCALE 0.5f
#define HUD_THRUST_BAR_COLOR sf::Color::Yellow

// Goal
#define GOAL_RADIUS 40.0f
#define GOAL_START_POS_X WINDOW_WIDTH * 0.9f
#define GOAL_START_POS_Y WINDOW_HEIGHT * 0.9f
#define GOAL_COLOR sf::Color::Green
#define GOAL_OUTLINE_THICKNESS 2.0f
#define GOAL_OUTLINE_COLOR sf::Color::White

// Obstacle
#define OBSTACLE_COLOR sf::Color(100, 100, 120) // Stony grey
#define OBSTACLE_OUTLINE_COLOR sf::Color(150, 150, 150)
#define OBSTACLE_OUTLINE_THICKNESS 2.0f
#define NUM_OBSTACLES 10
#define MIN_OBSTACLE_RADIUS 20
#define MAX_OBSTACLE_RADIUS 40

// Physics / Collision constants
#define COLLISION_BOUNCE_FACTOR 0.4f
#define COLLISION_KICK_FACTOR 0.5f

// Audio stream identifiers
#define SOUND_BACKGROUND "assets/sounds/space_background.mp3"
#define SOUND_BACKGROUND_SCARY "assets/sounds/space_scary.mp3"

#define SOUND_THRUST_HISS "assets/sounds/air-hiss.mp3"
#define SOUND_THRUST_GAS "assets/sounds/gas-flow.mp3"

#define SOUND_COLLISION "assets/sounds/crash.mp3"
#define SOUND_IMPACT "assets/sounds/impact.mp3"
#define SOUND_METAL_IMPACT "assets/sounds/metal_impact.mp3"

#define SOUND_BREATHING "assets/sounds/breath_loop.mp3"

#define SOUND_DEATH_SCREAM "assets/sounds/death_scream.mp3"
#define SOUND_GAME_OVER "assets/sounds/game_over.mp3"

#define SOUND_VICTORY "assets/sounds/dramatic_sound.mp3"
#define SOUND_WARP "assets/sounds/warp_hole.mp3"

#define SOUND_SOS "assets/sounds/SOS_morse.mp3"

// Font Paths
#define FONT_PATH "assets/fonts/arial.ttf"

// Game States
#define GAME_STATE_START -1
#define GAME_STATE_PLAYING 0
#define GAME_STATE_WON 1
#define GAME_STATE_LOST 2

// UI Text
#define TEXT_GAME_TITLE "Dying Astronaut"
#define TEXT_GAME_START_INSTRUCTIONS "Press S to Start"
#define TEXT_RULE_1                                                            \
  "1. Ship's direction controller is damaged so it can only rotate"
#define TEXT_RULE_2 "2. Hold SPACE to engage thrusters."
#define TEXT_RULE_3                                                            \
  "3. Avoid asteroids - they will damage your ship and cause oxygen leakage!"
#define TEXT_RULE_4 "4. Reach the wormhole before oxygen runs out."
#define TEXT_MISSION_COMPLETE "Mission Complete!"
#define TEXT_OXYGEN_DEPLETED "Oxygen Depleted"
#define TEXT_RESTART "Press R to Restart"
#define TEXT_SIZE_LARGE 48
#define TEXT_SIZE_SMALL 24

// Audio Thresholds
#define LOW_OXYGEN_THRESHOLD 50.0f
#define CRITICAL_OXYGEN_THRESHOLD 25.0f // SOS morse starts

// Texture identification constants
#define TEX_SHIP_HEALTHY "assets/textures/healthy_ship.png"
#define TEX_SHIP_DAMAGED "assets/textures/slight_damaged_ship.png"
#define TEX_SHIP_BROKEN "assets/textures/broken_ship.png"

#define TEX_THRUSTER_HEALTHY "assets/textures/healthy_thruster.png"
#define TEX_THRUSTER_BROKEN "assets/textures/broken_thruster.png"

#define TEX_ASTEROID_1 "assets/textures/asteroid1.png"
#define TEX_ASTEROID_2 "assets/textures/asteroid2.png"
#define TEX_ASTEROID_3 "assets/textures/asteroid3.png"
#define TEX_ASTEROID_4 "assets/textures/asteroid4.png"

#define TEX_WORMHOLE "assets/textures/wormhole.png"

#define TEX_BACKGROUND "assets/textures/space2.jpg"

// Damage thresholds
#define SHIP_HEALTHY_THRESHOLD 50.0f
#define SHIP_DAMAGED_THRESHOLD 25.0f

#endif
