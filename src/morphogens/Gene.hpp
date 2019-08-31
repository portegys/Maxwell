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
 * Gene.
 */

#ifndef __GENE__
#define __GENE__

#include <stdio.h>
#include "../base/Cell.hpp"

// Gene.
class Gene
{
public:

   // Parameters.
   static const int    MAX_TARGET_DISTANCE;
   static const double MAX_STRENGTH;
   static const double STRENGTH_QUANTUM;
   static const double MAX_TENDENCY_MAGNITUDE;
   static const double TENDENCY_QUANTUM;
   static const int    MAX_DELAY;
   static const int    MAX_DURATION;

   // Matching neighborhood body types.
   static const int IGNORE_CELL;
   static const int EMPTY_CELL;
   static const int OCCUPIED_CELL;
   int              types[3][3];

   // Action index.
   int action;

   // Action target relative location.
   int dx, dy;

   // Action parameters.
   int         type;
   Orientation orientation;
   double      strength;
   double      tendency;
   int         delay;
   int         duration;

   // Constructor.
   Gene();

   // Destructor.
   ~Gene();

   // Clear gene.
   void clear();

   // Randomize gene.
   void randomize();

   // Determine neighborhood match.
   bool matchNeighborhood(Neighborhood *neighborhood);

   // Duplicate gene.
   Gene *duplicate();

   // Read gene.
   static Gene *read(FILE *fp);

   // Write gene.
   static void write(FILE *fp, Gene *gene);

   // Print gene.
   void print();
};
#endif
