/*
 * This software is provided under the terms of the GNU General
 * Public License as published by the Free Software Foundation.
 *
 * Copyright (c) 2003 Tom Portegys, All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for NON-COMMERCIAL purposes and without
 * fee is hereby granted provided that this copyright notice
 * appears in all copies.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.
 */

/*
 * Mechanics for colliding bodies connected by bonds and charge forces.
 * Source: "Physics for Game Developers", Copyright 2000-2001 by David Bourg.
 */

#include "Mechanics.hpp"

// Constructor.
Mechanics::Mechanics()
{
   bodies       = NULL;
   numParticles = 0;
   collisions   = NULL;
}


// Destructor.
Mechanics::~Mechanics()
{
   Body *body;

   while (bodies != NULL)
   {
      body = bodies->next;
      delete bodies;
      bodies = body;
   }
}


// Body factory: create body with single particle.
Body *Mechanics::createBody(int type, double radius, double mass, double charge)
{
   if (numParticles >= MAX_PARTICLES)
   {
      return(NULL);
   }
   Body     *body     = createBody();
   Particle *particle = new Particle(type, radius, mass, charge);
   addParticle(body, particle);
   return(body);
}


Body *Mechanics::createBody()
{
   if (numParticles >= MAX_PARTICLES)
   {
      return(NULL);
   }
   Body *body = new Body();
   assert(body != NULL);
   addBody(body);
   return(body);
}


// Add body.
void Mechanics::addBody(Body *body)
{
   body->next = bodies;
   bodies     = body;
}


// Remove body.
void Mechanics::removeBody(Body *body)
{
   Body     *body2, *body3;
   Particle *particle;

   for (body2 = bodies, body3 = NULL; body2 != NULL && body2 != body;
        body3 = body2, body2 = body2->next)
   {
   }
   if (body2 == NULL)
   {
      return;
   }
   if (body3 == NULL)
   {
      bodies = body2->next;
   }
   else
   {
      body3->next = body2->next;
   }
   for (particle = body->particles; particle != NULL;
        particle = particle->next)
   {
      numParticles--;
   }
   delete body;
}


// Add particle to body.
void Mechanics::addParticle(Body *body, Particle *particle)
{
   Vector3D velocity;

   addParticle(body, particle, velocity);
}


void Mechanics::addParticle(Body *body, Particle *particle, Vector3D& velocity)
{
   particle->body   = body;
   particle->next   = body->particles;
   body->particles  = particle;
   body->vVelocity *= body->fMass;
   velocity        *= particle->fMass;
   body->vVelocity += velocity;
   body->fMass     += particle->fMass;
   body->vVelocity /= body->fMass;
   body->calcInertia();
   if (particle->fixed)
   {
      body->vVelocity.Zero();
      body->fixedCount++;
   }
   numParticles++;
}


// Remove particle from body.
// If remaining particles disconnected, create
// new body containing partitioned set of particles.
void Mechanics::removeParticle(Body *body, Particle *particle)
{
   Particle *particle2, *particle3;

   for (particle2 = body->particles, particle3 = NULL;
        particle2 != NULL && particle2 != particle;
        particle3 = particle2, particle2 = particle2->next)
   {
   }
   if (particle2 == NULL)
   {
      return;
   }
   if (particle3 == NULL)
   {
      body->particles = particle2->next;
   }
   else
   {
      particle3->next = particle2->next;
   }
   body->fMass -= particle->fMass;
   body->calcInertia();
   if (particle->fixed)
   {
      body->fixedCount--;
   }
   delete particle;
   numParticles--;
   if (body->particles == NULL)
   {
      removeBody(body);
      return;
   }

   // Create new body and partition particles.
   if (markParticlePartitions(body) > 1)
   {
      partitionParticles(body);
   }
}


// Is particle valid?
bool Mechanics::isValidParticle(Particle *particle)
{
   Body     *body;
   Particle *particle2;

   for (body = bodies; body != NULL; body = body->next)
   {
      for (particle2 = body->particles; particle2 != NULL;
           particle2 = particle2->next)
      {
         if (particle2 == particle)
         {
            return(true);
         }
      }
   }
   return(false);
}


