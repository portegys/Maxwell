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

/**
 * Type morphogen - set particle type.
 * TYPE signal parameters: particle type.
 */

#include "TypeMorph.hpp"

// Constructor.
TypeMorph::TypeMorph()
{
   int value[2];

   scope = ScopeFactory::newScope();
   scope->newValue(value);
   TYPE = new Compound(value, 2);
   assert(TYPE != NULL);
}


// Destructor.
TypeMorph::~TypeMorph()
{
   delete scope;
   delete TYPE;
}


// Morph.
void TypeMorph::morph(Cell *cell)
{
   unsigned long deltaType, type;
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

      // Type change?
      if ((emission->signal != NULL) && emission->signal->type->equals(TYPE))
      {
         deltaType = (unsigned long)(emission->signal->parameters[0]);
         type      = (unsigned long)(emission->signal->parameters[1]);

         // Check particles in cell.
         for (listItr = cell->particles.begin();
              listItr != cell->particles.end(); listItr++)
         {
            particle = *listItr;
            if (particle->type != type)
            {
               continue;
            }

            // Change particle type.
            particle->type = deltaType;
         }
      }
   }
}


// Create a type signal.
Signal *TypeMorph::createTypeSignal(int deltaType, int targetType)
{
   void **parameters = new void *[2];

   assert(parameters != NULL);
   parameters[0] = (void *)deltaType;
   parameters[1] = (void *)targetType;
   Signal *signal = new Signal(TYPE->clone(), parameters);
   assert(signal != NULL);
   return(signal);
}
