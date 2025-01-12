/*
 * Implementation file for the particle contact resolver.
 *
 * Part of the Cyclone physics system.
 */

#include <assert.h>
#include <iostream>
#include <cyclone/pcontacts.h>

using namespace cyclone;


void ParticleContact::resolve(real duration)
{
    resolveVelocity(duration);
    // resolveInterpenetration(duration);
}

real ParticleContact::calculateSeparatingVelocity() const
{
    Vector3 relativeVelocity = particle[0]->getVelocity();
    if (particle[1]) relativeVelocity -= particle[1]->getVelocity();
    return relativeVelocity * contactNormal;
}

void ParticleContact::resolveVelocity(real duration)
{
    // Find velocity in the direction of this contact.
    real separatingVelocity = calculateSeparatingVelocity();

    // Check if it needs to be resolved.
    if (separatingVelocity > 0)
    {
        // The contact is either separting or stationary;
        // no impulse is required.
        return;
    }

    // Calculate the new separating velocity.
    real newSepVelocity = -separatingVelocity * restitution;

    // Calculate the change in velocity post-contact.
    real deltaVelocity = newSepVelocity - separatingVelocity;

    // We apply the change in velocity to each object in 
    // proportion to their inverse mass (i.e. those with
    // lower inverse mass [higher actual mass] experience 
    // less change in velocity).
    real totalInverseMass = particle[0]->getInverseMass();
    if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

    // If all particles have infinite mass, then impulses have no effect.
    if (totalInverseMass <= 0) return;

    // Calculate the impulse to apply.
    real impulse = deltaVelocity / totalInverseMass;

    // Calculate amount of impluse per unit of impulse mass
    Vector3 impulsePerIMass = contactNormal * impulse;

    // Apply impulses: they are applied in the direction of the contact,
    // and are proportional to the inverse mass.
    particle[0]->setVelocity(particle[0]->getVelocity() +
                            impulsePerIMass * particle[0]->getInverseMass());
    if (particle[1])
    {
        // Particle 1 goes in the opposite direction.
        particle[1]->setVelocity(particle[1]->getVelocity() +
                                impulsePerIMass * -particle[0]->getInverseMass());
    }
}