// Bond particles.
// If particles in different bodies, combine bodies.
Bond *Mechanics::createBond(Particle *particle1, Particle *particle2)
{
   Bond *bond = particle1->getBond(particle2);

   if (bond != NULL)
   {
      return(bond);
   }
   bond = new Bond(particle1, particle2);
   assert(bond != NULL);
   bond->next1      = particle1->bonds;
   particle1->bonds = bond;
   bond->next2      = particle2->bonds;
   particle2->bonds = bond;
   Body *body1 = particle1->body;
   Body *body2 = particle2->body;
   if (body1 == body2)
   {
      return(bond);
   }
   while (body2->particles != NULL)
   {
      particle1        = body2->particles;
      body2->particles = particle1->next;
      addParticle(body1, particle1, body2->vVelocity);
   }
   removeBody(body2);
   return(bond);
}


// Remove bond.
// If remaining particles disconnected, create
// new body containing partitioned set of particles.
void Mechanics::removeBond(Bond *bond)
{
   Body *body = bond->particle1->body;

   delete bond;
   if (markParticlePartitions(body) > 1)
   {
      partitionParticles(body);
   }
}


// Mark connected sets of particles.
// Return number of partitions.
int Mechanics::markParticlePartitions(Body *body)
{
   int      mark;
   Particle *particle;

   // Clear marks.
   for (particle = body->particles; particle != NULL;
        particle = particle->next)
   {
      particle->mark = -1;
   }

   // Mark partitions.
   mark = 0;
   for (particle = body->particles; particle != NULL;
        particle = particle->next)
   {
      if (particle->mark == -1)
      {
         particle->mark = mark;
         mark++;
         markParticles(particle);
      }
   }
   return(mark);
}


// Mark connected particles.
void Mechanics::markParticles(Particle *particle1)
{
   Particle *particle2;
   Bond     *bond;

   for (bond = particle1->bonds; bond != NULL; )
   {
      if (particle1 == bond->particle1)
      {
         particle2 = bond->particle2;
         bond      = bond->next1;
      }
      else
      {
         particle2 = bond->particle1;
         bond      = bond->next2;
      }
      if (particle2->mark == particle1->mark)
      {
         continue;
      }
      particle2->mark = particle1->mark;
      markParticles(particle2);
   }
}


// Partition marked particles into separate bodies.
void Mechanics::partitionParticles(Body *body)
{
   Body     *body2;
   Particle *particle2, *particle3;

   body2            = new Body();
   body2->vVelocity = body->vVelocity;
   while (true)
   {
      for (particle2 = body->particles, particle3 = NULL;
           particle2 != NULL && particle2->mark != 1;
           particle3 = particle2, particle2 = particle2->next)
      {
      }
      if (particle2 == NULL)
      {
         break;
      }
      if (particle3 == NULL)
      {
         body->particles = particle2->next;
      }
      else
      {
         particle3->next = particle2->next;
      }
      body->fMass -= particle2->fMass;
      if (particle2->fixed)
      {
         body->fixedCount--;
      }
      particle2->body  = body2;
      particle2->next  = body2->particles;
      body2->particles = particle2;
      body2->fMass    += particle2->fMass;
      if (particle2->fixed)
      {
         body2->vVelocity.Zero();
         body2->fixedCount++;
      }
   }
   body2->calcInertia();
   addBody(body2);
   body->calcInertia();
}


