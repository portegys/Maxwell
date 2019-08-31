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

#include <math.h>
#include <assert.h>
#include "Gene.hpp"
#include "../base/Cell.hpp"
#include "../base/Particle.hpp"
#include "../util/Random.hpp"
#include MORPHOGEN_INCLUDE

// Parameters.
const int Gene::   MAX_TARGET_DISTANCE    = 1;
const double Gene::MAX_STRENGTH           = 5.0;
const double Gene::STRENGTH_QUANTUM       = 0.1;
const double Gene::MAX_TENDENCY_MAGNITUDE = 5.0;
const double Gene::TENDENCY_QUANTUM       = 0.1;
const int Gene::   MAX_DELAY    = 20;
const int Gene::   MAX_DURATION = 50;


// Special types.
const int Gene::IGNORE_CELL   = -1;
const int Gene::EMPTY_CELL    = -2;
const int Gene::OCCUPIED_CELL = -3;

// Constructor.
Gene::Gene()
{
   clear();
}


// Destructor.
Gene::~Gene()
{
}

// Clear gene values.
void Gene::clear()
{
   int x, y;

   // Clear body types.
   for (x = 0; x < 3; x++)
   {
      for (y = 0; y < 3; y++)
      {
         types[x][y] = IGNORE_CELL;
      }
   }

   // Initialize default action index.
   action = -1;

   // Initialize action target relative location.
   dx = 0;
   dy = 0;

   // Initialize parameters.
   type     = 0;
   strength = 0.0;
   tendency = 0.0;
   delay    = 0;
   duration = 1;
}


// Randomize gene.
void Gene::randomize()
{
   int x, y, i;

   // Randomize matching types.
   for (x = 0; x < 3; x++)
   {
      for (y = 0; y < 3; y++)
      {
         types[x][y] = Random::nextInt(NUM_PARTICLE_TYPES + 3);
         switch (types[x][y])
         {
         case NUM_PARTICLE_TYPES:
            types[x][y] = IGNORE_CELL;
            break;

         case NUM_PARTICLE_TYPES + 1:
            types[x][y] = EMPTY_CELL;
            break;

         case NUM_PARTICLE_TYPES + 2:
            types[x][y] = OCCUPIED_CELL;
            break;
         }
      }
   }

   // Randomize action index.
   action = Random::nextInt(NUM_ACTIONS);

   // Randomize target location.
   i = Random::nextInt(MAX_TARGET_DISTANCE + 1);
   if (Random::nextBoolean())
   {
      i = -i;
   }
   dx = i;
   i  = Random::nextInt(MAX_TARGET_DISTANCE + 1);
   if (Random::nextBoolean())
   {
      i = -i;
   }
   dy = i;

   // Randomize parameters.
   type = Random::nextInt(NUM_PARTICLE_TYPES);
   orientation.direction = Random::nextInt(8);
   orientation.mirrored  = Random::nextBoolean();
   i        = (int)(MAX_STRENGTH / STRENGTH_QUANTUM);
   strength = (double)(Random::nextInt(i + 1)) * STRENGTH_QUANTUM;
   i        = (int)(MAX_TENDENCY_MAGNITUDE / TENDENCY_QUANTUM);
   tendency =
      (double)(Random::nextInt(i + 1)) * TENDENCY_QUANTUM;
   if (Random::nextBoolean())
   {
      tendency = -tendency;
   }
   delay    = Random::nextInt(MAX_DELAY + 1);
   duration = Random::nextInt(MAX_DURATION + 1);
}


