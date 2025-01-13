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

#ifndef CYCLONE_PCONTACTS_H
#define CYCLONE_PCONTACTS_H

#include "core.h"
#include "particle.h"

namespace cyclone {

    /**
     * A contact represents two objects in contact (in
     * this case ParticleContact representing two particles).
     * Resolving a contact removes their interpenetration, and
     * applies sufficient impulse to keep them apart. Colliding
     * bodies may also rebound.
     * 
     * The contact has no callable functions, it just holds the
     * contact details. To resolve a set of contacts, use the 
     * particle contact resolver class.
     */
    class ParticleContact
    {
        /**
         * The contact resolver object needs access into the contacts to
         * set and effect the contact.
         */
        friend class ParticleContactResolver;

    public:

        /**
         * Holds the two particles that are involved in this
         * contact. The second of these two particles can be
         * NULL for contacts with the scenery.
         */
        Particle* particle[2];

        /**
         * Holds the normal restitution coefficient at the contact.
         */
        real restitution;

        /**
         * Holds the direction of the contact in world coordinates.
         */
        Vector3 contactNormal;

        /**
         * Holds the depth of penetration at the contact.
         */
        real penetration;

        /**
         * Holds the amount each particle is moved by during interpenetration
         * resolution.
         */
        Vector3 particleMovement[2];

    protected:

        /**
         * Resolves this contact for both velocity and interpenetration.
         */
        void resolve(real duration);

        /**
         * Calculates the separating velocity at this contact.
         */
        real calculateSeparatingVelocity() const;

    private:

        /**
         * Handles the impulse calculations for this collision.
         */
        void resolveVelocity(real duration);

        /**
         * Handles the interpenetration resolution for this contact.
         */
        void resolveInterpenetration(real duration);
    };

    class ParticleContactResolver
    {
    protected:

        /**
         * Holds the maximum number of iterations allowed for resolution.
         */
        unsigned iterations;

        /**
         * This is a performance tracking value; we keep a record of the 
         * actual number of iterations used to resolve all contacts.
         */
        unsigned iterationsUsed;

    public:

        /**
         * Creates a new contact resolver.
         */
        ParticleContactResolver(unsigned iterations);
        ParticleContactResolver();

        /**
         * Sets the number of iterations that can be used.
         */
        void setIterations(unsigned iterations);

        /**
         * Resolves a set of particle contacts for both penetration
         * and velocity.
         */
        void resolveContacts(ParticleContact* contactArray, unsigned numContacts, real duration);
    };

    /**
     * This is the basic polymorphic interface for contact
     * generators applying to particles.
     */
    class ParticleContactGenerator
    {
    public:

        /**
         * Fills the given contact structure with the generated
         * contact. The contact pointer should point to the first
         * available contact in a contact array, where limit is the
         * maximum number of contacts in the array that can be written
         * to. The method returns the number of contacts that been 
         * written.
         */
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;    
    };
}

#endif // CYCLONE_PCONTACTS_H