// Step system by given time increment.
void Mechanics::step(double dtime)
{
   Body      *body;
   Particle  *particle;
   Collision *collision;
   Bond      *bond;
   bool      done;
   float     dist;

   // Integrate.
   for (body = bodies; body != NULL; body = body->next)
   {
      // Body has fixed (immobile) particles?
      if (body->fixedCount > 0)
      {
         body->vVelocity.Zero();
         body->vForces.Zero();
         continue;
      }

      // Update the velocity of the object due to forces.
      body->vVelocity += (body->vForces / body->fMass) * dtime;
      if (body->vVelocity.Magnitude() > MAX_VELOCITY)
      {
         body->vVelocity.Normalize(MAX_VELOCITY);
      }

      // Apply viscosity friction.
      body->vVelocity *= (1.0f - VISCOSITY_FRICTION);

      // Update the positions of the body's particles.
      for (particle = body->particles; particle != NULL;
           particle = particle->next)
      {
         particle->vPosition += body->vVelocity * dtime;
      }

      // Reset forces.
      body->vForces.Zero();
   }

   // Break overstretched bonds.
   done = false;
   while (!done)
   {
      done = true;
      for (body = bodies; body != NULL; body = body->next)
      {
         for (particle = body->particles; particle != NULL;
              particle = particle->next)
         {
            for (bond = particle->bonds; bond != NULL; )
            {
               dist = (bond->particle1->vPosition - bond->particle2->vPosition).Magnitude();
               if ((double)dist > Bond::MAX_BOND_LENGTH)
               {
                  bond->disconnect(particle);
                  done = false;
                  break;
               }
               if (bond->particle1 == particle)
               {
                  bond = bond->next1;
               }
               else
               {
                  bond = bond->next2;
               }
            }
            if (!done)
            {
               break;
            }
         }
         if (!done)
         {
            break;
         }
      }
   }

   // Process collisions and charge forces.
   for (body = bodies; body != NULL; body = body->next)
   {
      body->collide = false;
      for (particle = body->particles; particle != NULL;
           particle = particle->next)
      {
         particle->collide = NULL;
      }
   }
   for (body = bodies; body != NULL; body = body->next)
   {
      // Check for collisions.
      checkCollisions(body);

      // Update charge forces.
      updateChargeForces(body);
   }

   // Resolve collisions.
   resolveCollisions();

   // Release collisions and pseudo-particles.
   while (collisions != NULL)
   {
      collision  = collisions;
      collisions = collision->next;
      particle   = collision->particle2;
      if (particle->type == WALL_TYPE)
      {
         delete particle->body;
      }
      delete collision;
   }
}


// Check for collisions with body's particles.
void Mechanics::checkCollisions(Body *body1)
{
   float     r = (3.0f * FIXED_RADIUS) / 4.0f;
   Body      *body2;
   Particle  *particle1, *particle2;
   Vector3D  vnormal, vrelative, vpoint, vposition;
   Collision *collision;

   if (body1->collide)
   {
      return;
   }
   for (particle1 = body1->particles;
        particle1 != NULL; particle1 = particle1->next)
   {
      // Check for wall collisions.
      vpoint    = particle1->vPosition;
      vposition = particle1->vPosition;
      if ((particle1->vPosition.y - particle1->fRadius) <= 0.0f)
      {
         vpoint.y   -= particle1->fRadius;
         vposition.y = -r;
      }
      else if ((particle1->vPosition.y + particle1->fRadius) >= HEIGHT)
      {
         vpoint.y   += particle1->fRadius;
         vposition.y = HEIGHT + r;
      }
      else if ((particle1->vPosition.x - particle1->fRadius) <= 0.0f)
      {
         vpoint.x   -= particle1->fRadius;
         vposition.x = -r;
      }
      else if ((particle1->vPosition.x + particle1->fRadius) >= WIDTH)
      {
         vpoint.x   += particle1->fRadius;
         vposition.x = WIDTH + r;
      }
      else
      {
         continue;
      }
      particle2 = new Particle(WALL_TYPE);
      assert(particle2 != NULL);
      particle2->fMass   = FIXED_MASS;
      particle2->fRadius = FIXED_RADIUS;
      particle2->setFixed(true);
      particle2->vPosition = vposition;
      body2 = new Body();  // fake body for collision only
      assert(body2 != NULL);
      addParticle(body2, particle2);
      vnormal = particle1->vPosition - particle2->vPosition;
      vnormal.Normalize();
      vrelative = body1->vVelocity;
      if ((vrelative * vnormal) < 0.0f)
      {
         collision = new Collision();
         assert(collision != NULL);
         collision->particle1         = particle1;
         collision->particle2         = particle2;
         collision->vCollisionNormal  = vnormal;
         collision->vCollisionPoint   = vpoint;
         collision->vRelativeVelocity = vrelative;
         collision->next = collisions;
         collisions      = collision;
         body1->collide  = true;
         return;
      }
      else
      {
         removeParticle(body2, particle2);
         delete body2;
      }
   }

   for (particle1 = body1->particles;
        particle1 != NULL; particle1 = particle1->next)
   {
      // Check for particle-particle collisions.
      for (body2 = body1->next; body2 != NULL; body2 = body2->next)
      {
         if (body2->collide)
         {
            continue;
         }
         if ((body1->fixedCount > 0) && (body2->fixedCount > 0))
         {
            continue;
         }
         for (particle2 = body2->particles;
              particle2 != NULL; particle2 = particle2->next)
         {
            // Particles intersect?
            vnormal = particle1->vPosition - particle2->vPosition;
            if (vnormal.Magnitude() < (particle1->fRadius + particle2->fRadius))
            {
               // Particles moving toward each other?
               vnormal.Normalize();
               vrelative = body1->vVelocity - body2->vVelocity;
               if ((vrelative * vnormal) < 0.0)
               {
                  collision = new Collision();
                  assert(collision != NULL);
                  collision->particle1        = particle1;
                  collision->particle2        = particle2;
                  particle1->collide          = particle2;
                  particle2->collide          = particle1;
                  collision->vCollisionNormal = vnormal;
                  collision->vCollisionPoint  = (vnormal * particle1->fRadius) +
                                                particle1->vPosition;
                  collision->vRelativeVelocity = vrelative;
                  collision->next = collisions;
                  collisions      = collision;
                  body1->collide  = body2->collide = true;
                  return;
               }
            }
         }
      }
   }
}


