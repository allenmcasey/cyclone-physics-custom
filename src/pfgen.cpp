/*
 * Implementation file for the particle force generators.
 *
 * Part of the Cyclone physics system.
 */

#include <assert.h>
#include <iostream>
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
    magnitude = magnitude - restLength;
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
    // Calculate the vector of the spring (anchor -> particle)
    Vector3 force;
    particle->getPosition(&force);
    force -= *anchorPoint;

    // Calculate the magnituge of the spring force.
    // If spring is stretched, 
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

ParticleLighterThanAir::ParticleLighterThanAir(real particleDensity, real particleVolume, real airDensityAtGround, real densityAltitudeSlope, ParticleGravity gravity) : 
    particleDensity(particleDensity), 
    particleVolume(particleVolume),
    airDensityAtGround(airDensityAtGround),
    densityAltitudeSlope(densityAltitudeSlope),
    gravity(gravity)
{
    assert(particleDensity > 0);
    assert(particleVolume > 0);
    assert(airDensityAtGround > 0);
    assert(densityAltitudeSlope < 0);
}

ParticleLighterThanAir::ParticleLighterThanAir(){}

void ParticleLighterThanAir::updateForce(Particle* particle, real duration) 
{
    particle->setVelocity(cyclone::Vector3(0,0,0));
    
    // Base buoyancy force countering gravity
    Vector3 force = gravity.getGravity() * -1.0f * particle->getMass();

    // Calculate air density at the altitude of the particle
    real currentAirDensity = densityAltitudeSlope * particle->getPosition().y + airDensityAtGround;
   
    /**
     * If air is less dense than particle, particle is no
     * longer rising. Set velocity to zero, add a force to
     * counteract gravity (so that particle levitates), and exit.
    */
    if (currentAirDensity <= particleDensity)
    {
        particle->addForce(force);
        return;
    }

    // Calculate y-component of the buoyancy force.
    real buoyancyComponentY = (currentAirDensity - particleDensity) * particleVolume;

    if (buoyancyComponentY > 0.5)
        std::cout << buoyancyComponentY << " " << particle->getVelocity().y << std::endl;

    // Apply counter-gravity plus buoyancy force.
    particle->addForce(force + cyclone::Vector3(0, buoyancyComponentY, 0));
}

ParticleWheelRoller::ParticleWheelRoller(){}

ParticleWheelRoller::ParticleWheelRoller(Particle* wheelCenter, const real& rollForceScalar)
: wheelCenter(wheelCenter), rollForceScalar(rollForceScalar)
{
}

void ParticleWheelRoller::updateForce(Particle* particle, real duration)
{
    // Check that we do not have infinite mass
    if (!particle->hasFiniteMass()) return;

    // Get vector from this particle to wheel center
    Vector3 spokeVector = wheelCenter->getPosition() - particle->getPosition();

    // Get tangent vector along which we'll add force
    Vector3 forceVector = spokeVector.vectorProduct(Vector3(0,0,1));
    forceVector.normalise();

    // Apply the mass-scaled force to the particle
    particle->addForce(forceVector * rollForceScalar * particle->getMass());

    std::cout << forceVector.x << " " << forceVector.y << " " << forceVector.z << std::endl;
}
