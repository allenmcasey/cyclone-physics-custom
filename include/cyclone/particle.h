/*
 * Interface file for the particle class.
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
 * This file contains the definitions for the paticle class, which can
 * be used in place of rigid bodies for simpler simulations or
 * assemblies.
 */
#ifndef CYCLONE_PARTICLE_H
#define CYCLONE_PARTICLE_H

#include "core.h"

namespace cyclone {

    /**
     * A particle is the simplest object that can be simulated in the
     * physics system.
     *
     * It has position data (no orientation data), along with
     * velocity. It can be integrated forward through time, and have
     * linear forces, and impulses applied to it. The particle manages
     * its state and allows access through a set of methods.
     */
    class Particle
    {

    protected:
        
        /**
         * Holds the inverse of the mass of the particle. It
         * is more useful to hold the inverse mass because
         * integration is simpler, and because in real time
         * simulation it is more useful to have objects with
         * infinite mass (immovable) than zero mass
         * (completely unstable in numerical simulation).
         */
        real inverseMass;

        /**
         * Holds the amount of damping applied to linear
         * motion. Damping is required to remove energy added
         * through numerical instability in the integrator.
         */
        real damping;

        /**
         * Holds the linear position of the particle in
         * world space.
         */
        Vector3 position;

        /**
         * Holds the linear velocity of the particle in
         * world space.
         */
        Vector3 velocity;

        /**
         * Holds the acceleration of the particle.  This value
         * can be used to set acceleration due to gravity (its primary
         * use), or any other constant acceleration.
         */
        Vector3 acceleration;

        /**
         * Holds the accumulated force to be applied at the next
         * simulation iteration only. This value is zeroed at each
         * integration step.
         */
        Vector3 forceAccum;

    public:
        

        /**
         * Integrates the particle forward in time by the given amount.
         * This function uses a Newton-Euler integration method, which is a
         * linear approximation to the correct integral. For this reason it
         * may be inaccurate in some cases.
         */
        void integrate(real duration);

        /**
         * Sets the mass of the particle.
         *
         * @param mass The new mass of the body. This may not be zero.
         * Small masses can produce unstable rigid bodies under
         * simulation.
         *
         * @warning This invalidates internal data for the particle.
         * Either an integration function, or the calculateInternals
         * function should be called before trying to get any settings
         * from the particle.
         */
        void setMass(const real mass);

        /**
         * Gets the mass of the particle.
         *
         * @return The current mass of the particle.
         */
        real getMass() const;

        /**
         * Sets the inverse mass of the particle.
         *
         * @param inverseMass The new inverse mass of the body. This
         * may be zero, for a body with infinite mass
         * (i.e. unmovable).
         *
         * @warning This invalidates internal data for the particle.
         * Either an integration function, or the calculateInternals
         * function should be called before trying to get any settings
         * from the particle.
         */
        void setInverseMass(const real inverseMass);

        /**
         * Gets the inverse mass of the particle.
         *
         * @return The current inverse mass of the particle.
         */
        real getInverseMass() const;

        /**
         * Returns true if the mass of the particle is not-infinite.
         */
        bool hasFiniteMass() const;

        /**
         * Sets both the damping of the particle.
         */
        void setDamping(const real damping);

        /**
         * Gets the current damping value.
         */
        real getDamping() const;

        /**
         * Sets the position of the particle.
         *
         * @param position The new position of the particle.
         */
        void setPosition(const Vector3 &position);

        /**
         * Sets the position of the particle by component.
         *
         * @param x The x coordinate of the new position of the rigid
         * body.
         *
         * @param y The y coordinate of the new position of the rigid
         * body.
         *
         * @param z The z coordinate of the new position of the rigid
         * body.
         */
        void setPosition(const real x, const real y, const real z);

        /**
         * Fills the given vector with the position of the particle.
         *
         * @param position A pointer to a vector into which to write
         * the position.
         */
        void getPosition(Vector3 *position) const;

        /**
         * Gets the position of the particle.
         *
         * @return The position of the particle.
         */
        Vector3 getPosition() const;

        /**
         * Sets the velocity of the particle.
         *
         * @param velocity The new velocity of the particle.
         */
        void setVelocity(const Vector3 &velocity);

        /**
         * Sets the velocity of the particle by component.
         *
         * @param x The x coordinate of the new velocity of the rigid
         * body.
         *
         * @param y The y coordinate of the new velocity of the rigid
         * body.
         *
         * @param z The z coordinate of the new velocity of the rigid
         * body.
         */
        void setVelocity(const real x, const real y, const real z);

        /**
         * Fills the given vector with the velocity of the particle.
         *
         * @param velocity A pointer to a vector into which to write
         * the velocity. The velocity is given in world local space.
         */
        void getVelocity(Vector3 *velocity) const;

        /**
         * Gets the velocity of the particle.
         *
         * @return The velocity of the particle. The velocity is
         * given in world local space.
         */
        Vector3 getVelocity() const;

        /**
         * Sets the constant acceleration of the particle.
         *
         * @param acceleration The new acceleration of the particle.
         */
        void setAcceleration(const Vector3 &acceleration);

        /**
         * Sets the constant acceleration of the particle by component.
         *
         * @param x The x coordinate of the new acceleration of the rigid
         * body.
         *
         * @param y The y coordinate of the new acceleration of the rigid
         * body.
         *
         * @param z The z coordinate of the new acceleration of the rigid
         * body.
         */
        void setAcceleration(const real x, const real y, const real z);

        /**
         * Fills the given vector with the acceleration of the particle.
         *
         * @param acceleration A pointer to a vector into which to write
         * the acceleration. The acceleration is given in world local space.
         */
        void getAcceleration(Vector3 *acceleration) const;

        /**
         * Gets the acceleration of the particle.
         *
         * @return The acceleration of the particle. The acceleration is
         * given in world local space.
         */
        Vector3 getAcceleration() const;

        /**
         * Adds the given force to the particle to be applied at the
         * next iteration only.
         */
        void addForce(const Vector3 &force);

        /**
         * Clears the forces applied to the particle. This will be
         * called automatically after each integration step.
         */
        void clearAccumulator();
    };
}

#endif // CYCLONE_BODY_H