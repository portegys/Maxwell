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
 * Orient morphogen - orient particles.
 * ORIENT signal parameters: direction and mirrored value.
 */

#include "OrientMorph.hpp"

// Constructor.
OrientMorph::OrientMorph()
{
   int value[2];

   scope = ScopeFactory::newScope();
   scope->newValue(value);
   ORIENT = new Compound(value, 2);
   assert(ORIENT != NULL);
}


// Destructor.
OrientMorph::~OrientMorph()
{
   delete scope;
   delete ORIENT;
}


// Morph.
void OrientMorph::morph(Cell *cell)
{
   unsigned long type, direction, m;
   bool          mirrored;
   Emission      *emission;
   Particle      *particle;

   std::list<Particle *>::const_iterator listItr;

   for (emission = cell->absorption; emission != NULL;
        emission = emission->next)
   {
      if (emission->delay > 0)
      {
         continue;
      }

      // Orient?
      if ((emission->signal != NULL) && emission->signal->type->equals(ORIENT))
      {
         direction = (unsigned long)(emission->signal->parameters[0]);
         m         = (unsigned long)(emission->signal->parameters[1]);
         if (m == 1)
         {
            mirrored = true;
         }
         else
         {
            mirrored = false;
         }
         type = (unsigned long)(emission->signal->parameters[2]);

         // Check particles in cell.
         for (listItr = cell->particles.begin();
              listItr != cell->particles.end(); listItr++)
         {
            particle = *listItr;
            if (particle->type != type)
            {
               continue;
            }

            // Orient particle.
            particle->orientation.mirrored  = mirrored;
            particle->orientation.direction = direction;
         }
      }
   }
}


// Create an orient signal.
Signal *OrientMorph::createOrientSignal(Orientation orientation, int targetType)
{
   void **parameters = new void *[3];

   assert(parameters != NULL);
   parameters[0] = (void *)orientation.direction;
   if (orientation.mirrored)
   {
      parameters[1] = (void *)1;
   }
   else
   {
      parameters[1] = (void *)0;
   }
   parameters[2] = (void *)targetType;
   Signal *signal = new Signal(ORIENT->clone(), parameters);
   assert(signal != NULL);
   return(signal);
}
