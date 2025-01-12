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

ParticleSpring::ParticleSpring(Particle* other, real& springConstant, real& restLength) : 
    other(other), 
    springConstant(springConstant),
    restLength(restLength)
{
}

ParticleSpring::ParticleSpring(){}

void ParticleSpring::updateForce(Particle* particle, real duration) 
{
    // Calculate the vector of the spring.
    Vector3 force;
    particle->getPosition(&force);
    force -= other->getPosition();

    // Calculate the magnituge of the spring force.
    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    // Calculate final force and apply it.
    force.normalise();
    force *= -magnitude;
    particle->addForce(force);
}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* anchorPoint, real& springConstant, real& restLength) : 
    anchorPoint(anchorPoint), 
    springConstant(springConstant),
    restLength(restLength)
{
}

ParticleAnchoredSpring::ParticleAnchoredSpring(){}

void ParticleAnchoredSpring::updateForce(Particle* particle, real duration)
{
    // Calculate the vector of the spring.
    Vector3 force;
    particle->getPosition(&force);
    force -= *anchorPoint;

    // Calculate the magnituge of the spring force.
    real magnitude = force.magnitude();
    magnitude = (magnitude - restLength) * springConstant;

    // Calculate final force and apply it.
    force.normalise();
    force *= -magnitude;
    particle->addForce(force);
}


ParticleBungee::ParticleBungee(Particle* other, real& springConstant, real& restLength) : 
    other(other), 
    springConstant(springConstant),
    restLength(restLength)
{
}

ParticleBungee::ParticleBungee(){}

void ParticleBungee::updateForce(Particle* particle, real duration) 
{
    // Calculate the vector of the spring.
    Vector3 force;
    particle->getPosition(&force);
    force -= other->getPosition();

    // Check if bungee is compressed; if so, return.
    real magnitude = force.magnitude();
    if (magnitude <= restLength) return;

    // Calculate the magnitude of the force.
    magnitude = (restLength - magnitude) * springConstant;

    // Calculate final force and apply it.
    force.normalise();
    force *= -magnitude;
    particle->addForce(force);
}

ParticleBuoyancy::ParticleBuoyancy(real maxDepth, real volume, real waterHeight, real liquidDensity) : 
    maxDepth(maxDepth), 
    volume(volume),
    waterHeight(waterHeight),
    liquidDensity(liquidDensity)
{
}

ParticleBuoyancy::ParticleBuoyancy(){}

void ParticleBuoyancy::updateForce(Particle* particle, real duration) 
{
    // Get submersion depth.
    real depth = particle->getPosition().y;

    // Check if particle is out of the water.
    if (depth >= waterHeight + maxDepth) return;
    Vector3 force(0,0,0);

    // Check if at maximum depth (i.e. fully submerged)
    if (depth <= waterHeight - maxDepth)
    {
        force.y = liquidDensity * volume;
        particle->addForce(force);
        return;
    }

    /**
     * Otherwise we're partially submerged.
     * 
     *     pv(y_0 - y_w - s)
     * F = -----------------
     *            2s
     */
    force.y = liquidDensity * volume * (depth - maxDepth - waterHeight) / (2 * maxDepth);
    particle->addForce(force);
}
