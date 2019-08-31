/*
 * This software is provided under the terms of the GNU General
 * Public License as published by the Free Software Foundation.
 *
 * Copyright (c) 2002 Tom Portegys, All Rights Reserved.
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
 * Propulsion morphogen - body propulsion.
 * PROPEL signal parameter: propulsion values.
 */

#include "PropelMorph.hpp"
#include "Gene.hpp"
#include "../base/Mechanics.hpp"

// Diagonal direction distance.
#define DIAG_DIST    0.707107f

// Constructor.
PropelMorph::PropelMorph()
{
   int value[2];

   scope = ScopeFactory::newScope();
   scope->newValue(value);
   PROPEL = new Compound(value, 2);
   assert(PROPEL != NULL);
}


// Destructor.
PropelMorph::~PropelMorph()
{
   delete scope;
   delete PROPEL;
}


// Morph.
void PropelMorph::morph(Cell *cell)
{
   unsigned long    type, direction;
   int              i;
   double           force;
   Emission         *emission;
   Particle         *particle;
   struct Particle::Propulsion *propulsion;

   std::list<Particle *>::const_iterator listItr;

   // Check particles in cell.
   for (listItr = cell->particles.begin();
        listItr != cell->particles.end(); listItr++)
   {
      particle = *listItr;

      // Store propulsion vectors for this particle.
      for (emission = cell->absorption; emission != NULL;
           emission = emission->next)
      {
         // Propel?
         if ((emission->signal == NULL) ||
             !emission->signal->type->equals(PROPEL))
         {
            continue;
         }

         // Signal applies to this particle?
         type = (unsigned long)(emission->signal->parameters[3]);
         if (particle->type != type)
         {
            continue;
         }

         // Get signal payload.
         direction = (unsigned long)(emission->signal->parameters[1]);
         force     = (double)((unsigned long)emission->signal->parameters[2]) *
                     Gene::STRENGTH_QUANTUM;

         // Create vector.
         propulsion           = new struct Particle::Propulsion;
         propulsion->weight   = emission->signal->strength;
         propulsion->delay    = emission->delay;
         propulsion->duration = emission->duration;
         switch (direction)
         {
         case NORTH:
            propulsion->force.y = force;
            break;

         case NORTHEAST:
            propulsion->force.y = (force * DIAG_DIST);
            propulsion->force.x = (force * DIAG_DIST);
            break;

         case EAST:
            propulsion->force.x = force;
            break;

         case SOUTHEAST:
            propulsion->force.y = -(force * DIAG_DIST);
            propulsion->force.x = (force * DIAG_DIST);
            break;

         case SOUTH:
            propulsion->force.y = -force;
            break;

         case SOUTHWEST:
            propulsion->force.y = -(force * DIAG_DIST);
            propulsion->force.x = -(force * DIAG_DIST);
            break;

         case WEST:
            propulsion->force.x = -force;
            break;

         case NORTHWEST:
            propulsion->force.y = (force * DIAG_DIST);
            propulsion->force.x = -(force * DIAG_DIST);
            break;
         }

         // Attach to particle.
         propulsion->next      = particle->propulsions;
         particle->propulsions = propulsion;

         // Release emission.
         emission->delay    = 0;
         emission->duration = 0;
      }
   }
}


// Post-morph processing.
void PropelMorph::postMorph()
{
   Body *body;

   for (body = mechanics->bodies; body != NULL; body = body->next)
   {
      body->propel();
   }
}


// Create a propel signal.
Signal *PropelMorph::createPropelSignal(Particle *particle, int direction,
                                        double force, double weight, int targetType)
{
   void **parameters = new void *[4];

   assert(parameters != NULL);
   parameters[0] = (void *)particle;
   parameters[1] = (void *)direction;
   parameters[2] = (void *)((int)(force / Gene::STRENGTH_QUANTUM));
   parameters[3] = (void *)targetType;
   Signal *signal = new Signal(PROPEL->clone(), parameters, weight);
   assert(signal != NULL);
   return(signal);
}
