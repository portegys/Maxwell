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

#ifndef __MECHANICS__
#define __MECHANICS__

#include <stdio.h>
#include <assert.h>
#include "Parameters.h"
#include "Physics.h"
#include "Body.hpp"
#include "Particle.hpp"
#include "Bond.hpp"
#ifdef UNIX
#include MORPHOGEN_INCLUDE
#endif
#ifdef WIN32
#include "../morphogens/MORPHOGEN.hpp"
#endif

class Mechanics
{
public:

   // Bodies.
   Body *bodies;
   int  numParticles;

   // Constructor.
   Mechanics();

   // Destructor.
   ~Mechanics();

   // Body factory: create body with single particle.
   Body *createBody(int type, double radius, double mass, double charge);

   Body *createBody();

   // Add body.
   void addBody(Body *body);

   // Remove body.
   void removeBody(Body *body);

   // Add particle to body.
   void addParticle(Body *body, Particle *particle);

   void addParticle(Body *body, Particle *particle, Vector3D& velocity);

   // Remove particle from body.
   // If remaining particles disconnected, create
   // new body containing partitioned set of particles.
   void removeParticle(Body *body, Particle *particle);

   // Is particle valid?
   bool isValidParticle(Particle *particle);

   // Bond particles.
   // If particles in different bodies, combine bodies.
   Bond *createBond(Particle *particle1, Particle *particle2);

   // Remove bond.
   // If remaining particles disconnected, create
   // new body containing partitioned set of particles.
   void removeBond(Bond *bond);

   // Step system by given time increment.
   void step(double dtime);

private:

   // Mark connected sets of particles.
   // Return number of partitions.
   int markParticlePartitions(Body *body);

   // Mark connected particles.
   void markParticles(Particle *particle1);

   // Partition marked particles into separate bodies.
   void partitionParticles(Body *body);

   // Particle collisions.
   class Collision
   {
public:

      Particle  *particle1;
      Particle  *particle2;
      Vector3D  vCollisionNormal;
      Vector3D  vCollisionPoint;
      Vector3D  vRelativeVelocity;
      Collision *next;

      Collision()
      {
         particle1 = particle2 = NULL;
         next      = NULL;
      }
   };
   Collision *collisions;

   // Check for collisions with body's particles.
   void checkCollisions(Body *body1);

   // Resolve collisions.
   void resolveCollisions();

   // Update charge forces.
   void updateChargeForces(Body *body1);
};
#endif
