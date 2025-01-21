/*
 * The cube project.
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
#include <cassert>
#include <iostream>

#define ROD_COUNT 16
#define PARTICLE_COUNT 9

#define SPOKE_LENGTH 5
#define BASE_MASS 1

#define ROLL_FORCE 10

/**
 * The main demo class definition.
 */
class WheelProject : public MassAggregateApplication
{
    cyclone::ParticleWheelRoller wheelRoller;

    cyclone::ParticleRod *rods;
    cyclone::real wheelSurfaceLength;

    cyclone::real getXFromAngle(cyclone::real angleRadians);
    cyclone::real getYFromAngle(cyclone::real angleRadians);

public:
    /** Creates a new demo object. */
    WheelProject();
    virtual ~WheelProject();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particles. */
    virtual void display();

    /** Update the particle positions. */
    virtual void update();

    /** Handle a key press. */
    virtual void key(unsigned char key);
};

cyclone::real WheelProject::getXFromAngle(cyclone::real angleRadians)
{
    return SPOKE_LENGTH * real_cos(angleRadians);
}

cyclone::real WheelProject::getYFromAngle(cyclone::real angleRadians)
{
    return SPOKE_LENGTH * real_sin(angleRadians);
}

// Method definitions
WheelProject::WheelProject()
:
MassAggregateApplication(PARTICLE_COUNT), rods(0)
{
    // Initialize particle force generator registry
    // MassAggregateApplication::registry = cyclone::ParticleForceRegistry();

    // Calculate the length of sides that touch the ground
    // Uses law of cosines
    cyclone::real left = 2 * (SPOKE_LENGTH * SPOKE_LENGTH);
    cyclone::real right = (left * real_cos(R_PI/ 4));
    wheelSurfaceLength = real_sqrt(left - right);

    // Center.
    particleArray[0].setPosition(0,SPOKE_LENGTH,0);
    wheelRoller = cyclone::ParticleWheelRoller(&particleArray[0], ROLL_FORCE);
    
    // primary spoke particles
    particleArray[1].setPosition(0,0,0);
    particleArray[2].setPosition(SPOKE_LENGTH,SPOKE_LENGTH,0);
    particleArray[3].setPosition(0,SPOKE_LENGTH*2,0);
    particleArray[4].setPosition(-SPOKE_LENGTH,SPOKE_LENGTH,0);

    // secondary spoke particles
    cyclone::real rad1 = R_PI / 4;
    cyclone::real rad2 = 3 * (R_PI / 4);
    cyclone::real rad3 = 5 * (R_PI / 4);
    cyclone::real rad4 = 7 * (R_PI / 4);
    particleArray[5].setPosition(getXFromAngle(rad4), SPOKE_LENGTH + getYFromAngle(rad4), 0);
    particleArray[6].setPosition(getXFromAngle(rad1), SPOKE_LENGTH + getYFromAngle(rad1), 0);
    particleArray[7].setPosition(getXFromAngle(rad2), SPOKE_LENGTH + getYFromAngle(rad2), 0);
    particleArray[8].setPosition(getXFromAngle(rad3), SPOKE_LENGTH + getYFromAngle(rad3), 0);

    for (unsigned i = 0; i < PARTICLE_COUNT; i++)
    {
        if (i == 0)
        {
            particleArray[i].setInverseMass(0);
            // particleArray[i].setMass(BASE_MASS * 2);
        }   
        else
        {
            particleArray[i].setMass(BASE_MASS);
        }
        
        particleArray[i].setVelocity(0,0,0);
        particleArray[i].setDamping(0.9f);
        // particleArray[i].setAcceleration(cyclone::Vector3::GRAVITY);
        particleArray[i].setAcceleration(cyclone::Vector3(0,0,0));
        particleArray[i].clearAccumulator();
    }

    world.getForceRegistry().add(&particleArray[1], &wheelRoller);
    world.getForceRegistry().add(&particleArray[2], &wheelRoller);
    world.getForceRegistry().add(&particleArray[3], &wheelRoller);
    world.getForceRegistry().add(&particleArray[4], &wheelRoller);

    rods = new cyclone::ParticleRod[ROD_COUNT];

    // 8 spokes
    rods[0].particle[0] = &particleArray[0];
    rods[0].particle[1] = &particleArray[1];
    rods[0].length = SPOKE_LENGTH;

    rods[1].particle[0] = &particleArray[0];
    rods[1].particle[1] = &particleArray[2];
    rods[1].length = SPOKE_LENGTH;

    rods[2].particle[0] = &particleArray[0];
    rods[2].particle[1] = &particleArray[3];
    rods[2].length = SPOKE_LENGTH;

    rods[3].particle[0] = &particleArray[0];
    rods[3].particle[1] = &particleArray[4];
    rods[3].length = SPOKE_LENGTH;

    rods[4].particle[0] = &particleArray[0];
    rods[4].particle[1] = &particleArray[5];
    rods[4].length = SPOKE_LENGTH;

    rods[5].particle[0] = &particleArray[0];
    rods[5].particle[1] = &particleArray[6];
    rods[5].length = SPOKE_LENGTH;

    rods[6].particle[0] = &particleArray[0];
    rods[6].particle[1] = &particleArray[7];
    rods[6].length = SPOKE_LENGTH;

    rods[7].particle[0] = &particleArray[0];
    rods[7].particle[1] = &particleArray[8];
    rods[7].length = SPOKE_LENGTH;


    // Wheel-to-ground surfaces
    rods[8].particle[0] = &particleArray[2];
    rods[8].particle[1] = &particleArray[6];
    rods[8].length = wheelSurfaceLength;

    rods[9].particle[0] = &particleArray[6];
    rods[9].particle[1] = &particleArray[3];
    rods[9].length = wheelSurfaceLength;

    rods[10].particle[0] = &particleArray[3];
    rods[10].particle[1] = &particleArray[7];
    rods[10].length = wheelSurfaceLength;

    rods[11].particle[0] = &particleArray[7];
    rods[11].particle[1] = &particleArray[4];
    rods[11].length = wheelSurfaceLength;

    rods[12].particle[0] = &particleArray[4];
    rods[12].particle[1] = &particleArray[8];
    rods[12].length = wheelSurfaceLength;

    rods[13].particle[0] = &particleArray[8];
    rods[13].particle[1] = &particleArray[1];
    rods[13].length = wheelSurfaceLength;

    rods[14].particle[0] = &particleArray[1];
    rods[14].particle[1] = &particleArray[5];
    rods[14].length = wheelSurfaceLength;

    rods[15].particle[0] = &particleArray[5];
    rods[15].particle[1] = &particleArray[2];
    rods[15].length = wheelSurfaceLength;

    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        world.getContactGenerators().push_back(&rods[i]);
    }
}

WheelProject::~WheelProject()
{
    if (rods) delete[] rods;
}

void WheelProject::display()
{
    MassAggregateApplication::display();

    glBegin(GL_LINES);
    glColor3f(0,0,1);
    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        cyclone::Particle **particles = rods[i].particle;
        const cyclone::Vector3 &p0 = particles[0]->getPosition();
        const cyclone::Vector3 &p1 = particles[1]->getPosition();
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
    }
    glEnd();
}

void WheelProject::update()
{
    MassAggregateApplication::update();
}

const char* WheelProject::getTitle()
{
    return "Cyclone > Cube Project";
}

void WheelProject::key(unsigned char key)
{
    switch(key)
    {
    case 'a': case 'A':
        particleArray[3].setVelocity(particleArray[3].getVelocity() + cyclone::Vector3(-0.1,0,0));
        break;
    case 'd': case 'D':
        particleArray[3].setVelocity(particleArray[3].getVelocity() + cyclone::Vector3(0.1,0,0));
        break;

    default:
        MassAggregateApplication::key(key);
    }
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new WheelProject();
}