// Resolve collisions.
void Mechanics::resolveCollisions()
{
   Collision *collision;
   Body      *body1, *body2;
   Particle  *particle1, *particle2;
   Vector3D  pt1, pt2;
   double    mass1, mass2, impulse;
   float     coefficientOfRestitution;

   for (collision = collisions; collision != NULL; collision = collision->next)
   {
      particle1 = collision->particle1;
      body1     = particle1->body;
      particle2 = collision->particle2;
      body2     = particle2->body;

      // For fixed bodies, use huge mass.
      if (body1->fixedCount == 0)
      {
         mass1 = body1->fMass;
      }
      else
      {
         mass1 = FIXED_MASS;
      }
      if (body2->fixedCount == 0)
      {
         mass2 = body2->fMass;
      }
      else
      {
         mass2 = FIXED_MASS;
      }

      // Calculate impulse force.
      pt1 = collision->vCollisionPoint - particle1->vPosition;
      pt2 = collision->vCollisionPoint - particle2->vPosition;
      coefficientOfRestitution = (particle1->coefficientOfRestitution +
                                  particle2->coefficientOfRestitution) / 2.0f;
      impulse =
         (-(1.0 + coefficientOfRestitution) *
          (collision->vRelativeVelocity * collision->vCollisionNormal)) /
         ((1.0 / mass1 + 1.0 / mass2) +
          (collision->vCollisionNormal *
                    (((pt1 ^ collision->vCollisionNormal) * body1->mInertiaInverse) ^ pt1)) +
          (collision->vCollisionNormal *
                    (((pt2 ^ collision->vCollisionNormal) * body2->mInertiaInverse) ^ pt2))
         );

      // Accumulate forces.
      body1->vForces += impulse * collision->vCollisionNormal;
      body2->vForces -= impulse * collision->vCollisionNormal;
   }
}


// Update charge forces.
void Mechanics::updateChargeForces(Body *body1)
{
   Body     *body2;
   Particle *particle1, *particle2;
   Vector3D vForce;
   double   dist;
   double   s;

   for (particle1 = body1->particles; particle1 != NULL;
        particle1 = particle1->next)
   {
      for (body2 = bodies; body2 != NULL; body2 = body2->next)
      {
         if (body1 == body2)
         {
            continue;
         }
         for (particle2 = body2->particles; particle2 != NULL;
              particle2 = particle2->next)
         {
            vForce = particle1->vPosition - particle2->vPosition;
            dist   = vForce.Magnitude();
            if (dist > 0.0)
            {
               // Force is proportional to inverse square of distance.
               vForce.Normalize();
               s = (CHARGECONSTANT * particle1->fCharge * particle2->fCharge) /
                   (dist * dist);
               vForce         *= s;
               body1->vForces += vForce;
            }
         }
      }
   }
}
