/*
 * Interface file for the particle link classes.
 *
 * Part of the Cyclone physics system.
 *
 * Copyright (c) Icosagon 2003. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */

#ifndef CYCLONE_PLINKS_H
#define CYCLONE_PLINKS_H

#include "core.h"
#include "pcontacts.h"

namespace cyclone {

    /**
     * Links connect two particles together, generating a contact
     * if they violate the constraints of their link. It is used
     * as a base class for cables and rods, and could be used as a
     * base class for springs with limit to their extension.
     */
    class ParticleLink : public ParticleContactGenerator
    {
    public:

        /**
         * Holds the pair of particles that are connected by this link.
         */
        Particle* particle[2];

    protected:

        /**
         * Returns the current length of the link.
         */
        real currentLength() const;

    public:

        /**
         * Generates the contacts to keep this link from being
         * violated. This class can only ever generate a single
         * contact, so the pointer can be a pointer to a single
         * element, the limit paramter is assumed to be at least
         * 1 (0 isn't valid), and the return value is 0 if the 
         * cable wasn't overextended, or 1 if a contact was needed.
         */
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
    };

    /**
     * Cables link a pair of particles, generating a contact
     * if they stray too far apart.
     */
    class ParticleCable : public ParticleLink
    {
    public:

        /**
         * Holds the maximum length of the cable.
         */
        real maxLength;

        /**
         * Holds the resitution (bounciness) of the cable.
         */
        real restitution;

        /**
         * Fills the given contact structure with the contact needed
         * to keep the cable from overextended.
         */
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
    };

    class ParticleRod : public ParticleLink
    {
    public:

        /**
         * Holds the length of the rod.
         */
        real length;

        /**
         * Fills the given contact structure with the contact needed
         * to keep the rod from extended or compressing.
         */
        virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
    };
}

#endif // CYCLONE_PLINKS_H