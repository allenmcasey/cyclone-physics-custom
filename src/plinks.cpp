/*
 * Implementation file for the particle contact resolver.
 *
 * Part of the Cyclone physics system.
 */

#include <assert.h>
#include <iostream>
#include <cyclone/plinks.h>

using namespace cyclone;


real ParticleLink::currentLength() const
{
    Vector3 relativePos = particle[0]->getPosition() - particle[1]->getPosition();
    return relativePos.magnitude();
}

unsigned ParticleCable::addContact(ParticleContact* contact, unsigned limit) const
{
    // Find the length of the cable.
    real length = currentLength();

    //  Check if cable is overextended.
    if (length < maxLength) return 0;

    // Otherwise, return the contact.
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    // Calculate the normal.
    Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
    normal.normalise();
    contact->contactNormal = normal;

    contact->penetration = length - maxLength;
    contact->restitution = restitution;

    return 1;
}

unsigned ParticleRod::addContact(ParticleContact* contact, unsigned limit) const
{
    // Find the length of the rod.
    real currentLen = currentLength();

    // If rod is nominal length, return.
    if (currentLen = length) return 0;

    // Otherwise, return the contact.
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    // Calculate the normal.
    Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
    normal.normalise();
    
    // The contact normal depends on whether we're extending
    // or compressing.
    if (currentLen > length)
    {
        contact->contactNormal = normal;
        contact->penetration = currentLen - length;
    }
    else
    {
        contact->contactNormal = normal * -1.0;
        contact->penetration = length - currentLen;
    }

    // Always use a contact restitution of 0.
    // (We don't want any bounciness).
    contact->restitution = 0;
    
    return 1;
}
