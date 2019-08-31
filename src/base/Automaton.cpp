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
 * Automaton.
 * A cellular automaton in which each cell is capable of emitting
 * and absorbing signals to and from its neighbors. In each cell,
 * morphogenic functions translate absorbed signals into changes
 * in orientation, body configuration, and subsequent signal emissions.
 */

#include "Automaton.hpp"

// Constructor.
Automaton::Automaton()
{
   int x, y;

   // Initialize cells.
   for (x = 0; x < WIDTH; x++)
   {
      for (y = 0; y < HEIGHT; y++)
      {
         cells[x][y].x = x;
         cells[x][y].y = y;
      }
   }

   // Initialize morphogen.
   ScopeFactory::reset();
   morphogen.init(&mechanics);
}


// Morph.
void Automaton::morph()
{
   int          x, y, x2, y2;
   Body         *body;
   Particle     *particle;
   Neighborhood neighbors;
   Emission     *emissionList, *emission;

#if (SNAPSHOT == 1)
   // Pause for snapshot
   printf("Hit enter to step, q to quit:");
   if (getchar() == 'q')
   {
      exit(0);
   }
#endif

   // Move particles.
   mechanics.step(DTIME);
   for (body = mechanics.bodies; body != NULL; body = body->next)
   {
      for (particle = body->particles; particle != NULL;
           particle = particle->next)
      {
         x = (int)particle->vPosition.x;
         y = (int)particle->vPosition.y;
         if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT))
         {
            cells[x][y].particles.push_front(particle);
         }
      }
   }

   // Pre-morph.
   morphogen.preMorph();

   // Emit signals.
   for (x = 0; x < WIDTH; x++)
   {
      for (y = 0; y < HEIGHT; y++)
      {
         for (x2 = -1; x2 < 2; x2++)
         {
            for (y2 = -1; y2 < 2; y2++)
            {
               neighbors.cells[x2 + 1][y2 + 1] = getCell(x + x2, y + y2);
            }
         }
         emissionList = morphogen.signal(&neighbors);
         while (emissionList != NULL)
         {
            emission     = emissionList;
            emissionList = emission->next;
            x2           = x + emission->dx;
            y2           = y + emission->dy;
            if ((x2 >= 0) && (x2 < WIDTH) && (y2 >= 0) && (y2 < HEIGHT))
            {
               cells[x2][y2].absorb(emission);
            }
            else
            {
               delete emission;
            }
         }
      }
   }

   // Morph cells.
   for (x = 0; x < WIDTH; x++)
   {
      for (y = 0; y < HEIGHT; y++)
      {
         morphogen.morph(&cells[x][y]);
      }
   }

   // Post-processing.
   morphogen.postMorph();

   // Reset cells.
   for (x = 0; x < WIDTH; x++)
   {
      for (y = 0; y < HEIGHT; y++)
      {
         cells[x][y].reset();
      }
   }
}


// Get cell at location.
Cell *Automaton::getCell(int x, int y)
{
   if ((x < 0) || (x >= WIDTH))
   {
      return(NULL);
   }
   if ((y < 0) || (y >= HEIGHT))
   {
      return(NULL);
   }
   return(&cells[x][y]);
}
