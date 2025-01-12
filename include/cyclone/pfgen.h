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
            virtual void updateForce(Particle* particle, real duration) = 0; 
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
            Registry registrations;

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

    /**
     * A force generator used to apply a gravitational force. One
     * instance can be used for multiple particles.
     */
    class ParticleGravity : public ParticleForceGenerator
    {
        /**
         * Holds the acceleration due to gravity.
         */
        Vector3 gravity;

        public:

            /**
             * Creates the generator with the given acceleration.
             */
            ParticleGravity(const Vector3 &gravity);
            ParticleGravity();

            /**
             * Returns this force generator's gravity vector.
             */
            Vector3 getGravity() const;

            /**
             * Applies the gravitational force to the given particle.
             */
            virtual void updateForce(Particle* particle, real duration);
    };

    class ParticlePointGravity : public ParticleForceGenerator
    {
        /**
         * Holds the scalar acceleration due to gravity. This force is scaled
         * based on the inverse square of the distance between the given
         * particle and the gravityPoint.
         */
        real gravityScalar;

        /**
         * Holds the position of the gravitational
         * attraction. All registered particles will be 
         * pulled toward this location.
         */
        Vector3 gravityPoint;

        public:

            /**
             * Creates the generator with the given acceleration and attraction point.
             */
            ParticlePointGravity(const real &gravityScalar, const Vector3 &gravityPoint);
            ParticlePointGravity();

            /**
             * Applies the gravitational force to the given particle.
             */
            virtual void updateForce(Particle* particle, real duration);
    };

    /**
     * A force generator used to apply a gravitational force. One
     * instance can be used for multiple particles.
     */
    class ParticleUplift : public ParticleForceGenerator
    {
        /**
         * Holds the acceleration due to gravity.
         */
        Vector3 upliftForce;

        /**
         * Center point of area affected by uplift force.
         */
        Vector3 upliftPoint;

        /**
         * Radius from uplift point that uplift force
         * has effect.
         */
        real upliftRadius;

        /**
         * Holds the maximum height (y-val) that this
         * force generator can lift a particle.
         */
        real maxUpliftHeight;

        /**
         * Gravity force generator associated with this uplift
         * generator. Once the uplift generator gets the particle
         * to the max uplift height, the force added to this particle
         * is the negative of the gravity force so that the particle 
         * levitates in place.
         */
        ParticleGravity gravity;

        public:

            /**
             * Creates the generator with the given acceleration.
             */
            ParticleUplift(const Vector3 &upliftForce, 
                            const Vector3 &upliftPoint,
                            const real &upliftRadius,
                            const real &maxUpliftHeight,
                            const ParticleGravity &gravity);
            ParticleUplift();

            /**
             * Applies the gravitational force to the given particle.
             */
            virtual void updateForce(Particle* particle, real duration);
    };

    /**
     * A force generator that applies a spring force.
     */
    class ParticleSpring : public ParticleForceGenerator
    {
        /**
         * The particle at the other end of the spring.
         */
        Particle* other;

        /**
         * Holds the spring constant.
         */
        real springConstant;

        /**
         * Holds the resting length of the spring.
         */
        real restLength;

    public:

        /**
         * Creates a new spring with the given parameters.
         */
        ParticleSpring(Particle* other, real &springConstant, real &restLength);
        ParticleSpring();

        /**
         * Applies the spring force to the given particle.
         */
        virtual void updateForce(Particle* particle, real duration);
    };

    class ParticleAnchoredSpring : public ParticleForceGenerator
    {
    protected:

        /**
         * The location of the anchored end of the spring.
         */
        Vector3* anchorPoint;

        /**
         * Holds the spring constant.
         */
        real springConstant;

        /**
         * Holds the resting length of the spring.
         */
        real restLength;

    public:

        /**
         * Creates a new spring with given parameters.
         */
        ParticleAnchoredSpring(Vector3* anchorPoint, real& springConstant, real& restLength);
        ParticleAnchoredSpring();

        /**
         * Applies the spring force to the given particle.
         */
        virtual void updateForce(Particle* particle, real duration);
    };

    class ParticleBungee : public ParticleForceGenerator
    {
        /**
         * The particle at the other end of the spring.
         */
        Particle* other;

        /**
         * Holds the spring constant.
         */
        real springConstant;

        /**
         * Holds the resting length of the spring.
         */
        real restLength;

    public:

        /**
         * Creates a new spring with the given parameters.
         */
        ParticleBungee(Particle* other, real &springConstant, real &restLength);
        ParticleBungee();

        /**
         * Applies the spring force to the given particle.
         */
        virtual void updateForce(Particle* particle, real duration);
    };

    class ParticleBuoyancy : public ParticleForceGenerator
    {
        /**
         * The maximum submersion depth of the object before
         * it generates its maximum buoyancy force (fully submerged).
         */
        real maxDepth;

        /**
         * The volume of the object.
         */
        real volume;

        /**
         * The height of the water plane above y = 0. The plane is 
         * assumed to be parallel to the XZ plane.
         */
        real waterHeight;

        /**
         * The density of the liquid. Pure water has a density of
         * 1000kg per cubic meter.
         */
        real liquidDensity;

    public:

        /**
         * Creates a new buoyancy force with the given parameters.
         */
        ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity = 1000.0f);
        ParticleBuoyancy();

        /**
         * Applies the spring force to the given particle.
         */
        virtual void updateForce(Particle* particle, real duration);
    };
}


#endif // CYCLONE_PFGEN_H