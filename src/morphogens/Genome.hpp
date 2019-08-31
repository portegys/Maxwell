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
 * Genome.
 */

#ifndef __GENOME__
#define __GENOME__

#include "Gene.hpp"

// Parameters.
#define NUM_GENES    27

// Genome.
class Genome
{
public:

   Gene *genes[NUM_GENES];

   // Constructor.
   Genome();

   // Destructor.
   ~Genome();

   // Randomize genome.
   void randomize();

   // Clear genes.
   void clear();

   // Duplicate genome.
   Genome *duplicate();

   // Read genome.
   static Genome *read(FILE *fp);

   // Write genome.
   static void write(FILE *fp, Genome *genome);

   // Print genome.
   void print();
};
#endif
