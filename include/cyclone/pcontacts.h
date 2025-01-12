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
    };

}

#endif // CYCLONE_PCONTACTS_H