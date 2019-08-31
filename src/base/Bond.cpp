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
 * Bond connecting two particles.
 */

#include <stdio.h>
#include "Bond.hpp"
#include "Physics.h"
#include "Particle.hpp"

// Maximum bond length.
const double Bond::MAX_BOND_LENGTH = 5.0;

// Constructor.
Bond::Bond(Particle *particle1, Particle *particle2)
{
   this->particle1 = particle1;
   this->particle2 = particle2;
   length          = (particle1->vPosition - particle2->vPosition).Magnitude();
   next1           = next2 = NULL;
}


// Destructor.
Bond::~Bond()
{
   // Disconnect from particles.
   disconnect(particle1);
   particle1 = NULL;
   next1     = NULL;
   disconnect(particle2);
   particle2 = NULL;
   next2     = NULL;
}


// Disconnect bond from given particle.
void Bond::disconnect(Particle *particle)
{
   Bond *bond, *bond2;

   if (particle != NULL)
   {
      for (bond = particle->bonds, bond2 = NULL;
           bond != NULL && bond != this; )
      {
         bond2 = bond;
         if (bond->particle1 == particle)
         {
            bond = bond->next1;
         }
         else
         {
            bond = bond->next2;
         }
      }
      if (bond == NULL)
      {
         return;
      }
      if (bond2 == NULL)
      {
         if (bond->particle1 == particle)
         {
            particle->bonds = bond->next1;
         }
         else
         {
            particle->bonds = bond->next2;
         }
      }
      else
      {
         if (bond->particle1 == particle)
         {
            if (bond2->next1 == bond)
            {
               bond2->next1 = bond->next1;
            }
            else
            {
               bond2->next2 = bond->next1;
            }
         }
         else
         {
            if (bond2->next1 == bond)
            {
               bond2->next1 = bond->next2;
            }
            else
            {
               bond2->next2 = bond->next2;
            }
         }
      }
   }
}
