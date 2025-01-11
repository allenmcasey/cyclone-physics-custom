/*
 * Implementation file for the particle force generators.
 *
 * Part of the Cyclone physics system.
 */


#include <cyclone/pfgen.h>

using namespace cyclone;

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
    ParticleForceRegistry::ParticleForceRegistration newRegistration;
    newRegistration.particle = particle;
    newRegistration.fg = fg;

    registrations.push_back(newRegistration);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
{
    Registry::iterator i = registrations.begin();
    while (i != registrations.end())
    {
        if ((i->particle == particle) && (i->fg == fg))
        {
            registrations.erase(i);
            break;
        }

        i++;
    }
}

void ParticleForceRegistry::clear()
{
    registrations.clear();
}

void ParticleForceRegistry::updateForces(real duration)
{
    Registry::iterator i = registrations.begin();
    for (; i != registrations.end(); i++)
    {
        i->fg->updateForce(i->particle, duration);
    }
}

ParticleGravity::ParticleGravity(const Vector3& gravity) : gravity(gravity)
{
}

ParticleGravity::ParticleGravity(){}

void ParticleGravity::updateForce(Particle* particle, real duration)
{
    // Ensure particle does not have infiinite mass.
    if (!particle->hasFiniteMass()) return;

    // Apply mass-scaled gravitational force to given particle.
    particle->addForce(gravity * particle->getMass());
}

ParticlePointGravity::ParticlePointGravity(){}

ParticlePointGravity::ParticlePointGravity(const real& gravityScalar, const Vector3& gravityPoint)
{
    ParticlePointGravity::gravityScalar = gravityScalar;
    ParticlePointGravity::gravityPoint = gravityPoint;
}

void ParticlePointGravity::updateForce(Particle* particle, real duration)
{
    // Ensure particle does not have infiinite mass.
    if (!particle->hasFiniteMass()) return;

    // Get position vector from particle to gravity point
    Vector3 particleToPoint = gravityPoint - particle->getPosition();

    // Get distance from particle to grav point
    real particleToPointDist = particleToPoint.magnitude();

    if (particleToPointDist < 0.5)
    {
        particle->setVelocity(cyclone::Vector3(0,0,0));
        return;
    }

    // Get unit vector from particle to point
    particleToPoint.normalise();

    // Get force vector of gravity on particle, scaled by particle's distance from gravity point
    Vector3 scaledPointGravity = (particleToPoint * (gravityScalar * particle->getMass())) * ((real)1.0 / real_pow(particleToPointDist, 1.5));
    // Vector3 scaledPointGravity = (particleToPoint * (gravityScalar * particle->getMass())) * ((real)1.0 / particleToPointDist);

    // Apply distance- and mass-scaled gravity to particle toward gravity point
    particle->addForce(scaledPointGravity);
}

ParticleUplift::ParticleUplift(const Vector3& upliftForce, const Vector3& upliftPoint, const real& upliftRadius, const real& maxUpliftHeight, const ParticleGravity& gravity) : 
    upliftForce(upliftForce), 
    upliftPoint(upliftPoint),
    upliftRadius(upliftRadius),
    maxUpliftHeight(maxUpliftHeight),
    gravity(gravity)
{
}

ParticleUplift::ParticleUplift(){}

Vector3 ParticleGravity::getGravity() const
{
    return gravity;
}

void ParticleUplift::updateForce(Particle* particle, real duration)
{
    // Ensure particle does not have infiinite mass.
    if (!particle->hasFiniteMass()) return;

    Vector3 particlePosition = particle->getPosition();

    // Ensure particle is in uplift radius of effect
    Vector3 particleToPoint = upliftPoint - particlePosition;
    if (particleToPoint.magnitude() > upliftRadius) return; 

    if (particlePosition.y >= maxUpliftHeight)
    {
        // If particle is at max height, stop its motion
        particle->setVelocity(cyclone::Vector3(0,0,0));

        // Apply negative of gravitational force to given particle.
        particle->addForce(gravity.getGravity() * (-1.0 *particle->getMass()));
    }
    else
    {
        // Apply mass-scaled gravitational force to given particle.
        particle->addForce(upliftForce * particle->getMass());
    }
}
