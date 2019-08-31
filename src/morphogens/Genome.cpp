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

#include "Genome.hpp"
#include <assert.h>

// Constructor.
Genome::Genome()
{
   for (int i = 0; i < NUM_GENES; i++)
   {
      genes[i] = new Gene();
      assert(genes[i] != NULL);
   }
}


// Destructor.
Genome::~Genome()
{
   clear();
}


// Randomize genes.
void Genome::randomize()
{
   for (int i = 0; i < NUM_GENES; i++)
   {
      if (genes[i] != NULL)
      {
         genes[i]->randomize();
      }
   }
}


// Clear genes.
void Genome::clear()
{
   for (int i = 0; i < NUM_GENES; i++)
   {
      if (genes[i] != NULL)
      {
         delete genes[i];
         genes[i] = NULL;
      }
   }
}


// Duplicate genome.
Genome *Genome::duplicate()
{
   Genome *genome = new Genome();

   assert(genome != NULL);
   genome->clear();
   for (int i = 0; i < NUM_GENES; i++)
   {
      genome->genes[i] = genes[i]->duplicate();
   }
   return(genome);
}


// Read genome.
Genome *Genome::read(FILE *fp)
{
   Genome *genome = new Genome();

   assert(genome != NULL);
   genome->clear();
   for (int i = 0; i < NUM_GENES; i++)
   {
      genome->genes[i] = Gene::read(fp);
   }
   return(genome);
}


// Write genome.
void Genome::write(FILE *fp, Genome *genome)
{
   for (int i = 0; i < NUM_GENES; i++)
   {
      Gene::write(fp, genome->genes[i]);
      fprintf(fp, "\n");
   }
   fflush(fp);
}


// Print genome.
void Genome::print()
{
   Gene *gene;

   printf("Genome:\n");
   for (int i = 0; i < NUM_GENES; i++)
   {
      gene = genes[i];
      gene->print();
   }
}