// Determine cell neighborhood match.
bool Gene::matchNeighborhood(Neighborhood *neighbors)
{
   int      x, y, x2, y2;
   Cell     *cell;
   Particle *particle;
   bool     match;

   std::list<Particle *>::const_iterator listItr;

   for (x = 0; x < 3; x++)
   {
      for (y = 0; y < 3; y++)
      {
         if (types[x][y] == IGNORE_CELL)
         {
            continue;
         }
         x2 = x - 1;
         y2 = y - 1;
         neighbors->getCellLocation(x2, y2);
         x2++;
         y2++;
         cell = neighbors->cells[x2][y2];
         if (cell == NULL)
         {
            return(false);
         }
         if (types[x][y] == EMPTY_CELL)
         {
            if (cell->particles.size() != 0)
            {
               return(false);
            }
         }
         else if (types[x][y] == OCCUPIED_CELL)
         {
            if (cell->particles.size() == 0)
            {
               return(false);
            }
         }
         else
         {
            match = false;
            for (listItr = cell->particles.begin();
                 listItr != cell->particles.end(); listItr++)
            {
               particle = *listItr;
               if (particle->type == types[x][y])
               {
                  match = true;
                  break;
               }
            }
            if (!match)
            {
               return(false);
            }
         }
      }
   }
   return(true);
}


// Duplicate gene.
Gene *Gene::duplicate()
{
   int x, y;

   Gene *gene = new Gene();

   assert(gene != NULL);
   for (x = 0; x < 3; x++)
   {
      for (y = 0; y < 3; y++)
      {
         gene->types[x][y] = types[x][y];
      }
   }
   gene->action      = action;
   gene->dx          = dx;
   gene->dy          = dy;
   gene->type        = type;
   gene->orientation = orientation;
   gene->strength    = strength;
   gene->tendency    = tendency;
   gene->delay       = delay;
   gene->duration    = duration;
   return(gene);
}


// Read gene.
Gene *Gene::read(FILE *fp)
{
   int  x, y, value;
   char buf[50];

   Gene *gene = new Gene();

   assert(gene != NULL);
   for (x = 0; x < 3; x++)
   {
      for (y = 0; y < 3; y++)
      {
         fscanf(fp, "%d", &gene->types[x][y]);
      }
   }
   fscanf(fp, "%d", &gene->action);
   fscanf(fp, "%d", &gene->dx);
   fscanf(fp, "%d", &gene->dy);
   fscanf(fp, "%d", &gene->type);
   fscanf(fp, "%d", &gene->orientation.direction);
   fscanf(fp, "%d", &value);
   if (value == 1)
   {
      gene->orientation.mirrored = true;
   }
   else
   {
      gene->orientation.mirrored = false;
   }
   fscanf(fp, "%s", buf);
   gene->strength = atof(buf);
   fscanf(fp, "%s", buf);
   gene->tendency = atof(buf);
   fscanf(fp, "%d", &gene->delay);
   fscanf(fp, "%d", &gene->duration);
   return(gene);
}


// Write gene.
void Gene::write(FILE *fp, Gene *gene)
{
   int x, y;

   for (x = 0; x < 3; x++)
   {
      for (y = 0; y < 3; y++)
      {
         fprintf(fp, "%d ", gene->types[x][y]);
      }
   }
   fprintf(fp, "%d ", gene->action);
   fprintf(fp, "%d ", gene->dx);
   fprintf(fp, "%d ", gene->dy);
   fprintf(fp, "%d ", gene->type);
   fprintf(fp, "%d ", gene->orientation.direction);
   if (gene->orientation.mirrored)
   {
      fprintf(fp, "1 ");
   }
   else
   {
      fprintf(fp, "0 ");
   }
   fprintf(fp, "%f ", gene->strength);
   fprintf(fp, "%f ", gene->tendency);
   fprintf(fp, "%d ", gene->delay);
   fprintf(fp, "%d ", gene->duration);
   fflush(fp);
}


// Print gene.
void Gene::print()
{
   int x, y;

   printf("types: {");
   for (x = 0; x < 3; x++)
   {
      for (y = 0; y < 3; y++)
      {
         printf("%d ", types[x][y]);
      }
   }
   printf("}");
   printf(" action=%d", action);
   printf(" location=%d,%d", dx, dy);
   printf(" parameters {");
   printf("type=%d ", type);
   printf("orientation {%d/", orientation.direction);
   if (orientation.mirrored)
   {
      printf("true");
   }
   else
   {
      printf("false");
   }
   printf("}");
   printf(" strength=%f", strength);
   printf(" tendency=%f", tendency);
   printf(" delay=%d", delay);
   printf(" duration=%d", duration);
   printf("}");
   printf("\n");
}
