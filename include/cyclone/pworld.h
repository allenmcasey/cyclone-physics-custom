/*
 * Interface file for the particle contact class.
 *
 * Part of the Cyclone physics system.
 *
 * Copyright (c) Icosagon 2003. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */

#ifndef CYCLONE_PWORLD_H
#define CYCLONE_PWORLD_H

#include <vector>
#include "core.h"
#include "pfgen.h"
#include "plinks.h"

namespace cyclone {

    /**
     * Keeps track of a set of particles, and provides the means
     * to update the physics of them all.
     */
    class ParticleWorld
    {
    public:

        typedef std::vector<Particle*> Particles;

    protected:

        /**
         * Holds the particles.
         */
        Particles particles;

    public:

        /**
         * Creates a new particle simulator that can handle up to the given
         * number of contacts per frame. You can also optoinally give a number
         * of contact-resolution iterations to use. If you don't give a number
         * of iterations, then twice the number of contacts will be used.
         */
        ParticleWorld(unsigned maxContacts = 100, unsigned iterations = 0);

        /**
         * Initializes the world for a simulation frame. This clears
         * force accumulators for particles in the world. After calling
         * this, the particles can have their forces for this frame added.
         */
        void startFrame();

        typedef std::vector<ParticleContactGenerator*> ContactGenerators;
        
        /**
         * Holds the force generators for the particles in this world.
         */
        ParticleForceRegistry registry;

        /**
         * Holds the resolver for contacts.
         */
        ParticleContactResolver resolver;

        /**
         * Contact generators.
         */
        ContactGenerators contactGenerators;

        /**
         * Holds the list of contacts.
         */
        ParticleContact* contacts;

        /**
         * Holds the maximum number of contacts allowed (i.e. the
         * size of the contacts list).
         */
        unsigned maxContacts;

        /**
         * Calls each of the registered contact generators to
         * report their contacts. Returns the number of generated
         * contacts.
         */
        unsigned generateContacts();

        /**
         * Integrates all the particles in this world forward in
         * time by the given duration.
         */
        void integrate(real duration);

        /**
         * Processes all the physics for the particle world.
         */
        void runPhysics(real duration);
    };

}

#endif // CYCLONE_PWORLD_H