#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>

// Window
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Space Drift"
#define FRAMERATE_LIMIT 60
#define BACKGROUND_COLOR sf::Color(5, 5, 15)

// Astronaut
#define ASTRO_RADIUS 15.0f
#define ASTRO_START_POS_X 400.0f
#define ASTRO_START_POS_Y 300.0f
#define ASTRO_BODY_COLOR sf::Color::White

#define POINTER_WIDTH 40.0f
#define POINTER_HEIGHT 4.0f
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
#define OXYGEN_DRAIN_COLLISION 10.0f
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
#define GOAL_RADIUS 30.0f
#define GOAL_START_POS_X 700.0f
#define GOAL_START_POS_Y 500.0f
#define GOAL_COLOR sf::Color::Green
#define GOAL_OUTLINE_THICKNESS 2.0f
#define GOAL_OUTLINE_COLOR sf::Color::White

// Obstacle
#define OBSTACLE_COLOR sf::Color(100, 100, 120) // Stony grey
#define OBSTACLE_OUTLINE_COLOR sf::Color(150, 150, 150)
#define OBSTACLE_OUTLINE_THICKNESS 2.0f

// Physics / Collision
#define COLLISION_BOUNCE_FACTOR 2.0f
#define COLLISION_KICK_FACTOR 0.5f

#endif
