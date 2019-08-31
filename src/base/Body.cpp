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
 * Body
 * A body consists of a connected set of particles.
 * Source: "Physics for Game Developers", Copyright 2000-2001 by David Bourg.
 */

#include <stdio.h>
#include "Body.hpp"
#include "Physics.h"
#include "Particle.hpp"
#include "Mechanics.hpp"

// Constructor.
Body::Body()
{
#if (USE_ENERGY == 1 || STORE_ENERGY == 1)
   energy = INITIAL_ENERGY;
#endif
   fMass      = 0.0;
   particles  = NULL;
   fixedCount = 0;
   next       = NULL;
}


// Destructor.
Body::~Body()
{
   Particle *particle;

   while (particles != NULL)
   {
      particle = particles->next;
      delete particles;
      particles = particle;
   }
}


// Calculate inertia.
void Body::calcInertia()
{
   float d = fMass / 12.0f * 4.0f; // assume body radius=1

   mInertia.e11    = d;
   mInertia.e12    = 0;
   mInertia.e13    = 0;
   mInertia.e21    = 0;
   mInertia.e22    = d;
   mInertia.e23    = 0;
   mInertia.e31    = 0;
   mInertia.e32    = 0;
   mInertia.e33    = d;
   mInertiaInverse = mInertia.Inverse();
}


// Propel body.
void Body::propel()
{
   Particle *particle;
   Vector3D force, totalForce;
   double   weight, totalWeight;

   // Accumulate weighted particle forces.
   totalWeight = 0.0;
   for (particle = particles; particle != NULL;
        particle = particle->next)
   {
      particle->propel(force, weight);
      totalForce  += (force * weight);
      totalWeight += weight;
   }
   if (totalWeight > 0.0)
   {
      vForces += totalForce / totalWeight;
   }
}


// Duplicate body.
Body *Body::duplicate(Mechanics *mechanics)
{
   int      i;
   Particle *particle, *particle2, *particle3, *particle4;

   Body *body = new Body();

   assert(body != NULL);

   // Duplicate particles.
   for (particle = particles, i = 0; particle != NULL;
        particle = particle->next, i++)
   {
      particle2        = particle->duplicate();
      particle2->body  = body;
      particle2->bonds = NULL;
      particle->mark   = particle2->mark = i;
      mechanics->addParticle(body, particle2, vVelocity);
   }

   // Duplicate bonds.
   for (particle = particles; particle != NULL;
        particle = particle->next)
   {
      for (particle2 = particles; particle2 != NULL;
           particle2 = particle2->next)
      {
         if (particle == particle2)
         {
            continue;
         }
         if (particle->getBond(particle2) != NULL)
         {
            for (particle3 = body->particles; particle3 != NULL;
                 particle3 = particle3->next)
            {
               if (particle3->mark == particle->mark)
               {
                  break;
               }
            }
            for (particle4 = body->particles; particle4 != NULL;
                 particle4 = particle4->next)
            {
               if (particle4->mark == particle2->mark)
               {
                  break;
               }
            }
            mechanics->createBond(particle3, particle4);
         }
      }
   }
   return(body);
}


// Read body.
Body *Body::read(FILE *fp, Mechanics *mechanics)
{
   int      i, p1, p2, num;
   char     buf[50];
   Particle *particle, *particle2;

   Body *body = new Body();

   assert(body != NULL);
   fscanf(fp, "%s", buf);
   body->vVelocity.x = atof(buf);
   fscanf(fp, "%s", buf);
   body->vVelocity.y = atof(buf);
   fscanf(fp, "%s", buf);
   body->vVelocity.z = atof(buf);

   // Read particles.
   fscanf(fp, "%d", &num);
   for (i = 0; i < num; i++)
   {
      particle = Particle::read(fp);
      mechanics->addParticle(body, particle, body->vVelocity);
   }

   // Read bonds.
   fscanf(fp, "%d", &num);
   for (i = 0; i < num; i++)
   {
      fscanf(fp, "%d %d", &p1, &p2);
      for (particle = body->particles; particle != NULL;
           particle = particle->next)
      {
         if (particle->mark == p1)
         {
            break;
         }
      }
      for (particle2 = body->particles; particle2 != NULL;
           particle2 = particle2->next)
      {
         if (particle2->mark == p2)
         {
            break;
         }
      }
      mechanics->createBond(particle, particle2);
   }
   return(body);
}


// Write body.
void Body::write(FILE *fp, Body *body)
{
   int      i;
   Particle *particle, *particle2;

   fprintf(fp, "%f %f %f\n", body->vVelocity.x,
           body->vVelocity.y, body->vVelocity.z);

   // Mark and write particles.
   for (particle = body->particles, i = 0; particle != NULL;
        particle = particle->next, i++)
   {
   }
   fprintf(fp, "%d\n", i);
   for (particle = body->particles, i = 0; particle != NULL;
        particle = particle->next, i++)
   {
      particle->mark = i;
      Particle::write(fp, particle);
   }

   // Write bonds.
   i = 0;
   for (particle = body->particles; particle != NULL;
        particle = particle->next)
   {
      for (particle2 = body->particles; particle2 != NULL;
           particle2 = particle2->next)
      {
         if (particle == particle2)
         {
            continue;
         }
         if (particle->getBond(particle2) != NULL)
         {
            i++;
         }
      }
   }
   fprintf(fp, "%d\n", i);
   for (particle = body->particles; particle != NULL;
        particle = particle->next)
   {
      for (particle2 = body->particles; particle2 != NULL;
           particle2 = particle2->next)
      {
         if (particle == particle2)
         {
            continue;
         }
         if (particle->getBond(particle2) != NULL)
         {
            fprintf(fp, "%d %d\n", particle->mark, particle2->mark);
         }
      }
   }
   fflush(fp);
}
