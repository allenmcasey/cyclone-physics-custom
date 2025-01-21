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

#define ROD_COUNT 16
#define PARTICLE_COUNT 9

#define BASE_MASS 1

/**
 * The main demo class definition.
 */
class TrebuchetProject : public MassAggregateApplication
{
    cyclone::ParticleRod *rods;
    cyclone::ParticleCable *cables;

public:
    /** Creates a new demo object. */
    TrebuchetProject();
    virtual ~TrebuchetProject();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particles. */
    virtual void display();

    /** Update the particle positions. */
    virtual void update();

    /** Handle a key press. */
    virtual void key(unsigned char key);
};

// Method definitions
TrebuchetProject::TrebuchetProject()
:
MassAggregateApplication(PARTICLE_COUNT), rods(0)
{
    // Create the structure base.
    particleArray[0].setPosition(0,0,0);
    particleArray[1].setPosition(0,0,-4);
    particleArray[2].setPosition(6,0,-4);
    particleArray[3].setPosition(6,0,0);

    particleArray[4].setPosition(3,0,-8);
    particleArray[5].setPosition(3,0,4);

    particleArray[6].setPosition(3,4,-4);
    particleArray[7].setPosition(3,4,0);
    particleArray[8].setPosition(3,4,-2);


    for (unsigned i = 0; i < PARTICLE_COUNT; i++)
    {
        particleArray[i].setInverseMass(0);
        particleArray[i].setVelocity(0,0,0);
        particleArray[i].setDamping(0.9f);
        particleArray[i].setAcceleration(cyclone::Vector3::GRAVITY);
        particleArray[i].clearAccumulator();
    }

    rods = new cyclone::ParticleRod[ROD_COUNT];

    // Cube bottom face
    rods[0].particle[0] = &particleArray[0];
    rods[0].particle[1] = &particleArray[1];
    rods[0].length = 4;

    rods[1].particle[0] = &particleArray[1];
    rods[1].particle[1] = &particleArray[2];
    rods[1].length = 6;

    rods[2].particle[0] = &particleArray[2];
    rods[2].particle[1] = &particleArray[3];
    rods[2].length = 4;

    rods[3].particle[0] = &particleArray[3];
    rods[3].particle[1] = &particleArray[0];
    rods[3].length = 6;

    rods[4].particle[0] = &particleArray[1];
    rods[4].particle[1] = &particleArray[4];
    rods[4].length = 5;

    rods[5].particle[0] = &particleArray[2];
    rods[5].particle[1] = &particleArray[4];
    rods[5].length = 5;

    rods[6].particle[0] = &particleArray[0];
    rods[6].particle[1] = &particleArray[5];
    rods[6].length = 5;

    rods[7].particle[0] = &particleArray[3];
    rods[7].particle[1] = &particleArray[5];
    rods[7].length = 5;

    // A-beam rods
    rods[8].particle[0] = &particleArray[1];
    rods[8].particle[1] = &particleArray[6];
    rods[8].length = 5;

    rods[9].particle[0] = &particleArray[2];
    rods[9].particle[1] = &particleArray[6];
    rods[9].length = 5;

    rods[10].particle[0] = &particleArray[4];
    rods[10].particle[1] = &particleArray[6];
    rods[10].length = real_sqrt(32);

    rods[11].particle[0] = &particleArray[0];
    rods[11].particle[1] = &particleArray[7];
    rods[11].length = 5;

    rods[12].particle[0] = &particleArray[3];
    rods[12].particle[1] = &particleArray[7];
    rods[12].length = 5;

    rods[13].particle[0] = &particleArray[5];
    rods[13].particle[1] = &particleArray[7];
    rods[13].length = real_sqrt(32);

    rods[14].particle[0] = &particleArray[6];
    rods[14].particle[1] = &particleArray[8];
    rods[14].length = 2;

    rods[15].particle[0] = &particleArray[7];
    rods[15].particle[1] = &particleArray[8];
    rods[15].length = 2;
    
    for (unsigned i = 0; i < ROD_COUNT; i++)
    {
        world.getContactGenerators().push_back(&rods[i]);
    }
}

TrebuchetProject::~TrebuchetProject()
{
    if (rods) delete[] rods;
}

void TrebuchetProject::display()
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

void TrebuchetProject::update()
{
    MassAggregateApplication::update();
}

const char* TrebuchetProject::getTitle()
{
    return "Cyclone > Cube Project";
}

void TrebuchetProject::key(unsigned char key)
{
    switch(key)
    {
    case 'a': case 'A':
        particleArray[4].setVelocity(particleArray[0].getVelocity() + cyclone::Vector3(-0.1,0,0));
        break;
    case 'd': case 'D':
        particleArray[4].setVelocity(particleArray[0].getVelocity() + cyclone::Vector3(0.1,0,0));
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
    return new TrebuchetProject();
}