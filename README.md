# Dying Astronaut

## The Backstory
This game was born from a moment of pure, misguided confidence after watching the movie *Salt*. I saw the high-stakes maneuvering and thought, "If I were an astronaut, I could totally pull that off." So I went through my high school physics notes and a few articles from the internet on inelastic and elastic collisions, I set out to build the ultimate salt simulator.

**The result?** I quickly realized that space doesn't care about "video game skills". Between inelastic collisions, conservation of angular momentum, and the sheer terror of persistent inertia, I found out that I would have died instantly in the void. Turns out, spinning wildly into a giant rock is much harder to recover from than it looks on screen.

## Game Rules
Your ship is badly damaged, and your life support is failing. You have one goal: reach the wormhole before you run out of oxygen.

1. **Automatic Rotation**: Your rotation controls are fried. The ship spins continuously.
2. **Thrust**: Hold **SPACE** to engage the thrusters. The force is applied in whatever direction you’re currently facing.
3. **Asteroids**: Avoid them. Collisions cause oxygen leaks and send you spinning out of control.
4. **Wormhole**: Reach the glowing green portal to escape.
5. **Restart**: Press **R** to try again.

## Technical Deep Dive: The Physics
The core of this game is a custom 2D physics engine built on top of SFML:

- **Inelastic Collisions**: Implemented using linear impulse resolution. When you hit an asteroid, kinetic energy is lost, and momentum is conserved.
- **Angular Momentum Transfer**: Collisions aren't just about bouncing; they transfer torque. Hitting an object tangentially will cause you (and the asteroid) to change spin based on mass and rotational inertia.
- **Persistent Inertia**: There is no "stop" button in space. Your linear and angular velocities persist until acted upon by thrust or another object.
- **Minimum Angular Velocity**: To prevent the player from getting "stalled" (facing one way forever), the ship maintains a guaranteed minimum spin (treat this as thrust from leaking oxygen).

## Libraries & Tools
- **SFML (Simple and Fast Multimedia Library)**: Used for window management, rendering, and audio.
- **C++17**: The core language for performance and modern syntax.
- **Procedural Graphics**: The thruster flames and plasma plumes are rendered dynamically using SFML primitives and harmonic oscillation for that sweet "flicker" effect.

*Created with love, bit of physics, and a healthy fear of the vacuum.*
