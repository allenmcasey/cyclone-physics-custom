/*
 * Interface file for the force generators.
 *
 * Part of the Cyclone physics system.
 *
 * Copyright (c) Icosagon 2003. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */

/**
 * @file
 *
 * This file contains the interface and sample force generators.
 */
#ifndef CYCLONE_PFGEN_H
#define CYCLONE_PFGEN_H

#include "core.h"
#include "particle.h"
#include <vector>

namespace cyclone {

    /**
     * A force generator can be asked to add a force to one or more
     * particleseach frame while registered.
     */
    class ParticleForceGenerator
    {
        public:

            /**
             * Overload this method in the implementation of the interface
             * to calculate and update the force applied to th given particle.
             */
            virtual void updateForce(Particle* particle, real duration); 
    };

    /**
     * Holds all force generators and the particles that they apply to.
     */
    class ParticleForceRegistry
    {
        protected:

            /**
             * Keeps track of one force generator and the particle it
             * applies to.
             */
            struct ParticleForceRegistration
            {
                Particle* particle;
                ParticleForceGenerator* fg;
            };

            /**
             * Holds the list of registered particle-generator pairs
             */
            typedef std::vector<ParticleForceRegistration> Registry;
            Registry registration;

        public:

            /**
             * Registers the given force generator to apply to the 
             * given particle.
             */
            void add(Particle* particle, ParticleForceGenerator* fg);

            /**
             * Removes the given registered pair from the registry.
             * If the pair is not regiestered, method will have no
             * effect.
             */
            void remove(Particle* particle, ParticleForceGenerator* fg);

            /**
             * Clears registry of all registrations. This will not delete
             * particles or force generators themselves, just the associations
             * between them.
             */
            void clear();

            /**
             * Calls all the force generators to update the forces of
             * their corresponding particles.
             */
            void updateForces(real duration);
    };
   
}

#endif // CYCLONE_PFGEN_H