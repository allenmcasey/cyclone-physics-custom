/*
 * Damping Test.
 *
 * Part of the Cyclone physics system.
 *
 * Copyright (c) Icosagon 2003. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */

#include <cyclone/cyclone.h>
#include "../ogl_headers.h"
#include "../app.h"
#include "../timing.h"

#include <stdio.h>
#include <iostream>

/**
 * The main demo class definition.
 */
class LighterDemo : public Application
{
    const static unsigned particleCount = 16;

    /** Holds the particle data. */
    cyclone::Particle particles[particleCount];

    cyclone::ParticleForceRegistry registry;
    cyclone::ParticleGravity particleGravity;
    cyclone::ParticleLighterThanAir particleLighterThanAir;

public:
    /** Creates a ne
     * w demo object. */
    LighterDemo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Update the particle positions. */
    virtual void update();

    /** Resets the position of all the boxes and primes the explosion. */
    virtual void reset();

    /** Display the particle positions. */
    virtual void display();

    /** Handle a mouse click. */
    virtual void mouse(int button, int state, int x, int y);

    /** Handle a keypress. */
    virtual void key(unsigned char key);
};


// Method definitions
LighterDemo::LighterDemo()
{
    // Reset the position of the boxes
    reset();
}

const char* LighterDemo::getTitle()
{
    return "Cyclone > Lighter Than Air Sim";
}


void LighterDemo::reset()
{
    // Initialize particle force generator registry
    registry = cyclone::ParticleForceRegistry();

    // Init gravity force generator
    particleGravity = cyclone::ParticleGravity(cyclone::Vector3(0, -10.0f, 0));

    // Init particle lighter-than-air force generator
    particleLighterThanAir = cyclone::ParticleLighterThanAir(1.0, 2.5, 10, -1.5, particleGravity);

    // Particle iterator
    cyclone::Particle *particle = particles;

    // Instantiate sphere 1
    particle->setPosition(2.5, 0, 0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(5.0, 0, 0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(7.5, 0, 0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(10.0, 0, 0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(-2.5, 0, 0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(-5.0, 0, 0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(-7.5, 0, 0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(-10.0, 0, 0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(0, 0, 2.5);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(0, 0, 5.0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(0, 0, 7.5);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(0, 0, 10.0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(0, 0, -2.5);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(0, 0, -5.0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(0, 0, -7.5);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;

    // Instantiate sphere 1
    particle->setPosition(0, 0, -10.0);
    particle->setVelocity(0, 0, 0);
    particle->setMass(5.0);
    particle->setDamping(0.999);

    // Register particle with force generator
    registry.add(particle, &particleGravity);
    registry.add(particle, &particleLighterThanAir);
    particle++;
}


void LighterDemo::update()
{
    // Find the duration of the last frame in seconds
    float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (duration <= 0.0f) return;

    // Update all registered force-particle pairs
    registry.updateForces(duration);

    // Update the physics of each particle in turn
    for (cyclone::Particle *particle = particles+1; particle < particles+particleCount; particle++)
    {
        
        // Run the physics
        particle->integrate(duration);
    }

    Application::update();
}

void LighterDemo::display()
{
    // Clear the viewport and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(-12.0, 8.0, -20.0,  0.0, 5.0, 22.0,  0.0, 1.0, 0.0);

    glColor3f(0, 0, 0);

    cyclone::Vector3 position;

    // Particle iterator
    cyclone::Particle *particle = particles;

    // Render all particles
    for (; particle < particles+particleCount; particle++)
    {
        particle->getPosition(&position);
    
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glutSolidSphere(0.3f, 5, 4);
        glPopMatrix();
    }
}

void LighterDemo::mouse(int button, int state, int x, int y)
{
    
}

void LighterDemo::key(unsigned char key)
{

}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new LighterDemo();
}