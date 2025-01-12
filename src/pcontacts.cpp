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
    resolveInterpenetration(duration);
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

    // Check the velocity buildup de to acceleration only.
    Vector3 accelCausedVelocity = particle[0]->getAcceleration();
    if (particle[1]) accelCausedVelocity -= particle[1]->getAcceleration();
    real accelCausedSepVelocity = accelCausedVelocity * contactNormal * duration;

    // If we've got a closing velocity due to acceleration buildup,
    // remove it from the new separating velocity.
    if (accelCausedSepVelocity < 0)
    {
        newSepVelocity += restitution * accelCausedSepVelocity;

        // Make sure we haven't removed more than was there
        // to remove.
        if (newSepVelocity < 0) newSepVelocity = 0;
    }

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

void ParticleContact::resolveInterpenetration(real duration)
{
    // If we don't have any penetration, skip this step.
    if (penetration <= 0) return;

    // The movement of each object is based on its inverse mass,
    // so we total that.
    real totalInverseMass = particle[0]->getInverseMass();
    if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

    // If all particles have infinite mass, then we do nothing.
    if (totalInverseMass <= 0) return;

    // Calculate the amount of penetration resolution per unit
    // of inverse mass.
    Vector3 movePerIMass = contactNormal * (penetration / totalInverseMass);

    // Calculate the movement amounts.
    particleMovement[0] = movePerIMass * particle[0]->getInverseMass();
    if (particle[1])
    {
        particleMovement[1] = movePerIMass * particle[1]->getInverseMass();
    }
    else
    {
        particleMovement[1].clear();
    }

    // Apply the penetration resolution.
    particle[0]->setPosition(particle[0]->getPosition() + particleMovement[0]);
    if (particle[1])
    {
        particle[1]->setPosition(particle[1]->getPosition() + particleMovement[1]);
    }
}
