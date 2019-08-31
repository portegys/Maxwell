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
 * Body particle.
 * Source: "Physics for Game Developers", Copyright 2000-2001 by David Bourg.
 */

#include <stdio.h>
#include <assert.h>
#include "Physics.h"
#include "Cell.hpp"
#include "Orientation.hpp"
#include "Body.hpp"
#include "Bond.hpp"
#include "../util/Random.hpp"

// Constructor.
Particle::Particle(int type, float radius, float mass, float charge)
{
   this->type = type;
   fRadius    = radius;
   fMass      = mass;
   fCharge    = charge;
   coefficientOfRestitution = COEFFICIENTOFRESTITUTION;
   cell        = NULL;
   fixed       = false;
   body        = NULL;
   bonds       = NULL;
   next        = NULL;
   propulsions = NULL;
}


Particle::Particle(int type)
{
   this->type = type;
   fRadius    = DEFAULT_RADIUS;
   fMass      = DEFAULT_MASS;
   fCharge    = DEFAULT_CHARGE;
   coefficientOfRestitution = COEFFICIENTOFRESTITUTION;
   cell        = NULL;
   fixed       = false;
   body        = NULL;
   bonds       = NULL;
   next        = NULL;
   propulsions = NULL;
}


// Destructor.
Particle::~Particle()
{
   // Remove bonds from connected particles.
   while (bonds != NULL)
   {
      delete bonds;
   }

   // Delete propulsion vectors.
   struct Propulsion *p;
   while (propulsions != NULL)
   {
      p           = propulsions;
      propulsions = p->next;
      delete p;
   }
}


// Set fixed status.
void Particle::setFixed(bool fixed)
{
   if (this->fixed && !fixed && (body != NULL))
   {
      body->fixedCount--;
   }
   if (!this->fixed && fixed && (body != NULL))
   {
      body->fixedCount++;
      body->vVelocity.Zero();
   }
   this->fixed = fixed;
}


// Get bond to specific particle.
Bond *Particle::getBond(Particle *particle)
{
   Bond *bond;

   for (bond = bonds; bond != NULL; )
   {
      if (bond->particle1 == this)
      {
         if (bond->particle2 == particle)
         {
            return(bond);
         }
         else
         {
            bond = bond->next1;
         }
      }
      else
      {
         if (bond->particle1 == particle)
         {
            return(bond);
         }
         else
         {
            bond = bond->next2;
         }
      }
   }
   return(NULL);
}


// Propel particle.
void Particle::propel(Vector3D& force, double& weight)
{
   double            total, select, accum;
   struct Propulsion *propulsion, *newPropulsions;

   // Accumulate weight of current propulsions.
   total = 0.0;
   for (propulsion = propulsions; propulsion != NULL;
        propulsion = propulsion->next)
   {
      if (propulsion->delay == 0)
      {
         total += propulsion->weight;
      }
   }

   // Make a weighted selection of a propulsion.
   force.Zero();
   weight = 0.0;
   if (total > 0.0)
   {
      // Use total weight as a normalizer for random selection.
      if (total < 1.0)
      {
         total = 1.0;
      }
      select = Random::nextDouble();
      accum  = 0.0;

      for (propulsion = propulsions; propulsion != NULL;
           propulsion = propulsion->next)
      {
         if (propulsion->delay > 0)
         {
            continue;
         }

         // Select this propulsion?
         accum += (propulsion->weight / total);
         if (select < accum)
         {
            force  = propulsion->force;
            weight = propulsion->weight;
            break;
         }
      }
   }

   // Update propulsions.
   newPropulsions = NULL;
   while (propulsions != NULL)
   {
      propulsion  = propulsions;
      propulsions = propulsion->next;
      if (propulsion->delay > 0)
      {
         propulsion->delay--;
         propulsion->next = newPropulsions;
         newPropulsions   = propulsion;
      }
      else
      {
         propulsion->duration--;
         if (propulsion->duration <= 0)
         {
            delete propulsion;
         }
         else
         {
            propulsion->next = newPropulsions;
            newPropulsions   = propulsion;
         }
      }
   }
   propulsions = newPropulsions;
}


// Duplicate particle.
Particle *Particle::duplicate()
{
   Particle *particle = new Particle(type, fRadius, fMass, fCharge);

   assert(particle != NULL);
   particle->coefficientOfRestitution = coefficientOfRestitution;
   particle->cell = cell;
   particle->orientation.direction = orientation.direction;
   particle->orientation.mirrored  = orientation.mirrored;
   particle->vPosition             = vPosition;
   particle->fixed = fixed;
   particle->body  = body;
   particle->bonds = bonds;
   particle->mark  = mark;
   return(particle);
}


// Read particle.
Particle *Particle::read(FILE *fp)
{
   int      value;
   char     buf[50];
   Particle *particle = new Particle(0);

   assert(particle != NULL);
   fscanf(fp, "%d", &particle->type);
   fscanf(fp, "%s", buf);
   particle->fRadius = atof(buf);
   fscanf(fp, "%s", buf);
   particle->fMass = atof(buf);
   fscanf(fp, "%s", buf);
   particle->fCharge = atof(buf);
   fscanf(fp, "%s", buf);
   particle->coefficientOfRestitution = atof(buf);
   fscanf(fp, "%d", &particle->orientation.direction);
   fscanf(fp, "%d", &value);
   if (value == 1)
   {
      particle->orientation.mirrored = true;
   }
   else
   {
      particle->orientation.mirrored = false;
   }
   fscanf(fp, "%s", buf);
   particle->vPosition.x = atof(buf);
   fscanf(fp, "%s", buf);
   particle->vPosition.y = atof(buf);
   fscanf(fp, "%s", buf);
   particle->vPosition.z = atof(buf);
   fscanf(fp, "%d", &value);
   if (value == 1)
   {
      particle->fixed = true;
   }
   else
   {
      particle->fixed = false;
   }
   fscanf(fp, "%d", &particle->mark);
   return(particle);
}


// Write particle.
void Particle::write(FILE *fp, Particle *particle)
{
   fprintf(fp, "%d ", particle->type);
   fprintf(fp, "%f %f %f %f ", particle->fRadius,
           particle->fMass, particle->fCharge,
           particle->coefficientOfRestitution);
   if (particle->orientation.mirrored)
   {
      fprintf(fp, "%d 1 ", particle->orientation.direction);
   }
   else
   {
      fprintf(fp, "%d 0 ", particle->orientation.direction);
   }
   fprintf(fp, "%f %f %f ", particle->vPosition.x,
           particle->vPosition.y, particle->vPosition.z);
   if (particle->fixed)
   {
      fprintf(fp, "1 ");
   }
   else
   {
      fprintf(fp, "0 ");
   }
   fprintf(fp, "%d\n", particle->mark);
   fflush(fp);
}
