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

/**
 * Create morphogen - create/destroy bodies/particles.
 * CREATE signal parameter: particle type.
 * DESTROY signal parameter: none.
 */

#include "CreateMorph.hpp"
#include "../base/Mechanics.hpp"
#if (STORE_ENERGY == 1)
#include MORPHOGEN_INCLUDE
#endif

// Constructor.
CreateMorph::CreateMorph()
{
   int value[2];

   scope = ScopeFactory::newScope();
   scope->newValue(value);
   CREATE = new Compound(value, 2);
   assert(CREATE != NULL);
   scope->newValue(value);
   DESTROY = new Compound(value, 2);
   assert(DESTROY != NULL);
}


// Destructor.
CreateMorph::~CreateMorph()
{
   delete scope;
   delete CREATE;
   delete DESTROY;
}


// Morph..
void CreateMorph::morph(Cell *cell)
{
   unsigned long direction, m, type;
   bool          mirrored;
   float         dx, dy;
   Emission      *emission;
   Body          *body;
   Particle      *particle1, *particle2;

   std::list<Particle *>::iterator listItr;

   for (emission = cell->absorption; emission != NULL;
        emission = emission->next)
   {
      if (emission->delay > 0)
      {
         continue;
      }

      // Create?
      if ((emission->signal != NULL) && emission->signal->type->equals(CREATE))
      {
         particle1 = (Particle *)(emission->signal->parameters[0]);
         direction = (unsigned long)(emission->signal->parameters[1]);
         m         = (unsigned long)(emission->signal->parameters[2]);
         if (m == 1)
         {
            mirrored = true;
         }
         else
         {
            mirrored = false;
         }
         type = (unsigned long)(emission->signal->parameters[3]);

         // Cannot create if cell occupied.
         if (cell->particles.size() > 0)
         {
            continue;
         }

         // Make sure particle is valid.
         if (!mechanics->isValidParticle(particle1))
         {
            continue;
         }

#if (USE_ENERGY == 1)
         // Use energy to create particle.
         if (particle1->body->energy != INFINITE_ENERGY)
         {
            if (particle1->body->energy < PARTICLE_CREATE_ENERGY)
            {
               continue;
            }
            particle1->body->energy -= PARTICLE_CREATE_ENERGY;
         }
#endif

         // Create body.
         body = mechanics->createBody(type, DEFAULT_RADIUS, DEFAULT_MASS,
                                      DEFAULT_CHARGE);
         if (body != NULL)
         {
            body->vVelocity = particle1->body->vVelocity;
            particle2       = body->particles;
            dx = POSITION(cell->x) - POSITION(particle1->vPosition.x);
            dy = POSITION(cell->y) - POSITION(particle1->vPosition.y);
            particle2->vPosition.x = particle1->vPosition.x + dx;
            particle2->vPosition.y = particle1->vPosition.y + dy;

            // Bond particles.
            mechanics->createBond(particle1, particle2);

            // Orient particle.
            particle2->orientation.direction = direction;
            particle2->orientation.mirrored  = mirrored;

            // Only one creation permitted.
            break;
         }
      }

      // Destroy?
      if ((emission->signal != NULL) && emission->signal->type->equals(DESTROY))
      {
         if (emission->delay > 0)
         {
            continue;
         }

         type = (unsigned long)(emission->signal->parameters[0]);

         // Check particles in cell.
         for (listItr = cell->particles.begin();
              listItr != cell->particles.end(); listItr++)
         {
            particle1 = *listItr;
            if (particle1->type != type)
            {
               continue;
            }

            // Remove particle from cell.
            listItr = cell->particles.erase(listItr);

#if (STORE_ENERGY == 1)
            // Add energy for digested food.
            if ((particle1->type == DIGESTED_FOOD_TYPE) &&
                (particle1->body->energy != INFINITE_ENERGY))
            {
               particle1->body->energy += FOOD_PARTICLE_ENERGY;
            }
#endif

            // Remove particle.
            mechanics->removeParticle(particle1->body, particle1);
         }
      }
   }
}


// Create a create signal.
Signal *CreateMorph::createCreateSignal(Particle *particle,
                                        Orientation orientation, int type)
{
   void **parameters = new void *[4];

   assert(parameters != NULL);
   parameters[0] = (void *)particle;
   parameters[1] = (void *)orientation.direction;
   if (orientation.mirrored)
   {
      parameters[2] = (void *)1;
   }
   else
   {
      parameters[2] = (void *)0;
   }
   parameters[3] = (void *)type;
   Signal *signal = new Signal(CREATE->clone(), parameters);
   assert(signal != NULL);
   return(signal);
}


// Create a destroy signal.
Signal *CreateMorph::createDestroySignal(int targetType)
{
   void **parameters = new void *[1];

   assert(parameters != NULL);
   parameters[0] = (void *)targetType;
   Signal *signal = new Signal(DESTROY->clone(), parameters);
   assert(signal != NULL);
   return(signal);
}
