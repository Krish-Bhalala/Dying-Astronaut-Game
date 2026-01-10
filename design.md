### 1. Core Physics & Mechanics

| Rule | Logic Detail |
| --- | --- |
| **Inertia** | Objects preserve velocity.  unless a force (thrust or collision) is applied. |
| **Thrust Logic** | Thrust is a vector  applied in the direction of the arrow. Acceleration . |
| **Directional Lock** | The arrow rotates **only** when the thrust key is released. Pressing the key "locks" the current vector. |
| **Thrust Decay** | Each distinct "press" or prolonged hold reduces the maximum thrust magnitude (simulating engine wear). |
| **Oxygen Drain** | Constant linear decay (). Thrusting increases the decay rate significantly. |

---

### 2. Game Entities & Behaviors

#### **A. The Astronaut**

* **State:** `Position`, `Velocity`, `CurrentAngle`, `OxygenLevel`, `ThrustCapacity`.
* **Action:** * **Idle:** Arrow rotates 360° continuously.
* **Thrusting:** Arrow stops rotating; velocity increases in the arrow's direction.


* **Edge Case:** If `ThrustCapacity` reaches 0, the player can no longer change velocity and must rely on existing momentum to reach the goal.

#### **B. Obstacles (Debris & Asteroids)**

* **Behavior:** Move in linear paths with constant velocity.
* **Collision:** Use **Elastic Collision** logic. When the astronaut hits debris, velocities are swapped based on mass ratios, potentially knocking the player off course.

#### **C. The Ships**

* **Exploding Ship (Start):** The origin point.
* **Healthy Ship (Goal):** A static or slow-moving trigger zone. Reaching this resets oxygen or wins the level.

---

### 3. Audio Scenarios (The Soundscape)

| Scenario | Audio Effect |
| --- | --- |
| **Thrusting** | Low-frequency rumble/hiss (loops while key is held). |
| **Direction Rotation** | Subtle mechanical "clicking" or "whirring" as the arrow spins. |
| **Collision** | High-impact metallic "clank" (debris) or muffled thud (asteroid). |
| **Critical Oxygen** | Rhythmic breathing sound that increases in tempo as . |

---

### 4. Minimalist UI Design

To keep the screen clean, use non-intrusive geometric indicators:

* **The Reticle:** A thin circular arc around the astronaut representing the rotating arrow.
* **Oxygen HUD:** A single thin horizontal line at the very top of the screen (White  Red).
* **Thrust HUD:** A small bar underneath the astronaut that shrinks with use.

---

### 5. Code Structure (C++ Classes)

```cpp
// --- Proposed Structural Organization ---

class Entity {
    sf::Vector2f position, velocity;
    virtual void update(float dt) = 0;
};

class Astronaut : public Entity {
    float oxygen, thrustPower, rotationAngle;
    bool isThrusting;
    void applyThrust(float dt); // Updates velocity based on rotationAngle
    void handleInput();         // Logic for locking/unlocking rotation
};

class ObstacleManager {
    std::vector<Debris> activeDebris;
    void spawnDebris();         // Periodically adds moving obstacles
    void resolveCollisions();   // Physics check: Astronaut vs Debris
};

class GameState {
    enum { START, PLAYING, SUCCESS, DEAD } status;
    void checkWinCondition();   // Distance check to Healthy Ship
};

```

### 6. Edge Case Handling

* **Screen Boundary:** If the astronaut flies out of bounds, do you wrap them around (Pac-Man style) or treat it as a "lost in deep space" death? (Recommended: Wrapping keeps the focus on reflexes).
* **Zero Velocity:** If a player perfectly cancels their momentum and stops, the arrow must resume rotation immediately so they aren't stuck.

**Would you like me to provide the specific C++ math for the "Elastic Collision" logic to ensure the physics feel authentic when hitting debris?**