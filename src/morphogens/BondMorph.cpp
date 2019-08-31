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
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.
 */

/*
 * Bond morphogen - Bonds/unbonds particles.
 * BOND/UNBOND signal parameter: particle.
 */

#include "BondMorph.hpp"
#include "../base/Mechanics.hpp"

// Constructor.
BondMorph::BondMorph()
{
   int value[2];

   scope = ScopeFactory::newScope();
   scope->newValue(value);
   BOND = new Compound(value, 2);
   assert(BOND != NULL);
   scope->newValue(value);
   UNBOND = new Compound(value, 2);
   assert(UNBOND != NULL);
}


// Destructor.
BondMorph::~BondMorph()
{
   delete scope;
   delete BOND;
   delete UNBOND;
}


// Morph.
void BondMorph::morph(Cell *cell)
{
   unsigned long type;
   Emission      *emission;
   Particle      *particle1, *particle2;
   Bond          *bond;

   std::list<Particle *>::const_iterator listItr;

   for (emission = cell->absorption; emission != NULL;
        emission = emission->next)
   {
      if (emission->delay > 0)
      {
         continue;
      }

      // Bond?
      if ((emission->signal != NULL) && emission->signal->type->equals(BOND))
      {
         particle1 = (Particle *)(emission->signal->parameters[0]);
         type      = (unsigned long)(emission->signal->parameters[1]);

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
         }
      }

      // Unbond?
      if ((emission->signal != NULL) && emission->signal->type->equals(UNBOND))
      {
         if (emission->delay > 0)
         {
            continue;
         }

         particle1 = (Particle *)(emission->signal->parameters[0]);
         type      = (unsigned long)(emission->signal->parameters[1]);

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

            // Unbond.
            bond = particle1->getBond(particle2);
            if (bond != NULL)
            {
               mechanics->removeBond(bond);
            }
         }
      }
   }
}


// Create a bond signal.
Signal *BondMorph::createBondSignal(Particle *particle, int targetType)
{
   void **parameters = new void *[2];

   assert(parameters != NULL);
   parameters[0] = (void *)particle;
   parameters[1] = (void *)targetType;
   Signal *signal = new Signal(BOND->clone(), parameters);
   assert(signal != NULL);
   return(signal);
}


// Create an unbond signal.
Signal *BondMorph::createUnbondSignal(Particle *particle, int targetType)
{
   void **parameters = new void *[2];

   assert(parameters != NULL);
   parameters[0] = (void *)particle;
   parameters[1] = (void *)targetType;
   Signal *signal = new Signal(UNBOND->clone(), parameters);
   assert(signal != NULL);
   return(signal);
}
