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
 * Grapple morphogen - particle grappling and movement.
 * GRAPPLE signal parameter: grappling particle and movement direction.
 */

#include "GrappleMorph.hpp"
#include "../base/Mechanics.hpp"

// Constructor.
GrappleMorph::GrappleMorph()
{
   int value[2];

   scope = ScopeFactory::newScope();
   scope->newValue(value);
   GRAPPLE = new Compound(value, 2);
   assert(GRAPPLE != NULL);
}


// Destructor.
GrappleMorph::~GrappleMorph()
{
   delete scope;
   delete GRAPPLE;
}


// Morph.
void GrappleMorph::morph(Cell *cell)
{
   unsigned long type;
   float         dx, dy, dx2, dy2;
   Emission      *emission;
   Particle      *particle1, *particle2;

   std::list<Particle *>::const_iterator listItr;

   for (emission = cell->absorption; emission != NULL;
        emission = emission->next)
   {
      // Grapple?
      if ((emission->signal != NULL) && emission->signal->type->equals(GRAPPLE))
      {
         if (emission->delay > 0)
         {
            continue;
         }

         particle1 = (Particle *)(emission->signal->parameters[0]);
         dx        = (float)((unsigned long)(emission->signal->parameters[1]));
         dy        = (float)((unsigned long)(emission->signal->parameters[2]));
         type      = (unsigned long)(emission->signal->parameters[3]);

         // Make sure particle is valid.
         if (!mechanics->isValidParticle(particle1))
         {
            continue;
         }

         // Check particles in cell.
         for (listItr = cell->particles.begin();
              listItr != cell->particles.end(); listItr++)
         {
            particle2 = *listItr;
            if (particle1 == particle2)
            {
               continue;
            }
            if (particle2->type != type)
            {
               continue;
            }

            if (particle1->getBond(particle2) == NULL)
            {
               // Bond particles.
               mechanics->createBond(particle1, particle2);
            }

            // Move particle.
            dx2 = dx + POSITION(cell->x) - POSITION(particle1->vPosition.x);
            dy2 = dy + POSITION(cell->y) - POSITION(particle1->vPosition.y);
            particle2->vPosition.x = particle1->vPosition.x + dx2;
            particle2->vPosition.y = particle1->vPosition.y + dy2;
         }
      }
   }
}


// Create a grapple signal.
Signal *GrappleMorph::createGrappleSignal(Particle *particle,
                                          int dx, int dy, int targetType)
{
   void **parameters = new void *[4];

   assert(parameters != NULL);
   parameters[0] = (void *)particle;
   parameters[1] = (void *)dx;
   parameters[2] = (void *)dy;
   parameters[3] = (void *)targetType;
   Signal *signal = new Signal(GRAPPLE->clone(), parameters);
   assert(signal != NULL);
   return(signal);
}
