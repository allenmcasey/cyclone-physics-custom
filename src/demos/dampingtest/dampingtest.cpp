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
class DampingTest : public Application
{

    const static unsigned particleCount = 2;

    /** Holds the particle data. */
    cyclone::Particle particles[particleCount];

public:
    /** Creates a new demo object. */
    DampingTest();

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
DampingTest::DampingTest()
{
    // Reset the position of the boxes
    reset();
}

const char* DampingTest::getTitle()
{
    return "Cyclone > Damping Test";
}


void DampingTest::reset()
{
    unsigned int counter = 0;
    for (cyclone::Particle *particle = particles; particle < particles+particleCount; particle++)
    {        
        particle->setPosition(counter, (counter+1)*10.0f, 0);
        particle->setVelocity(0, 0, 0);
        particle->setAcceleration(0, -10.0f, 0);
        particle->setMass(25.0);
        particle->setDamping(0.99);

        counter++;
        std::cout << particle->getPosition().x << " " << particle->getPosition().y << std::endl;
    }
}


void DampingTest::update()
{
    // Find the duration of the last frame in seconds
    float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (duration <= 0.0f) return;

    cyclone::Particle *particle = particles;

    // Run the physics
    particle->integrate(duration, 0);
    std::cout << particle->getPosition().x << " " << particle->getPosition().y << std::endl;

    particle++;

    // Run the physics
    particle->integrate(duration, 1);
    std::cout << particle->getPosition().x << " " << particle->getPosition().y << std::endl;

    // // Update the physics of each particle in turn
    // for (cyclone::Particle *particle = particles; particle < particles+particleCount; particle++)
    // {
    //     // Run the physics
    //     particle->integrate(duration);
    //     std::cout << particle->getPosition().x << " " << particle->getPosition().y << std::endl;
    // }

    Application::update();
}

void DampingTest::display()
{
    // Clear the viewport and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(-25.0, 8.0, 5.0,  0.0, 5.0, 22.0,  0.0, 1.0, 0.0);

}

void DampingTest::mouse(int button, int state, int x, int y)
{
    
}

void DampingTest::key(unsigned char key)
{

}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new DampingTest();
}